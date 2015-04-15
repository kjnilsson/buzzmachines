#include <windows.h>
#include "../mdk/mdk.h"
#include "math.h"
#pragma optimize ("awy", on)

//#include <float.h>
//#include <stdlib.h>
//#include "resource.h"

////////////////////////////////////////////////////////////////////////
// Parameters


CMachineParameter const paraEven =
{
	pt_byte,			//Type
	"Evens",				//Short name
	"Evens" ,		//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x7F				//Default value
};

CMachineParameter const paraOdd =
{
	pt_byte,			//Type
	"Odd",				//Short name
	"Odds",		//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x7F				//Default value
};
////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraEven,
	&paraOdd
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

#pragma pack(1)

class gvals
{
public:
	byte Even;
	byte Odd;

};

#pragma pack()

////////////////////////////////////////////////////////////////////////
// Machine info

CMachineInfo const MacInfo =
{
	MT_EFFECT,						//Machine type
	MI_VERSION,						//Interface version
	MIF_DOES_INPUT_MIXING,			//Flags
	0,								//Min tracks
	0,								//Max tracks
	2,								//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"Intoxicat Harmonic Distortion",		//Full name
	"Crash",						//Short name
	"Karl Nilsson",			//Author
	"About..."						//Commands
};

////////////////////////////////////////////////////////////////////////
// The "miex" class is needed for handling multiple inputs

class mi;

class miex : public CMDKMachineInterfaceEx
{
public:
	//virtual void AddInput(const char *macname, bool stereo);
	//virtual void DeleteInput(const char *macname);
	//virtual void RenameInput(char const *macoldname, char const *macnewname);			
	//virtual void SetInputChannels(char const *macname, bool stereo);
	//virtual void Input(float *psamples, int numsamples, float amp);
	mi *pmi; // pointer to 'parent'
};

////////////////////////////////////////////////////////////////////////
//Custom Classes:



////////////////////////////////////////////////////////////////////////
// The main machine interface class

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop() {}
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode) { return false; };
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void Command(const int i);
	virtual void OutputModeChanged(bool stereo) {}

	miex ex;

	gvals gval;


	//My Variables:
	float evenf, oddf;
	float outL, outR, ctrlL, ctrlR, lastL, lastR, tmpL, tmpR;
	//std::ofstream ofile;

};


DLL_EXPORTS
////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	ex.pmi = this;
}

mi::~mi()
{
	//ofile.close();
}

////////////////////////////////////////////////////////////////////////


#define MAC_VERSION	1

void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	pCB->SetnumOutputChannels(pCB->GetThisMachine(), 2);
	
	evenf = oddf = 0.0f;
	outL = outR = ctrlL = ctrlR = lastL = lastR = tmpL = tmpR = 0.0f;

}

void mi::MDKSave(CMachineDataOutput * const po)
{
	NULL;
}

////////////////////////////////////////////////////////////////////////

void mi::Tick()
{

	if (gval.Even != paraEven.NoValue){
		evenf = (gval.Even / (float)0xFE) - 0.5f;
	}
	if (gval.Odd != paraOdd.NoValue){
		oddf = (gval.Odd / (float)0x7E) - 1.0f;
	}

}

//////////////////////////////////////////////////////////////////////// Gui Stuff:
/*
HINSTANCE dllInstance;

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

BOOL APIENTRY ChooseDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_INITDIALOG: 
		return 1;

	case WM_SHOWWINDOW: 
		return 1;

	case WM_CLOSE:
		EndDialog (hDlg, TRUE);
		return 0;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_GROUPA:
			EndDialog(hDlg, (int)'A');
			return 1;
		default:
			return 0;
		}
		break;
	}
	return 0;
}
*/

/*
inline float intp(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}

inline int f2i(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}
////////////////////////////////////////////////////////////////////////
*/

float fltIntegrate(float f, float l)
{
	return -(f*0.25f) + (l*0.5f);
}
//The Work() function


bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{
	float inL, inR, tmp2L, tmp2R;
	
	if (mode == WM_READWRITE)
	{

		for (int i=0; i<numsamples; i++)
		{
			inL = psamples[i*2] / 32767.f;
			inR = psamples[(i*2)+1] / 32767.f;
			
			//Even
			outL = inL;// * (1.0f + (outL * evenf));
			outR = inR;// * (1.0f + (outR * evenf));

			//odd left
			outL = tmpL = (outL * ctrlL);
			outL *= ctrlL; outL *= ctrlL; outL *= ctrlL;
			tmpL *= 0.5f;
			tmp2L = tmpL * evenf;
			tmpL *= tmpL;//quadrature
			tmpL += tmp2L;
		//	fltIntegrate(tmpL, lastL);
			lastL = tmpL;
			ctrlL = 1.0f + (tmpL * oddf);
			
			
			//odd right
			outR = tmpR = (outR * ctrlR);
			outR *= ctrlR; outR *= ctrlR; outR *= ctrlR;
			tmpR *= 0.5f;
			tmp2R = tmpR * evenf;
			tmpR *= tmpR;//quadrature
			tmpR += tmp2R;
			//fltIntegrate(tmpR, lastR);
			lastR = tmpR;
			ctrlR = 1.0f + (tmpR * oddf);	

			psamples[i*2] = outL * 32767.f;
			psamples[(i*2)+1] = outR * 32767.f;
		}
	}
	else outL = outR = ctrlL = ctrlR = lastL = lastR = tmpL = tmpR = 0.0f;

	return (mode & WM_READ);
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i)
{
	//The exciting about box
	char txt[10000];
	strcpy(txt,
		"Intoxicat Harmonic Distortion v1.0\n"
		"Yohey! \n\n"
		);
	pCB->MessageBox(txt);
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];

	switch (param)
	{
	case 0: //input
		sprintf(txt,"%.0f", (float)value - (value/2));
        return txt;
		break;
	case 1: //input
		sprintf(txt,"%.0f", (float)value - (value/2));
        return txt;
		break;
	default:
		return NULL;
	}
}
