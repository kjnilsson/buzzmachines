#include "windows.h"
#include "../mdk/mdk.h"
#include "dgrain.h"
#include "math.h"


#define MAX_GRAINS 128

CMachineParameter const paraDecay =
{
	pt_byte,			//Type
	"Decay",			//Short name
	"Decay Probability",//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0xBB				//Default value
};
CMachineParameter const paraGrainsize =
{
	pt_word,			//Type
	"Grain Size",		//Short name
	"Grain Size",		//Long name
	100,				//Min value
	11025,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	2000				//Default value
};
CMachineParameter const paraDelaylen =
{
	pt_word,			//Type
	"Delay Length",		//Short name
	"Delay Length",		//Long name
	100,				//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	12100				//Default value
};
CMachineParameter const paraDensity =
{
	pt_word,			//Type
	"Density",		//Short name
	"Density",		//Long name
	1,				//Min value
	4000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	1000				//Default value
};
CMachineParameter const paraSpread =
{
	pt_byte,			//Type
	"Spread",			//Short name
	"Stereo Spread",//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0				//Default value
};
CMachineParameter const paraDrift =
{
	pt_byte,			//Type
	"Drift",			//Short name
	"Grain Time Drift",//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0				//Default value
};
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraDecay,
	&paraGrainsize,
	&paraDelaylen,
	&paraDensity,
	&paraSpread,
	&paraDrift

};


//Attributes
CMachineAttribute const *pAttributes[] = {
	NULL
};

enum {
	P_DECAY,
	P_GSIZE,
	P_DLEN,
	P_DENSITY,
	P_SPREAD,
	P_DRIFT

};
	
#pragma pack(1)

class gvals
{
public:
	byte decay;
	word grainsize;
	word delaylen;
	word density;
	byte spread;
	byte drift;


};

#pragma pack()

CMachineInfo const MacInfo =
{
	MT_EFFECT,						//Machine type
	MI_VERSION,						//Interface version
	MIF_DOES_INPUT_MIXING,			//Flags
	0,								//Min tracks
	0,								//Max tracks
	6,								//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"Intoxicat Grain Delay",		//Full name
	"Geez",						//Short name
	"Karl Nilsson",			//Author
	"&About...\nDon't Panic! It is meant to sound like this."						//Commands
};



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

// The main machine interface class

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop() {};
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
	
	//user funcitons
	int ActiveGrains();
	int FindNextGrain();
	//general vars
	float srfactor;//for sample rate conversions, if needed
	int spt;
	DGrain grains[MAX_GRAINS];
	
	int density_count;
	int density;//frames
	int grain_size;//frames
	int delaylen;//frames
	int timedrift;
	float decayrate;
	float stereo_spread;
	bool lastworkread;

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
}

#define MAC_VERSION	1

///////////////////////// user functions /////////////////////

int mi::FindNextGrain()
{
	for(int i = 0; i < MAX_GRAINS; i++)
	{
		if(grains[i].Status == 0) return i;
		//if(!grains[i].IsActive()) return i;
	}
	return -1;
}

int mi::ActiveGrains()
{
	int c = 0;
	for(int i = 0; i < MAX_GRAINS; i++)
	{
		if(grains[i].IsActive()) c++;
	}
	return c;

}

////////////////////////////////////////////////////////////
void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	pCB->SetnumOutputChannels(pCB->GetThisMachine(), 2);
	density_count = 0;
	density = 1000;
	grain_size = 2000;//frames
	delaylen = 12100;
	decayrate = 0.75f;
	stereo_spread = 0.0f;
	lastworkread = false;
	srfactor = (float)pMasterInfo->SamplesPerSec / 44100.0f;

	for(int i =0; i<MAX_GRAINS; i++)
	{
		grains[i].Init((11026+256) * srfactor * 2);
	}


//TODO make debug only
	#ifdef _DEBUG
		AllocConsole(); freopen("CON", "w", stdout);
	#endif
}

void mi::MDKSave(CMachineDataOutput * const po)
{
	NULL;
}

