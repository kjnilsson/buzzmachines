// Example of using PeerCtrl library
// ©2003 E Powley
// e@btd2001.freeserve.co.uk  |  http://btdsys.cjb.net
// This is just a really simple control machine with inertia

#include "../mdk/mdk.h"
#include "peerctrl.h"
#include <float.h>
#include <stdlib.h>
#include "resource.h"


////////////////////////////////////////////////////////////////////////
// Parameters
CMachineParameter const paraStepLenght =
{
	pt_byte,			//Type
	"Step Lenght",			//Short name
	"Step Lenght",		//Long name
	1,					//Min value
	32,				//Max value
	0xFF,				//No value
	 MPF_STATE,					//Flags
	1,					//Default value
};

CMachineParameter const paraStep0 =
{
	pt_note,			//Type
	"Step0",			//Short name
	"Step0 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	64,					//Default value
};
CMachineParameter const paraStep1 =
{
	pt_note,			//Type
	"Step1",			//Short name
	"Step1 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	65,					//Default value
};

CMachineParameter const paraStep2 =
{
	pt_note,			//Type
	"Step2",			//Short name
	"Step2 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	66,					//Default value
};
CMachineParameter const paraStep3 =
{
	pt_note,			//Type
	"Step3",			//Short name
	"Step3 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	67,					//Default value
};
CMachineParameter const paraStep4 =
{
	pt_note,			//Type
	"Step4",			//Short name
	"Step4 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	68,					//Default value
};
CMachineParameter const paraStep5 =
{
	pt_note,			//Type
	"Step5",			//Short name
	"Step5 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	69,					//Default value
};
CMachineParameter const paraStep6 =
{
	pt_note,			//Type
	"Step6",			//Short name
	"Step6 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	70,					//Default value
};
CMachineParameter const paraStep7 =
{
	pt_note,			//Type
	"Step7",			//Short name
	"Step7 (Note)",				//Long name
	NOTE_MIN,					//Min value
	NOTE_OFF,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	71,					//Default value
};



CMachineParameter const paraTrigger =
{
	pt_byte,			//Type
	"Trigger",			//Short name
	"Trigger IT",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,					//Flags
	0,					//Default value
};

////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraStepLenght,
	&paraStep0,
	&paraStep1,
	&paraStep2,
	&paraStep3,
	&paraStep4,
	&paraStep5,
	&paraStep6,
	&paraStep7,
	
	&paraTrigger,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

enum {
//	Step1,
numInertia,
NUMG = 1,
numValue = numInertia + 1,
NUMT = 1
};

#pragma pack(1)

class gvals
{
public:
	byte StepLenght;
	byte Step0;
	byte Step1;
	byte Step2;
	byte Step3;
	byte Step4;
	byte Step5;
	byte Step6;
	byte Step7;
	

};

class tvals
{
public:
	byte Trigger;
};

class avals
{
public:
};

#pragma pack()

////////////////////////////////////////////////////////////////////////
// Machine info

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT | MIF_CONTROL_MACHINE,			//Flags
	1,								//Min tracks
	128,							//Max tracks
	9,							//Number of global parameters
	1,							//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"PeerCtrl Lib Example",			//Full name
	"StepSeq Test",						//Short name
	"intoxicat",					//Author
	"/Assignments"					//Commands
};

////////////////////////////////////////////////////////////////////////
// The "miex" class

class mi;

class miex : public CMDKMachineInterfaceEx
{
public:
	mi *pmi; // pointer to 'parent'

	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);
};

////////////////////////////////////////////////////////////////////////
// The main machine interface class


