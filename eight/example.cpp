// Example of using PeerCtrl library
// ©2003 E Powley
// e@btd2001.freeserve.co.uk  |  http://btdsys.cjb.net
// This is just a really simple control machine with inertia

#include "../mdk/mdk.h"
#include "peerctrl.h"
#include <float.h>
#include <stdlib.h>
#include "resource.h"
#include <time.h>


////////////////////////////////////////////////////////////////////////
// Parameters


CMachineParameter const paraStep0 =
{
	pt_byte,			//Type
	"Step0",			//Short name
	"Step0 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	48,					//Default value
};
CMachineParameter const paraStep1 =
{
	pt_byte,			//Type
	"Step1",			//Short name
	"Step1 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	48,					//Default value
};

CMachineParameter const paraStep2 =
{
	pt_byte,			//Type
	"Step2",			//Short name
	"Step2 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	48,					//Default value
};
CMachineParameter const paraStep3 =
{
	pt_byte,			//Type
	"Step3",			//Short name
	"Step3 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	48,					//Default value
};
CMachineParameter const paraStep4 =
{
	pt_byte,			//Type
	"Step4",			//Short name
	"Step4 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	48,					//Default value
};
CMachineParameter const paraStep5 =
{
	pt_byte,			//Type
	"Step5",			//Short name
	"Step5 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	43,					//Default value
};
CMachineParameter const paraStep6 =
{
	pt_byte,			//Type
	"Step6",			//Short name
	"Step6 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	43,					//Default value
};
CMachineParameter const paraStep7 =
{
	pt_byte,			//Type
	"Step7",			//Short name
	"Step7 (best programmed in the parameter view but if you insist: 30 = C4)",				//Long name
	NOTE_MIN,					//Min value
	121,				//Max value
	NOTE_NO,				//No value
	MPF_STATE,			//Flags
	46,					//Default value
};
//volumes params --------------------------------------------------------------------------------
CMachineParameter const paraVolume0 =
{
	pt_byte,			//Type
	"Volume0",			//Short name
	"Volume0 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	254,				//Max value
	255,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume1 =
{
	pt_byte,			//Type
	"Volume1",			//Short name
	"Volume1 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};

CMachineParameter const paraVolume2 =
{
	pt_byte,			//Type
	"Volume2",			//Short name
	"Volume2 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume3 =
{
	pt_byte,			//Type
	"Volume3",			//Short name
	"Volume3 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume4 =
{
	pt_byte,			//Type
	"Volume4",			//Short name
	"Volume4 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume5 =
{
	pt_byte,			//Type
	"Volume5",			//Short name
	"Volume5 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume6 =
{
	pt_byte,			//Type
	"Volume6",			//Short name
	"Volume6 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};
CMachineParameter const paraVolume7 =
{
	pt_byte,			//Type
	"Volume7",			//Short name
	"Volume7 - set volume (or other parameter) level",				//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	200,				//Default value
};

CMachineParameter const paraTrigger =
{
	pt_byte,			//Type
	"Trigger",			//Short name
	"Trigger 00 - 07 for offset trigger, A0 for stop, B0 for backwards play, F0 for forwards",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,					//Flags
	0,					//Default value
};

CMachineParameter const paraStepLenght =
{
	pt_byte,			//Type
	"Step Lenght",			//Short name
	"Step Lenght - in ticks",		//Long name
	1,					//Min value
	255,				//Max value
	0xFF,				//No value
	 MPF_STATE,					//Flags
	2,					//Default value
};

CMachineParameter const paraTranspose =
{
	pt_byte,			//Type
	"Transpose",			//Short name
	"Transpose - 18 = 0",		//Long name
	0,					//Min value
	48,				//Max value
	0xFF,				//No value
	 MPF_STATE,					//Flags
	24,					//Default value
};

CMachineParameter const paraRootNote =
{
	pt_note,			//Type
	"Transpose",			//Short name
	"Transpose - 18 = 0",		//Long name
	NOTE_MIN,					//Min value
	NOTE_MAX,				//Max value
	NOTE_NO,				//No value
	0,					//Flags
	65,					//Default value
};


CMachineParameter const paraTargetTrack =
{
	pt_byte,			//Type
	"Target Track",			//Short name
	"Select the Target track of the Controlled Machine",		//Long name
	0,					//Min value
	127,				//Max value
	0xFF,				//No value
	 MPF_STATE,					//Flags
	0,					//Default value
};

CMachineParameter const paraNoteLength =
{
	pt_byte,			//Type
	"Note Length",			//Short name
	"Note Length in ticks (affects all notes in the sequence",		//Long name
	0,					//Min value
	255,				//Max value
	0xFF,				//No value
	 MPF_STATE,					//Flags
	0,					//Default value
};


////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	
	&paraStep0,
	&paraStep1,
	&paraStep2,
	&paraStep3,
	&paraStep4,
	&paraStep5,
	&paraStep6,
	&paraStep7,
	&paraVolume0,
	&paraVolume1,
	&paraVolume2,
	&paraVolume3,
	&paraVolume4,
	&paraVolume5,
	&paraVolume6,
	&paraVolume7,////////////////track
	&paraTrigger,
	&paraStepLenght,
	&paraTranspose,
	&paraRootNote,
	&paraTargetTrack,
	&paraNoteLength,
	
	
	NULL
};





CMachineAttribute const attrHeedStart = 
{
	"heed start control",
	0, // min     
	1, // max
	1 // default
};
CMachineAttribute const attrHeedStop = 
{
	"heed stop control",
	0, // min     
	1, // max
	1 // default
};

CMachineAttribute const *pAttributes[] = {

	&attrHeedStart,
	&attrHeedStop,
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

	byte Step0;
	byte Step1;
	byte Step2;
	byte Step3;
	byte Step4;
	byte Step5;
	byte Step6;
	byte Step7;
	byte Volume0;
	byte Volume1;
	byte Volume2;
	byte Volume3;
	byte Volume4;
	byte Volume5;
	byte Volume6;
	byte Volume7;
	

};

class tvals
{
public:
	byte Trigger;
	byte StepLenght;
	byte Transpose;
	byte RootNote;
	byte TargetTrack;
	byte NoteLength;
	
	
};

class avals
{
public:

	int HeedStart;
	int HeedStop;
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
	16,							//Number of global parameters
	6,							//Number of track parameters
	pParameters,					//Pointer to parameters
	2,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"intoxicat eight-n - step sequencer",			//Full name
	"8n",						//Short name
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
	virtual void Stop();
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode) { return false; };
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void Command(const int i);
	virtual void OutputModeChanged(bool stereo) {}
	virtual void mi::AttributesChanged();
	virtual void SetNumTracks(const int n);// {numTracks = n; };

	miex ex;
	CMachine *ThisMac;
	int numTracks;

	bool Initialising;

	gvals gval;
	gvals gState;
	tvals tval[128];
	avals aval;

	CPeerCtrl *ctrl[128];
	CPeerCtrl *vol_ctrl[128];

	int step_count[128];
	int lenght_count[128];
	int step_len[128];
	int triggerval[128];
	int target_track[128];
	int transpose[128];
	bool is_playing[128];
	bool backwards[128];
	bool wasPlayingLastTick;
	int heed_start;
	int heed_stop;

	bool note_triggered[128];
	int note_off_count[128];
	int note_length[128];
	int root_note[128];

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
		if (vol_ctrl[t]) delete vol_ctrl[t];
}

////////////////////////////////////////////////////////////////////////

void mi::MDKInit (CMachineDataInput * const pi)
{
	Initialising = true;
	

	//AllocConsole(); freopen("CON", "w", stdout);
	//	printf ("\nlenc: : %d",NOTE_MIN);
	//printf ("\nlenc: : %d",NOTE_MAX);

	ThisMac = pCB->GetThisMachine();

	for (int t=0; t<128; t++)
	{
		ctrl[t] = new CPeerCtrl(ThisMac, this);
		if (pi) ctrl[t]->ReadFileData(pi);
		vol_ctrl[t] = new CPeerCtrl(ThisMac, this);
		if (pi) vol_ctrl[t]->ReadFileData(pi);

		step_count[t] = 0;
		lenght_count[t] = 0;
		step_len[t] = 2;
		target_track[t] = 0;
		transpose[t] = 24;
		//triggerval[t] = 0;
		is_playing[t] = false;
			//if (heed_start_stop == 0)
			//	{
			//	is_playing[t] = true;
			//	}
		root_note[t] = 65;

		backwards[t] = false;
	}
	
	//play_on_load = 0;	
	//heed_start_stop = 1;


}

void mi::MDKSave(CMachineDataOutput * const po)
{
	for (int t=0; t<128; t++)
	{
		ctrl[t]->WriteFileData(po);
		vol_ctrl[t]->WriteFileData(po);
	}
}

////////////////////////////////////////////////////////////////////////

inline int GetParamValue(CPeerCtrl *ct, float val)
{
	const CMachineParameter *mpar = ct->GetParamInfo();
	return mpar->MinValue + (int)((val / 255) * (mpar->MaxValue - mpar->MinValue));
}
inline int GetParamNoValue(CPeerCtrl *ct) {
	const CMachineParameter *mpar = ct->GetParamInfo();
	return mpar->NoValue;
}

inline int GetParamMax(CPeerCtrl *ct) {
	const CMachineParameter *mpar = ct->GetParamInfo();
	return mpar->MaxValue;
}

inline int GetParamMin(CPeerCtrl *ct) {
	const CMachineParameter *mpar = ct->GetParamInfo();
	return mpar->MinValue;
}
/*
inline int GetIntParamValue(CPeerCtrl *ct, int val)
{
	const CMachineParameter *mpar = ct->GetParamInfo();
	float middle = (mpar->MaxValue / 255);
	//printf ("\nMid:  %f", middle);
	return int(val * middle);
}*/

inline int convert_to_buzz(int in_note, int trs, int rnote)
{
	if (in_note == 120) return 255;

	else
		{
		in_note = (in_note + (trs - 24) + (rnote));
			if (in_note >= 120) in_note = 119;
			else if (in_note <= 1) in_note = 1;
        int oct = (in_note / 12);
        oct = int(oct);
        int out_note = ((oct * 16) + ((in_note % 12) + 1));
        return out_note;
		}
}

inline int bnote_to_transpose(int bnote)
{
	int note_transp;

	note_transp = ((int(bnote / 16) * 12) + (bnote % 16));
	note_transp = note_transp - 49;
	return note_transp;
}

inline int rand_trigger()
{
	//srand( (unsigned)time( NULL ) );

	int rnd = rand();
		rnd = int(rnd / 4095.875);
		//printf ("\nlenc: : %d", rnd);
		return rnd;
}
void mi::Tick()
{

	for (int t=0; t<128; t++)
	{
		if (tval[t].StepLenght != paraStepLenght.NoValue) step_len[t] = tval[t].StepLenght;
		if (tval[t].Transpose != paraTranspose.NoValue) transpose[t] = tval[t].Transpose;
		if (tval[t].RootNote != paraRootNote.NoValue) root_note[t] = tval[t].RootNote;
		if (tval[t].NoteLength != paraNoteLength.NoValue) note_length[t] = tval[t].NoteLength;
		if (tval[t].TargetTrack != paraTargetTrack.NoValue) //chekc if target track has been changed if so send some note offs
		{
			
			if (ctrl[t]->GotParam()) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(120, 0, 0)); //sending not_off if changing the target track so that there will be no hung notes...
			target_track[t] = tval[t].TargetTrack;
		}
		//start controls
		if (heed_start == 1)
		{
			if (pCB->GetStateFlags() & SF_PLAYING) 
				{ 
				   if (!wasPlayingLastTick) 
				   {
					   for (int t=0; t<128; t++) is_playing[t] = true; //changed from stopped to playing 

					   wasPlayingLastTick = true; 
					}
				  } 
				else 
				   wasPlayingLastTick = false; 
		}

	}


	if (!Initialising) 
	{
		//loop for each track
			for (int t=0; t<numTracks; t++)
			{
				ctrl[t]->CheckMachine(); // is this needed? yes
				vol_ctrl[t]->CheckMachine();

				//increment the length count - it is init - ed at 0 for this 
				lenght_count[t] = lenght_count[t] + 1;
				//inc the note length count
				if (note_triggered) 
					note_off_count[t] = note_off_count[t] + 1;

				//if the trigger has been activated 
				if (tval[t].Trigger != paraTrigger.NoValue)
					{
						lenght_count[t] = step_len[t]; //make sure some note is triggered
						triggerval[t] = tval[t].Trigger; //assing the trigger value to a variable
						//check if this value is off then turn playing off
						if (triggerval[t] == 160) is_playing[t] = false;
						else is_playing[t] = true;
						if (triggerval[t] == 240) backwards[t] = false;
						else if (triggerval[t] == 176) backwards[t] = true;
						if (triggerval[t] == 192) step_count[t] = rand_trigger();
					}
				//printf ("\nisp:  %d",triggerval[t]);
			//note off/lenght stuff
		if ((note_off_count[t] >= note_length[t]) && (note_length[t] > 0))
		{
			if (ctrl[t]->GotParam()) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(120, 0, 0));
			note_triggered[t] = false;
		}


		//if the lenght count has reached the end do the rest
			if ((lenght_count[t] >= step_len[t]) && (is_playing[t] == true))
				{
				lenght_count[t] = 0; //reset len count as it has now reacked the step_len

					//if the trigger parameter has a value in it set count to this - also the lenght count needs to be restarted here.
					if ((tval[t].Trigger != paraTrigger.NoValue) && (tval[t].Trigger <= 16))
					{
					step_count[t] = tval[t].Trigger;
					}

					//check if the count has gone to the end of the loop, if os = reset to 0
					if (step_count[t] >= 8) step_count[t] = 0;
					else if (step_count[t] <= -1) step_count[t] = 7;

					//here we need some switch statement to send the right global note value to the target machine

						switch(step_count[t])
						{
						case 0:if ((ctrl[t]->GotParam())	&& (gState.Step0 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step0, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume0));
						break;
						case 1:if ((ctrl[t]->GotParam())	&& (gState.Step1 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step1, transpose[t], bnote_to_transpose(root_note[t])));	
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume1));
						break;
						case 2:if ((ctrl[t]->GotParam())	&& (gState.Step2 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step2, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume2));
						break;
						case 3:if ((ctrl[t]->GotParam())	&& (gState.Step3 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step3, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume3));
						break;
						case 4:if ((ctrl[t]->GotParam())	&& (gState.Step4 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step4, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume4));
						break;
						case 5:if ((ctrl[t]->GotParam())	&& (gState.Step5 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step5, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume5));
						break;
						case 6:if ((ctrl[t]->GotParam())	&& (gState.Step6 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step6, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume6));
						break;
						case 7:if ((ctrl[t]->GotParam())	&& (gState.Step7 != 121)) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(gState.Step7, transpose[t], bnote_to_transpose(root_note[t])));
							   if (vol_ctrl[t]->GotParam())	vol_ctrl[t]->ControlChange_NextTick(target_track[t], GetParamValue(vol_ctrl[t], gState.Volume7));
						break;
						} //for swith statement 
						
						//set a flag to say a note has been triggered.
						note_triggered[t] = true;
						note_off_count[t] = 0;

						//increment the step count and chekc for a bakwards flag
						
						if (!backwards[t])
							step_count[t] = step_count[t] + 1;
						else
							step_count[t] = step_count[t] - 1;

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
	if(gval.Step0 != pParameters[0]->NoValue) gState.Step0 = gval.Step0;
	if(gval.Step1 != pParameters[1]->NoValue) gState.Step1 = gval.Step1;
	if(gval.Step2 != pParameters[2]->NoValue) gState.Step2 = gval.Step2;
	if(gval.Step3 != pParameters[3]->NoValue) gState.Step3 = gval.Step3;
	if(gval.Step4 != pParameters[4]->NoValue) gState.Step4 = gval.Step4;
	if(gval.Step5 != pParameters[5]->NoValue) gState.Step5 = gval.Step5;
	if(gval.Step6 != pParameters[6]->NoValue) gState.Step6 = gval.Step6;
	if(gval.Step7 != pParameters[7]->NoValue) gState.Step7 = gval.Step7;

	if(gval.Volume0 != pParameters[8]->NoValue) gState.Volume0 = gval.Volume0;
	if(gval.Volume1 != pParameters[9]->NoValue) gState.Volume1 = gval.Volume1;
	if(gval.Volume2 != pParameters[10]->NoValue) gState.Volume2 = gval.Volume2;
	if(gval.Volume3 != pParameters[11]->NoValue) gState.Volume3 = gval.Volume3;
	if(gval.Volume4 != pParameters[12]->NoValue) gState.Volume4 = gval.Volume4;
	if(gval.Volume5 != pParameters[13]->NoValue) gState.Volume5 = gval.Volume5;
	if(gval.Volume6 != pParameters[14]->NoValue) gState.Volume6 = gval.Volume6;
	if(gval.Volume7 != pParameters[15]->NoValue) gState.Volume7 = gval.Volume7;

			
/*  - moved these to tick()
	for (int t=0; t<128; t++)
	{
		if (tval[t].StepLenght != paraStepLenght.NoValue) step_len[t] = tval[t].StepLenght;
		if (tval[t].Transpose != paraTranspose.NoValue) transpose[t] = tval[t].Transpose;
		if (tval[t].TargetTrack != paraTargetTrack.NoValue) 
		{
			
			if (ctrl[t]->GotParam()) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(120, 0));
			target_track[t] = tval[t].TargetTrack;
		}
	}
	*/
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
void mi::Stop()
{
if (heed_stop == 1)
	{
		for (int t=0; t<128; t++)
			{
			//if (ctrl[t]->GotParam()) ctrl[t]->ControlChange_NextTick(target_track[t], convert_to_buzz(120, 0));
			is_playing[t] = false;
			}
	}
}

void mi::AttributesChanged() {

	heed_start = aval.HeedStart;
	heed_stop = aval.HeedStop;
}

void mi::SetNumTracks(int const n) {
	if (numTracks < n) {
		for (int i = numTracks; i < n; i++) {
		step_len[i] = 2;
		target_track[i] = 0;
		transpose[i] = 24;
		}
	} else if (n < numTracks) {
		//for (int i = n; i < numTracks; i++)
			//tracks[i].Stop();
	}
	numTracks = n;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE dllInstance;
mi *g_mi;
CPeerCtrl *g_ctrl, *g_vol_ctrl;
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
} // this bit shouldbe ok..........

bool DoEndAssignDlg (HWND hDlg, bool ShouldSave) //this bit - who knows...?
//Store the parameters chosen (if any) in the dialog, or not
//Return value: true if should close dialog, false if not
{
	if (ShouldSave)
	{
		char MacName[128];
		char ParName[255];
		char VolName[255];

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
			else{
				g_ctrl->UnassignParameter();
				}

			s = SendMessage(GetDlgItem(hDlg,IDC_PARALIST2), 
				LB_GETCURSEL, 0, 0);
			if (s != LB_ERR)
			{
				SendMessage(GetDlgItem(hDlg,IDC_PARALIST2),
					LB_GETTEXT, s, long(&VolName));

				g_vol_ctrl->AssignParameter(MacName, VolName);
			}
			else{
				g_vol_ctrl->UnassignParameter();  //end meeeeeeeeeeeeeeeeeeeeeeeeeeeee
				}
		}
		else{
			g_ctrl->UnassignParameter();
			g_vol_ctrl->UnassignParameter();  // meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
			}

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
		/*char txt[128];
		sprintf(txt, "%s track %i, current assign: %s %s",
			g_mi->pCB->GetMachineName(g_mi->ThisMac),
			g_tracknum,
			g_ctrl->GetAssignmentString(),
			g_vol_ctrl->GetAssignmentString());
		 */

		char txt[128];
		char mac_name[128];
		char note_name[128];
		char vol_name[128];

		sprintf(note_name, g_ctrl->GetAssignmentString());
		sprintf(mac_name, g_mi->pCB->GetMachineName(g_mi->ThisMac));
		sprintf(vol_name, g_vol_ctrl->GetAssignmentString());
		sprintf(txt, "%s: note: %s; volume: %s",
			mac_name, note_name, vol_name);
		SetDlgItemText(hDlg, IDC_THISMACINFO, txt);

		//Populate the boxes ------------- not sure this works ok for the volume -dont know why argh!!!
		g_ctrl->GetMachineNamesToCombo(hDlg, 
			IDC_MACHINECOMBO, g_mi->pCB->GetMachineName(g_mi->ThisMac));

		if (g_ctrl->GotParam())
		{
			//select machine and parameter
			if (g_ctrl->SelectMachineInCombo(hDlg, IDC_MACHINECOMBO))
			{
				g_ctrl->GetParamNamesToList(
					g_ctrl->GetMachine(),
					hDlg, IDC_PARALIST, ALLOW_NOTE | ALLOW_ALL_GROUPS);

				g_ctrl->SelectParameterInList(hDlg, IDC_PARALIST);

			}
		}
		//start: meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		if (g_vol_ctrl->GotParam())
		{
			//select machine and parameter
			if (g_vol_ctrl->SelectMachineInCombo(hDlg, IDC_MACHINECOMBO))
			{
				g_vol_ctrl->GetParamNamesToList(
					g_vol_ctrl->GetMachine(),
					hDlg, IDC_PARALIST2, ALLOW_SWITCH | ALLOW_BYTE | ALLOW_WORD | ALLOW_ALL_GROUPS);

				g_vol_ctrl->SelectParameterInList(hDlg, IDC_PARALIST2);

			}
		}
		//end: meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee */
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
		
		case IDC_DEASSIGN:
			{
			g_ctrl->UnassignParameter();
			g_vol_ctrl->UnassignParameter();
			return 1;
			}

		case IDC_MACHINECOMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE) //selection is changed - this seems to work ok...
			{
				char MacName[128];
				
				if (GetDlgItemText(hDlg, IDC_MACHINECOMBO, MacName, 128) != 0)
				{	//ie if a machine is selected
					//Populate parameter list
					g_ctrl->GetParamNamesToList(
						g_mi->pCB->GetMachine(MacName),
						hDlg, IDC_PARALIST, ALLOW_NOTE | ALLOW_ALL_GROUPS);
					g_vol_ctrl->GetParamNamesToList(
						g_mi->pCB->GetMachine(MacName),
						hDlg, IDC_PARALIST2, ALLOW_SWITCH | ALLOW_BYTE | ALLOW_WORD | ALLOW_ALL_GROUPS);
					//
					//end my stuff/*/

				}
				else{
					SendMessage(GetDlgItem(hDlg, IDC_PARALIST), LB_RESETCONTENT, 0,0);
					//my stuff
					SendMessage(GetDlgItem(hDlg, IDC_PARALIST2), LB_RESETCONTENT, 0,0);
					}//end my stuff
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
/*
void miex::GetSubMenu(int const i, CMachineDataOutput *pout)
{
	char txt[128];
	for (int t=0; t<pmi->numTracks; t++)
	{
		sprintf(txt, "%i: %s, %i: %s", t, pmi->ctrl[t]->GetAssignmentString(), t, pmi->vol_ctrl[t]->GetAssignmentString());
		pout->Write(txt);
	}
}	
*/
//from note pool:
void miex::GetSubMenu(int const i, CMachineDataOutput *pout) {
	char txt[256];
	int m = 0;
	switch (i) {
	case 0:
		// assign submenu

		// i declare space for the _name strings because
		// in case 0 here, we would get eg "Primifun->Note; Primifun->Note" if we
		// called GetAssignmentString() directly in the sprintf(txt) call. i think.
		char note_name[128];
		char vol_name[128];
		for (m = 0; m < pmi->numTracks; m++) {
			sprintf(note_name, pmi->ctrl[m]->GetAssignmentString());
			sprintf(vol_name, pmi->vol_ctrl[m]->GetAssignmentString());
			sprintf(txt, "Generator %i: %s; %s",
				m, note_name, vol_name);
			pout->Write(txt);
		}		
		break;
	case 1:
		// about - no submenu
		break;
	default:
		break;
	}
}

void mi::Command(const int i)
{
	if (i>=256 && i<512)
	{
		g_mi = this;
		g_tracknum = i-256;
		g_ctrl = ctrl[i-256];
		g_vol_ctrl = vol_ctrl[i-256];

		DialogBox(dllInstance, MAKEINTRESOURCE (IDD_ASSIGN), GetForegroundWindow(), (DLGPROC) &AssignDialog);
	}
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];

	switch (param)
	{
	case 0:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 1:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 2:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));

			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 3:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 4:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 5:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 6:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i ticks", value);
		return txt;
		}
	case 7:
		{
		int oct = (value / 12);
		oct = int(oct);
		int nnote = ((value % 12));
			switch (nnote)
			{
				case 0:
					if (oct >= 10)
						sprintf(txt, "NOTE OFF %i", value);
					else
						sprintf(txt, "C %i %i", oct, value);
				break;
				case 1: 
					if (oct >= 10)
						sprintf(txt, "THRU %i", value);
					else
						sprintf(txt, "C#%i %i", oct, value);
				break;
				case 2: sprintf(txt, "D %i %i", oct, value);
				break;
				case 3: sprintf(txt, "D#%i %i", oct, value);
				break;
				case 4: sprintf(txt, "E %i %i", oct, value);
				break;
				case 5: sprintf(txt, "F %i %i", oct, value);
				break;
				case 6: sprintf(txt, "F#%i %i", oct, value);
				break;
				case 7: sprintf(txt, "G %i %i", oct, value);
				break;
				case 8: sprintf(txt, "G#%i %i", oct, value);
				break;
				case 9: sprintf(txt, "A %i %i", oct, value);
				break;
				case 10: sprintf(txt, "A#%i %i", oct, value);
				break;
				case 11: sprintf(txt, "B %i %i", oct, value);
				break;

				default: sprintf(txt, "%i ", value);
			}
		//sprintf(txt, "%i %", (value / 255) * 100);
		return txt;
		}
	case 8: sprintf(txt, "% .0f%%", value / 2.54f);
			return txt;
	case 9: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 10: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 11: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 12: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 13: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 14: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 15: sprintf(txt, "% .0f%%", value / 2.54f);
		return txt;
	case 17:sprintf(txt, "%i ticks", value);
		return txt;
	case 18: sprintf(txt, "%i", (value - 24)); return txt;
	case 21: if (value == 0) sprintf(txt, "infinite"); 
			else if (value == 1) sprintf(txt, "%i tick", value);
			else sprintf(txt, "%i ticks", value);
			return txt;
	default:
		sprintf(txt, "%i", value);
		//sprintf(txt, "%.2f%%", value / 655.34f);
		return txt;

//	default:
		//return txt;
	}
}
