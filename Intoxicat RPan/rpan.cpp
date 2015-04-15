#include <windows.h>
#include "../mdk/mdk.h"
#include "math.h"
#pragma optimize ("awy", on)

//#include <fstream>
//#include <float.h>
//#include <stdlib.h>
//#include "resource.h"

////////////////////////////////////////////////////////////////////////
// Parameters


CMachineParameter const paraPan =
{
	pt_byte,			//Type
	"Pan",				//Short name
	"Pan Position",		//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x7F				//Default value
};
CMachineParameter const paraRand =
{
	pt_byte,			//Type
	"Rand Amt",			//Short name
	"Random Pan Amount",//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};
CMachineParameter const paraFreq =
{
	pt_word,			//Type
	"Rand Freq",			//Short name
	"Random Frequency",			//Long name
	0,					//Min value
	0xFFFE,					//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000					//Default value
};

////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraPan,
	&paraRand,
	&paraFreq
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

#pragma pack(1)

class gvals
{
public:
	byte Pan;
	byte Rand;
	word Freq;

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
	3,								//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"Intoxicat Random Panner",		//Full name
	"Pants",						//Short name
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
	float pan, lpan, rpan, randamt;
	
	float ntbl[515];
	double frac,  count, randfreq;
	void InitNtbl(){
		for(int i = 0; i < 514; i++){
			ntbl[i] = (rand()/(RAND_MAX / 2.0f));
		}
		ntbl[513] = ntbl[0];
		ntbl[514] = ntbl[1];
	};

	inline struct pans
	{
		pans()
		{
			l = r = 1.0f;
		}
		float l, r;
	};

	pans SetPans(float pos);
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
	pan = lpan = rpan = 1.0f;
	randamt = 0.0f;
	randfreq = 1.0f;
	frac = count = 0.0f;

	InitNtbl();

	//ofile.open ("c:\\rpan.txt");

}

void mi::MDKSave(CMachineDataOutput * const po)
{
	NULL;
}

////////////////////////////////////////////////////////////////////////

void mi::Tick()
{
	float f = (1.0f / pMasterInfo->SamplesPerSec);

	if (gval.Pan != paraPan.NoValue){
		pan = (gval.Pan) / 127.0f;

	}
	if (gval.Rand != paraRand.NoValue){
		randamt = gval.Rand / 255.0f;
	}
	if (gval.Freq != paraFreq.NoValue){
		randfreq =  pow(gval.Freq / (float)paraFreq.MaxValue, 2) * 20;  //1 hz 
		frac = f * randfreq; //1hz * multiplier
	}
};

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

inline mi::pans mi::SetPans(float pos)
{
	pans p;

	if (pos > 1.0f) { 
		p.l = 1.0f - (pos - 1.0f); 
		p.r = 1.0f;
		return p;
	}
	p.r = pos; 
	p.l = 1.0f; 
	return p;
}

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

//The Work() function

bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{
	int y0, y1,y2,y3;
	float f, tmp;// l ,r;
	float t = 1.0f - randamt;
	pans p;

	if (mode == WM_READWRITE)
	{

			for (int i=0; i<numsamples; i++)
			{
				tmp = (float)count;
				y1 = f2i(tmp);
				y0 = y1 - 1;
				if (y0 < 0) y0 = 512;
				y2 = y1+1;
				y3 = y1+2;
				f = tmp - y1;


				float q = intp(f, ntbl[y0], ntbl[y1], ntbl[y2], ntbl[y3]);

				p = SetPans( (q*randamt) + (pan*t) );
			//	p = SetPans( pan );
			

				psamples[i*2] *= p.l;
				psamples[(i*2)+1] *= p.r;

				count += frac;
				if(count > 512.00) count -= 512.00;
			}
	}

	return (mode & WM_READ);
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i)
{
	//The exciting about box
	char txt[10000];
	strcpy(txt,
		"Intoxicat Pants v1.0\n"
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
		break;
	case 1:
		break;
	case 2:
		sprintf(txt, "%.2f Hz", powf(value / (float)paraFreq.MaxValue, 2) * 20);
		return txt;
		break;
	default:
		return NULL;
	}
}