struct STrack
{
	float ValCurrent, ValTarget, ValStep;
	int ValCountDown;
};

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop() {}
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode) { return false; };
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void Command(const int i);
	virtual void OutputModeChanged(bool stereo) {}
	virtual void SetNumTracks(const int n) {numTracks = n; };

	miex ex;
	CMachine *ThisMac;
	int numTracks;

	bool Initialising;

	gvals gval;
	gvals gState;
	tvals tval[128];
	avals aval;

	CPeerCtrl *ctrl[128];

	int step_count[128];
	int lenght_count;
	int step_len;

	/*
	int note0;
	int note1;
	int note2;
	int note3;
	int note4;
	int note5;
	int note6;
	int note7;
	*/
//int InertiaLength;
//bool Inertia;

	STrack tracks[128];
};

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	//GlobalVals = &gState;
	TrackVals = &tval;
	AttrVals = (int *)&aval;
	ex.pmi = this;
}

mi::~mi()
{
	for (int t=0; t<128; t++)
		if (ctrl[t]) delete ctrl[t];
}

////////////////////////////////////////////////////////////////////////

void mi::MDKInit (CMachineDataInput * const pi)
{
	Initialising = true;
	
	//step_count[t] = 0;

	AllocConsole(); freopen("CON", "w", stdout);
//	printf ("\nlenc: : %d",NOTE_MIN);
	//printf ("\nlenc: : %d",NOTE_MAX);

	ThisMac = pCB->GetThisMachine();

	for (int t=0; t<128; t++)
	{
		ctrl[t] = new CPeerCtrl(ThisMac, this);
		if (pi) ctrl[t]->ReadFileData(pi);
		step_count[t] = 0;
	}
	
	lenght_count = 0;
	step_len = 1;
//	note0 = gval.Step1;

}

void mi::MDKSave(CMachineDataOutput * const po)
{
	for (int t=0; t<128; t++)
		ctrl[t]->WriteFileData(po);

	//gState->Write(po);
}

////////////////////////////////////////////////////////////////////////

inline int GetParamValue(CPeerCtrl *ct, float val)
{
	const CMachineParameter *mpar = ct->GetParamInfo();
	return mpar->MinValue + (int)(val * (mpar->MaxValue - mpar->MinValue));
}

/*
inline int get_note (int note)
{
switch (note)
	{
case 0: return gState.Step0;
case 1: return gState.Step1;

}
*/