void mi::Tick()
{
	#ifdef _DEBUG
	printf("\nNum. Active Grains: : %d",ActiveGrains());
	printf("\nSRFactor: : %f",srfactor);
	#endif

	srfactor = (float)pMasterInfo->SamplesPerSec / 44100.0f; //call this every tick as sample rates might change mid session
	spt = pMasterInfo->SamplesPerTick;

	if(gval.decay != paraDecay.NoValue)
	{
		decayrate = (float)gval.decay / (float)paraDecay.MaxValue;
			
		for(int i =0; i<MAX_GRAINS; i++)
		{
			grains[i].UpdateDecay(decayrate);
		}

	}
	if(gval.grainsize != paraGrainsize.NoValue)
	{
			grain_size = gval.grainsize  * srfactor;
	}
	if(gval.delaylen != paraDelaylen.NoValue)
	{
		delaylen = gval.delaylen  * srfactor;
		if(delaylen < grain_size) delaylen = grain_size + 1;//dont watnt eh delay len shorter than grain size
	}
	if(gval.density != paraDensity.NoValue)
	{
		density = gval.density * srfactor;
	}
	if(gval.spread != paraSpread.NoValue)
	{
		stereo_spread = ((float)gval.spread / (float)paraSpread.MaxValue);// - 0.5f; 0-1f
		stereo_spread = stereo_spread;
		#ifdef _DEBUG
			printf("\nSpread: : %f",stereo_spread);
		#endif
	}
	if(gval.drift != paraDrift.NoValue)
	{
		
		//timedrift = (gval.drift * srfactor) * frand();//((float)gval.drift / (float)paraDrift.MaxValue);// - 0.5f; 0-1f
		//timedrift -= (timedrift/2);
		timedrift = gval.drift * 2 * srfactor;
		#ifdef _DEBUG
			printf("\nDrift Frames: : %d",timedrift);
		#endif
	}
}


bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{

	if(mode == WM_NOIO) return false;

	float inL = 0.0f;
	float inR = 0.0f;
	float outL = 0.0f;
	float outR = 0.0f;
	float madenoise = 0.0f;
	float rp = 0.0f;
	int g = 0;

	//read phase

	//activate new grains for input 
	if(mode & WM_READ)
	{
		if(!lastworkread) density_count = density;//ensure grain is promptly triggered after silence
		for(int i = 0; i < numsamples;i++)//shoudl be able to make this more efficient this at some point
		{
			density_count++;
			if(density_count >= density)
			{
				int n = FindNextGrain();
				if(n > -1)//FindNextGrain returns -1 if it cannot find another grain to activate
				{
					grains[n].Activate(grain_size, delaylen, 1.0f, decayrate, stereo_spread, i, timedrift);//If grain never receives any sound needs to recycle grain
				}
				density_count = 0;
			}
		}
		lastworkread = true;
	}

	for(g = 0; g < MAX_GRAINS; g++)
	{
		grains[g].Read(psamples, numsamples, mode);	
	}

	//zero out psamples buffer (no dry out)
	for (int q =0; q < numsamples *2; q++) {psamples[q] = 0.f;}
	//write phase
	for(g = 0; g < MAX_GRAINS; g++)
	{
		grains[g].GenerateAdd(psamples, numsamples);
	}
	//check if noise was made
	for(int c = 0; c < numsamples;c++)
	{
		madenoise += (float)fabs(psamples[c*2]);
	}
	
	if(madenoise > 0.f) return true;
	return false;
}




////////////////////////////////////////////////////////////////////////

void mi::Command(const int i)
{
	//The exciting about box
	char txt[10000];
	switch(i)
	{
	case 0:
		strcpy(txt,
		"Intoxicat Grain Delay v0.01\n"
		"Yohey! \n\n"
		);
		pCB->MessageBox(txt);
		break;
	case 1:

		break;
	}

}	


////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];
	float f, m;

	switch (param)
	{
	case P_DECAY: //input
		sprintf(txt,"%.2f", (float)value / (float)paraDecay.MaxValue); return txt;
		break;
	case P_GSIZE:
		sprintf(txt,"%.2fms", ((value*srfactor)/(float)pMasterInfo->SamplesPerSec) * 1000); 
		return txt;
		break;
	case P_DLEN:
		sprintf(txt,"%.2fms", ((value*srfactor)/(float)pMasterInfo->SamplesPerSec) * 1000);	
		break;
	case P_DENSITY:
		sprintf(txt,"%.2fms", ((value*srfactor)/(float)pMasterInfo->SamplesPerSec) * 1000);	
		break;
	case P_SPREAD: //input
		sprintf(txt,"%.2f", (float)value / (float)paraDecay.MaxValue); return txt;
		break;
	case P_DRIFT: //input
		sprintf(txt,"+/- %.2fms", ((value*2*srfactor)/(float)pMasterInfo->SamplesPerSec) * 1000);	
		break;
	default:
		return NULL;
	}
	return txt;
}
/*
	P_DECAY,
	P_GSIZE,
	P_DLEN,
	P_DENSITY,
	P_SPREAD,
	P_DRIFT
	*/