void mi::Tick()
{


	if (!Initialising)
	{
		//check if steplenght has changed and assign this to a var
		//if (gval.StepLenght != paraStepLenght.NoValue) step_len = gval.StepLenght;
		
		//increment the lenght count as it always comes in as a 0
		lenght_count = lenght_count + 1;
		//printf ("\nlenc: : %d",lenght_count);
		
		//if the lenght count has reached the end do the rest
		if (lenght_count >= step_len)
		{
		lenght_count = 0; //reset len count as it has now reacked the step_len

		//loop for each track
			for (int t=0; t<numTracks; t++)
			{
				ctrl[t]->CheckMachine(); // is this needed?
				
					//if the trigger parameter has a value in it set count to this - also the lenght count needs to be restarted here.
					if (tval[t].Trigger != paraTrigger.NoValue)
					{
					step_count[t] = tval[t].Trigger;
					//lenght_count = 0; //actually this wont work as we would not get this far if it wasnt 0 anyway.
					}
					
					//check if the count has gone to the end of the loop, if os = reset to 0
					if (step_count[t] >= 8)
					{
					step_count[t] = 0;
					}

					//here we need some switch statement to send the right global note value to the target machine
		
							switch(step_count[t])
							{
							case 0:if ((ctrl[t]->GotParam())	&& (gState.Step0 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step0);
							break;
							case 1:if ((ctrl[t]->GotParam())	&& (gState.Step1 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step1);	
							break;
							case 2:if ((ctrl[t]->GotParam())	&& (gState.Step2 != 1))	ctrl[t]->ControlChange_NextTick(0, gState.Step2);
							break;
							case 3:if ((ctrl[t]->GotParam())	&& (gState.Step3 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step3);
							break;
							case 4:if ((ctrl[t]->GotParam())	&& (gState.Step4 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step4);
							break;
							case 5:if ((ctrl[t]->GotParam())	&& (gState.Step5 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step5);
							break;
							case 6:if ((ctrl[t]->GotParam())	&& (gState.Step6 != 1))	ctrl[t]->ControlChange_NextTick(0, gState.Step6);
							break;
							case 7:if ((ctrl[t]->GotParam())	&& (gState.Step7 != 1)) ctrl[t]->ControlChange_NextTick(0, gState.Step7);
							break;
							} //for swith statement 
						
						//increment the step count
							printf ("\ngState:  %d",gState.Step0);
						printf ("\nstep count: : %d",step_count[t]);
						step_count[t] = step_count[t] + 1;
						

			}//track for loop
		}//for the len check

	}//init loop
}//main loop




				//if (ctrl[t]->GotParam() && tracks[t].ValCountDown > 0)
		//ctrl[t]->ControlChange_NextTick(0, GetParamValue(ctrl[t], tracks[t].ValCurrent));

////////////////////////////////////////////////////////////////////////

bool mi::MDKWork(float *psamples, int numsamples, const int mode)
{
Initialising = false;

///*
	if(gval.Step0 != pParameters[1]->NoValue) gState.Step0 = gval.Step0;
	if(gval.Step1 != pParameters[2]->NoValue) gState.Step1 = gval.Step1;
	if(gval.Step2 != pParameters[3]->NoValue) gState.Step2 = gval.Step2;
	if(gval.Step3 != pParameters[4]->NoValue) gState.Step3 = gval.Step3;
	if(gval.Step4 != pParameters[5]->NoValue) gState.Step4 = gval.Step4;
	if(gval.Step5 != pParameters[6]->NoValue) gState.Step5 = gval.Step5;
	if(gval.Step6 != pParameters[7]->NoValue) gState.Step6 = gval.Step6;
	if(gval.Step7 != pParameters[8]->NoValue) gState.Step7 = gval.Step7;
			
//*/
	if (gval.StepLenght != paraStepLenght.NoValue) step_len = gval.StepLenght;

return false;
}
//{
//	for (int t=0; t<numTracks; t++)
//	{
//		if (tracks[t].ValCountDown > 0 && ctrl[t]->GotParam())
//		{
//			tracks[t].ValCurrent += tracks[t].ValStep * numsamples;
//			tracks[t].ValCountDown -= numsamples;
//			if (tracks[t].ValCountDown <= 0)
//			{
//				tracks[t].ValCurrent = tracks[t].ValTarget;
//				ctrl[t]->ControlChange_NextTick(0, GetParamValue(ctrl[t], tracks[t].ValCurrent));
//			}
////
//			ctrl[t]->ControlChange_Immediate(0, GetParamValue(ctrl[t], tracks[t].ValCurrent));
//		}
//
//	Initialising = false;

//	return false;
//}/*

////////////////////////////////////////////////////////////////////////

HINSTANCE dllInstance;
mi *g_mi;
CPeerCtrl *g_ctrl;
int g_tracknum;

BOOL WINAPI DllMain ( HANDLE hModule, DWORD fwdreason, LPVOID lpReserved )
{
   switch (fwdreason) {
   case DLL_PROCESS_ATTACH:
      dllInstance = (HINSTANCE) hModule;
      break;

   case DLL_THREAD_ATTACH: break;
   case DLL_THREAD_DETACH: break;
   case DLL_PROCESS_DETACH: break;
   }
   return TRUE;
}

bool DoEndAssignDlg (HWND hDlg, bool ShouldSave)
//Store the parameters chosen (if any) in the dialog, or not
//Return value: true if should close dialog, false if not
{
	if (ShouldSave)
	{
		char MacName[128];
		char ParName[255];

		if (GetDlgItemText(hDlg, IDC_MACHINECOMBO, MacName, 128) != 0)
		{
			int s = SendMessage(GetDlgItem(hDlg,IDC_PARALIST), 
				LB_GETCURSEL, 0, 0);
			if (s != LB_ERR)
			{
				SendMessage(GetDlgItem(hDlg,IDC_PARALIST),
					LB_GETTEXT, s, long(&ParName));

				g_ctrl->AssignParameter(MacName, ParName);
			}
			else
				g_ctrl->UnassignParameter();
		}
		else
			g_ctrl->UnassignParameter();

		return true;
	}
	else
		return true;
}

BOOL APIENTRY AssignDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) 
//The big procedure for the GUI dialog
{
	switch(uMsg) {
	case WM_INITDIALOG:
		char txt[128];
		sprintf(txt, "%s track %i, current assign: %s",
			g_mi->pCB->GetMachineName(g_mi->ThisMac),
			g_tracknum,
			g_ctrl->GetAssignmentString());

		SetDlgItemText(hDlg, IDC_THISMACINFO, txt);

		//Populate the boxes
		g_ctrl->GetMachineNamesToCombo(hDlg, 
			IDC_MACHINECOMBO, g_mi->pCB->GetMachineName(g_mi->ThisMac));

		if (g_ctrl->GotParam())
		{
			//select machine and parameter
			if (g_ctrl->SelectMachineInCombo(hDlg, IDC_MACHINECOMBO))
			{
				g_ctrl->GetParamNamesToList(
					g_ctrl->GetMachine(),
					hDlg, IDC_PARALIST, ALLOW_ALL_TYPES | ALLOW_ALL_GROUPS);

				g_ctrl->SelectParameterInList(hDlg, IDC_PARALIST);
			}
		}
	return 1;

	case WM_SHOWWINDOW: 
		return 1;

	case WM_CLOSE:
		if (DoEndAssignDlg(hDlg, false))
			EndDialog (hDlg, TRUE);
		return 0;

	case WM_COMMAND:
		//If a button or other control is clicked or something
		switch (LOWORD (wParam))
		{
		case IDOK:
			if (DoEndAssignDlg(hDlg, true))
				EndDialog(hDlg, TRUE);
			return 1;

		case IDCANCEL:
			if (DoEndAssignDlg(hDlg, false))
				EndDialog(hDlg, TRUE);
			return 1;

		case IDC_MACHINECOMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE) //selection is changed
			{
				char MacName[128];
				
				if (GetDlgItemText(hDlg, IDC_MACHINECOMBO, MacName, 128) != 0)
				{	//ie if a machine is selected
					//Populate parameter list
					g_ctrl->GetParamNamesToList(
						g_mi->pCB->GetMachine(MacName),
						hDlg, IDC_PARALIST, ALLOW_ALL_TYPES | ALLOW_ALL_GROUPS);
				}
				else
					SendMessage(GetDlgItem(hDlg, IDC_PARALIST), LB_RESETCONTENT, 0,0);
			}

			return 1;
			break;
			
		default:
			return 0;
		}
		break;
	}
	return 0;
}

void miex::GetSubMenu(int const i, CMachineDataOutput *pout)
{
	char txt[128];
	for (int t=0; t<pmi->numTracks; t++)
	{
		sprintf(txt, "%i: %s", t, pmi->ctrl[t]->GetAssignmentString());
		pout->Write(txt);
	}
}	

void mi::Command(const int i)
{
	if (i>=256 && i<512)
	{
		g_mi = this;
		g_tracknum = i-256;
		g_ctrl = ctrl[i-256];

		DialogBox(dllInstance, MAKEINTRESOURCE (IDD_ASSIGN), GetForegroundWindow(), (DLGPROC) &AssignDialog);
	}
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];
/*
	switch (param)
	{
	case numInertia:
		sprintf(txt, "%i ticks", value);
		return txt;

	case numValue:
		sprintf(txt, "%.2f%%", value / 655.34f);
		return txt;
*/
//	default:
		return NULL;
	
}
