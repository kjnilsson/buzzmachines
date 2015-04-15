//
//  THE BEST WAY TO MAKE A VOLUME SLIDER!
//
//  (yes, that it actually tricky if you want the best linearity for such a slider.)
//  The linaerity is heavily inspired by my 48-track mixing console, which i think is optimal
//
//
//
//	Set up your dev.studio6 project settings for your machine as follows:
//
//	Bring up project settings (Alt+F7)
//	Do the following for both Debug and Release targets
//
//	Select C/C++ Tab
//	Select Category: Preprocessor
//	Set Additional Include Directories to where you keep your Buzz header files.
//
//	Select Link Tab
//	Select Category: General
//	Set Output Filename to where you keep your Buzz effects+desired dll name (should match the one in MachineInfo below)
//	Select Category: Input
//	Set Additional Library Path to where you keep your Buzz library files (mdk.lib).
//
//	If you need debugging, do:
//	Select Debug Tab
//	Set Executable for debug session to your Buzz exe
//	Set Working directory to your Buzz dir


#include "stdlib.h"
#include <windows.h>
#include "math.h"
#include "polac.h"
#include "../mdk/mdk.h"						// you will probably have to change the path here

HINSTANCE hAppInstance=0;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason==DLL_PROCESS_ATTACH)
	{									
	
	}		
	else if (fdwReason==DLL_PROCESS_DETACH)
	{		
	
	}
	hAppInstance=hinstDLL;
	return 1;
}

CMachineParameter const paraTrigger = 
{
	pt_byte,										// type
	"Trigger",
	"Trigger",										// description
	0,												// MinValue
	4,												// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	0
};
CMachineParameter const paraMode = 
{
	pt_byte,										// type
	"Mode",
	"Mode",										// description
	0,												// MinValue
	7,												// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	0
};
CMachineParameter const paraSlice0 = 
{
	pt_byte,										// type
	"Slice 0 Gain",
	"Slice 0 Gain",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	144
};
CMachineParameter const paraS0Length = 
{
	pt_byte,										// type
	"S0 Length",
	"Slice 0 length",								// description
	0,												// MinValue
	128,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	8
};
CMachineParameter const paraS0Attack = 
{
	pt_byte,										// type
	"S0 Attack",
	"Attack Slice 0",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};

CMachineParameter const paraS0Decay = 
{
	pt_byte,										// type
	"S0 Decay",
	"Decau Slice 0",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS0Pan = 
{
	pt_byte,										// type
	"S0 Pan",
	"Pan Slice 0",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	100
};
CMachineParameter const paraSlice1 = 
{
	pt_byte,										// type
	"Slice 1 Gain",
	"Slice 1 Gain",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	144
};
CMachineParameter const paraS1Length = 
{
	pt_byte,										// type
	"S1 Length",
	"Slice 1 length",								// description
	0,												// MinValue
	128,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	8
};
CMachineParameter const paraS1Attack = 
{
	pt_byte,										// type
	"S1 Attack",
	"Attack Slice 1",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS1Decay = 
{
	pt_byte,										// type
	"S1 Decay",
	"Decay Slice 1",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS1Pan = 
{
	pt_byte,										// type
	"S1 Pan",
	"Pan Slice 1",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	100
};
CMachineParameter const paraSlice2 = 
{
	pt_byte,										// type
	"Slice 2 Gain",
	"Slice 2 Gain",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	144
};
CMachineParameter const paraS2Length = 
{
	pt_byte,										// type
	"S2 Length",
	"Slice 2 length",								// description
	0,												// MinValue
	128,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	8
};
CMachineParameter const paraS2Attack = 
{
	pt_byte,										// type
	"S2 Attack",
	"Attack Slice 2",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS2Decay = 
{
	pt_byte,										// type
	"S2 Decay",
	"Decay Slice 2",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS2Pan = 
{
	pt_byte,										// type
	"S2 Pan",
	"Pan Slice 2",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	100
};
CMachineParameter const paraSlice3 = 
{
	pt_byte,										// type
	"Slice 3 Gain",
	"Slice 3 Gain",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	144
};
CMachineParameter const paraS3Length = 
{
	pt_byte,										// type
	"S3 Length",
	"Slice 3 length",								// description
	0,												// MinValue
	128,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	8
};
CMachineParameter const paraS3Attack = 
{
	pt_byte,										// type
	"S3 Attack",
	"Attack Slice 3",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS3Decay = 
{
	pt_byte,										// type
	"S3 Decay",
	"Decay Slice 3",								// description
	0,												// MinValue
	254,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraS3Pan = 
{
	pt_byte,										// type
	"S3 Pan",
	"Pan Slice 3",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	100
};/*
CMachineParameter const paraABend = 
{
	pt_byte,										// type
	"A Bend",
	"Attack Bend",								// description
	0,												// MinValue
	0xFE,											// MaxValue
	0xFF,											// NoValue
	MPF_STATE,										// Flags
	0x80
};
CMachineParameter const paraDBend = 
{
	pt_byte,										// type
	"D Bend",
	"Decay Bend",								// description
	0,												// MinValue
	0xFE,											// MaxValue
	0xFF,											// NoValue
	MPF_STATE,										// Flags
	0x80
};*/
CMachineParameter const paraOGain = 
{
	pt_byte,										// type
	"OGain",
	"Output Gain",								// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	144
};

CMachineAttribute const attrWhatever =
{
	"Whatever",
	0,
	1,
	1
};

CMachineParameter const *pParameters[] = 
{ 
	&paraTrigger,
	&paraMode,
	&paraSlice0,
	&paraS0Length,
	&paraS0Attack,
	&paraS0Decay,
	&paraS0Pan,
	&paraSlice1,
	&paraS1Length,
	&paraS1Attack,
	&paraS1Decay,
	&paraS1Pan,
	&paraSlice2,
	&paraS2Length,
	&paraS2Attack,
	&paraS2Decay,
	&paraS2Pan,
	&paraSlice3,
	&paraS3Length,
	&paraS3Attack,
	&paraS3Decay,
	&paraS3Pan,
	//&paraABend,
	//&paraDBend,
	&paraOGain,
};

CMachineAttribute const *pAttributes[] = 
{
	&attrWhatever
};



#pragma pack(1)		

class gvals						// variables coming from the sliders
{
public:
	byte Trigger;
	byte Mode;
	
	byte Slice0;
	byte S0Length;
	byte S0Attack;
	byte S0Decay;
	byte S0Pan;

	byte Slice1;
	byte S1Length;
	byte S1Attack;
	byte S1Decay;
	byte S1Pan;

	byte Slice2;
	byte S2Length;
	byte S2Attack;
	byte S2Decay;
	byte S2Pan;

	byte Slice3;
	byte S3Length;
	byte S3Attack;
	byte S3Decay;
	byte S3Pan;

	//byte ABend;
	//byte DBend;

	byte OGain;
};

class avals						// variables coming from the attributes
{
public:
	int Whatever;
};



#pragma pack()

CMachineInfo const MacInfo = 
{
	MT_EFFECT,								// type
	MI_VERSION,
	MIF_DOES_INPUT_MIXING,					// flags
	0,										// min tracks (0=anything goes)
	0,										// max tracks
	23,										// number of global parameters
	0,										// number of per-track parameters
	pParameters,
	0,										// number of attributes
	pAttributes,
#ifdef _DEBUG
	"qSlice",								// name (when in debug mode)
#else
	"QuadSlice",							// name (for the final release version)
#endif
	"qslice",								// short name
	"Karl Nilsson",							// author
	NULL
};

class mi;
class miex : public CMDKMachineInterfaceEx
{
public:			
			//virtual char const	*DescribeParam(int const param);			
			virtual void		GetSubMenu(int const i, CMachineDataOutput *pout){};			
			mi					*pmi;
};

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();

	virtual void Tick();

	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);

	virtual void Command(int const i);
	
	virtual void MDKSave(CMachineDataOutput * const po);

	virtual char const *DescribeValue(int const param, int const value);

	virtual float const SliderToVolume(float sliderpos);
	virtual float const VolumeToSlider(float sliderpos);
	virtual float const VolumeToDB(float volume);
	virtual float const DBtoVolume(float volumedb);

	virtual void GetNextSlice(int m);
	virtual float AttackEnv(int cs);
	virtual float DecayEnv(int cs);
	virtual void SetEnvelope(int cs);

public:
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void OutputModeChanged(bool stereo) {}

	void				CreateOutBufs(void);
	void				DeleteOutBufs(void);
			
	CMachine*			thismachine;


public:
	miex ex;


	OUTINFO				outInfo;
	float				*outbuf[32];
	float				*dummybuf;


private:
	// if you have internal variables, put them here

public:
	// variables that should be available from the outside
	int s_trigger;
	int curr_slice;
	int s_mode;

	double lcount[4];
	double s_len[4];
	float sl[4];
	float s_volume[4];
	float atk[4];
	float dec[4];
	float atk_pos[4];
	float dec_pos[4];
	float atk_len[4];
	float dec_len[4];

	//float atk_bend;
	//float dec_bend;
	
	float LRamt[4];
	float RLamt[4];
	int PanPos[4];
	float ogain;

	bool eoatk;
	bool bgdec;
	bool thru;

	int gen_count;

	gvals gval;
	avals aval;

};


mi::mi()
{
	GlobalVals = &gval;
	AttrVals = (int *)&aval;
}


mi::~mi()
{
	M_Lock();
		//M_Deoffer(MAKELONG(MACH_WANT_MIDI,POLAC),this);
		M_Deoffer(MAKELONG(MACH_MULTI_OUT,POLAC),this);
	M_Unlock();	
	M_FreeMachines();
	DeleteOutBufs();
}

void mi::CreateOutBufs(void)
{	
	outInfo.numOutputs=8;
	strcpy(outInfo.machineName,"rrouter");		
	for (int i=0;i<4;i++)
	{		
		outbuf[i*2]=new float[MAX_BUFFER_LENGTH];		//left
		ZeroMemory(outbuf[i*2],MAX_BUFFER_LENGTH*sizeof(float));
		outbuf[(i*2)+1]=new float[MAX_BUFFER_LENGTH];		//right
		ZeroMemory(outbuf[(i*2)+1],MAX_BUFFER_LENGTH*sizeof(float));
		char buf[80];
		sprintf(buf,"Slice %d L",i);
		strcpy(outInfo.outputNames[i*2],buf);
		sprintf(buf,"Slice %d R",i);
		strcpy(outInfo.outputNames[(i*2)+1],buf);
	}
	dummybuf=new float[MAX_BUFFER_LENGTH];
	ZeroMemory(dummybuf,MAX_BUFFER_LENGTH*sizeof(float));
	
	for (i=8;i<32;i++) //just zeroing the remaining buffers
		outbuf[i]=dummybuf;					
	
	outInfo.out=outbuf;
}

void mi::DeleteOutBufs(void)
{
	if (dummybuf) delete[] dummybuf;
	for (int i=0;i<8;i++)
		if (outbuf[i]) delete[] outbuf[i];	
}

void mi::MDKInit(CMachineDataInput * const pi)
{
	SetOutputMode( true );		//	If true, the MDKWork will never be called, meaning that Buzz will convert a mono signal to
								//	stereo itself and call MDKWorkStereo insted.
								//	If false, MDKWork will be called in mono cases, and the output should be mono
			thismachine = pCB->GetThisMachine();	
			pCB->SetnumOutputChannels(thismachine,2);
			pCB->SetMachineInterfaceEx((CMachineInterfaceEx *)&ex);	
			CMachineInfo *cminfo=(CMachineInfo *)pCB->GetMachineInfo(thismachine);
			M_LoadMachines();			
			
			CreateOutBufs();			//MACH_MULTI_OUT
			
			//CoCreateGuid(&machID);		//MACH_WANT_MIDI
			
			M_Lock();
				//M_Offer(MAKELONG(MACH_WANT_MIDI,POLAC),this,(FARPROC)MachineCallback,thismachine,cminfo,&machID);
				M_Offer(MAKELONG(MACH_MULTI_OUT,POLAC),this,(FARPROC)MachineCallback,thismachine,cminfo,0);
			M_Unlock();
			// init GUI values
	
//AllocConsole(); freopen("CON", "w", stdout);


for(int i=0; i<4; i++)
{
	lcount[i] = 0.f;
	sl[i] = 8.0f;
	//s_len[i] = 5250; this gets initiated on work anyway...
	s_volume[i] = 1.0f;
	ogain = 1.0f;
	atk[i] = 0.0393f;
	dec[i] = 0.0393f;
	atk_pos[i] = 0;
	dec_pos[i] = 0;
	atk_len[i] = 0;
	dec_len[i] = 0;
	LRamt[i] = 0.0f;
	RLamt[i] = 0.0f;
	PanPos[i] = 100;
	mi::SetEnvelope(i);
	//printf ("\nSPT!: : %f",pMasterInfo->SamplesPerTick);
}

	//atk_bend = 1.0f;
	//atk_bend = 1.0f;

	curr_slice = 0;
	s_mode = 0;
	thru = false;
	eoatk = true;
	bgdec = false;
	gen_count = 0;


}

void mi::MDKSave(CMachineDataOutput * const po)
{
}

void mi::Tick()
{


	if (gval.Trigger != paraTrigger.NoValue) 
	{
		
			s_trigger = (gval.Trigger);

			if (s_trigger < 4)
			{
				thru = false;
				atk_pos[curr_slice] = 0;
				lcount[curr_slice] = 0;
				eoatk = false;
				curr_slice = s_trigger;
				mi::SetEnvelope(curr_slice);
				lcount[curr_slice] = 0;
				bgdec = false;
			}
			else thru = true;

	}
	
	if (gval.Mode != paraTrigger.NoValue) s_mode = (int)(gval.Mode);

	if (gval.Slice0 != paraSlice0.NoValue){
		s_volume[0] = (float)(gval.Slice0)/ 200.0f;
		s_volume[0] = mi::SliderToVolume(s_volume[0]);
		}
	if (gval.S0Length != paraS0Length.NoValue)
		{
			sl[0] = (float)(gval.S0Length)/8.0f; //just here to hold the vaue of the parameter between changes.,
			s_len[0] = pMasterInfo->SamplesPerTick * sl[0];
			mi::SetEnvelope(0);
		}
	if (gval.S0Attack != paraS0Attack.NoValue)
	{
		atk[0] = (float)(gval.S0Attack) / 254;
		mi::SetEnvelope(0);
	}
	if (gval.S0Decay != paraS0Decay.NoValue)
	{
		dec[0] = (float)(gval.S0Decay) / 254;
		mi::SetEnvelope(0);
	}

	if (gval.Slice1 != paraSlice1.NoValue){
		s_volume[1] = (float)(gval.Slice1)/ 200.0f;
		s_volume[1] = mi::SliderToVolume(s_volume[1]);;
		}
	if (gval.S1Length != paraS1Length.NoValue)
		{
			sl[1] = (float)(gval.S1Length)/8.0f; //just here to hold the vaue of the parameter between changes.,
			s_len[1] = pMasterInfo->SamplesPerTick * sl[1];
			mi::SetEnvelope(1);
		}
	if (gval.S1Attack != paraS1Attack.NoValue)
	{
		atk[1] = (float)(gval.S1Attack) / 254;
		mi::SetEnvelope(1);
	}
	if (gval.S1Decay != paraS1Decay.NoValue)
	{
		dec[1] = (float)(gval.S1Decay) / 254;
		mi::SetEnvelope(1);
	}

	if (gval.Slice2 != paraSlice2.NoValue){
		s_volume[2] = (float)(gval.Slice2)/ 200.0f;
		s_volume[2] = mi::SliderToVolume(s_volume[2]);
		};
	if (gval.S2Length != paraS2Length.NoValue)
		{
			sl[2] = (float)(gval.S2Length)/8.0f; //just here to hold the vaue of the parameter between changes.,
			s_len[2] = pMasterInfo->SamplesPerTick * sl[2];
			mi::SetEnvelope(2);
		}
	if (gval.S2Attack != paraS2Attack.NoValue)
	{
		atk[2] = (float)(gval.S2Attack) / 254;
		mi::SetEnvelope(2);
	}
	if (gval.S2Decay != paraS2Decay.NoValue)
	{
		dec[2] = (float)(gval.S2Decay) / 254;
		mi::SetEnvelope(2);
	}


	if (gval.Slice3 != paraSlice3.NoValue){
		s_volume[3] = (float)(gval.Slice3)/ 200.0f;
		s_volume[3] = mi::SliderToVolume(s_volume[3]);
		}
	if (gval.S3Length != paraS3Length.NoValue)
		{
			sl[3] = (float)(gval.S3Length)/8.0f; //just here to hold the vaue of the parameter between changes.,
			s_len[3] = pMasterInfo->SamplesPerTick * sl[3];
			mi::SetEnvelope(3);
		}
	if (gval.S3Attack != paraS3Attack.NoValue)
	{
		atk[3] = (float)(gval.S3Attack) / 254;
		mi::SetEnvelope(3);
	}
	if (gval.S3Decay != paraS3Decay.NoValue)
	{
		dec[3] = (float)(gval.S3Decay) / 254;
		mi::SetEnvelope(3);
	}
	
	if (gval.OGain != paraOGain.NoValue){
		ogain = (float)(gval.OGain)/ 200.0f;
		ogain = mi::SliderToVolume(ogain);
		}


	if (gval.S0Pan != paraS0Pan.NoValue){
		PanPos[0] = (int)gval.S0Pan;
		if (PanPos[0] > 100){	//rght pan
			LRamt[0] = (float)(PanPos[0] - 100.0f) / 100.0f;
			RLamt[0] = 0.0f;}
		else {
			RLamt[0] = 1.0f - ((float)PanPos[0] / 100.0f);
			LRamt[0] = 0.0f;}
	}
	if (gval.S1Pan != paraS1Pan.NoValue){
		PanPos[1] = (int)gval.S1Pan;
		if (PanPos[1] > 100){	//rght pan
			LRamt[1] = (float)(PanPos[1] - 100.0f) / 100.0f;
			RLamt[1] = 0.0f;}
		else {
			RLamt[1] = 1.0f - ((float)PanPos[1] / 100.0f);
			LRamt[1] = 0.0f;}
	}
	if (gval.S2Pan != paraS2Pan.NoValue){
		PanPos[2] = (int)gval.S2Pan;
		if (PanPos[2] > 100){	//rght pan
			LRamt[2] = (float)(PanPos[2] - 100.0f) / 100.0f;
			RLamt[2] = 0.0f;}
		else {
			RLamt[2] = 1.0f - ((float)PanPos[2] / 100.0f);
			LRamt[2] = 0.0f;}
	}
	if (gval.S3Pan != paraS3Pan.NoValue){
		PanPos[3] = (int)gval.S3Pan;
		if (PanPos[3] > 100){	//rght pan
			LRamt[3] = (float)(PanPos[3] - 100.0f) / 100.0f;
			RLamt[3] = 0.0f;}
		else {
			RLamt[3] = 1.0f - ((float)PanPos[3] / 100.0f);
			LRamt[3] = 0.0f;}
	}
/*
	if (gval.ABend != paraABend.NoValue){
		atk_bend = (float)gval.ABend / 0x80;
	}

*/	
	//if (gval.DBend != paraDBend.NoValue)
	
}

bool mi::MDKWork(float *psamples, int numsamples, int const mode)					// MONO
{
	//	Handle special flag cases. If we can't write to the buffer, return a proper result based on whether
	//	we could read from it. This is necessary for <thru> patterns.
	return false;

}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)				// STEREO
{
	//	Handle special flag cases. If we can't write to the buffer, return a proper result based on whether
	//	we could read from it. This is necessary for <thru> patterns.
		if (mode==WM_NOIO)
			return false;
		/*
		if (mode==WM_WRITE)
			return false;
		
		if (mode==WM_READ)                        // <thru>
            return true;*/

		if( (mode&WM_WRITE)==0 )
			return mode&WM_READ?true:false;
		if( (mode&WM_READ)==0 )
		{
			for(int i=0; i<numsamples; i++)
			{
					if (lcount[curr_slice] > (s_len[curr_slice] - dec_len[curr_slice])) bgdec = true; //chekc if decay shoudl start
					else bgdec = false;

						if (!eoatk)  //attack stuff
						{
							atk_pos[curr_slice]++; //incr atkpost

								if (atk_pos[curr_slice] > atk_len[curr_slice])
								{
									atk_pos[curr_slice] = 0;
									eoatk = true;
								}
						}
						if (bgdec)  //decay stuff
						{
							dec_pos[curr_slice]++;

						}

					lcount[curr_slice] = lcount[curr_slice] + 1;//increment slice length counter
					
					if (lcount[curr_slice] >= s_len[curr_slice])
					{	
						dec_pos[curr_slice] = 0;
						bgdec = false;

						mi::GetNextSlice(s_mode);
						mi::SetEnvelope(curr_slice);
						
						if (atk_len[curr_slice] > 0) eoatk = false;
						else eoatk = true;

						dec_pos[curr_slice] = 0;
						
					}
			}
			return false;	//	Tell Buzz we haven't produced any output.
		}
		else
		{

		float inL, inR, outL, outR;
		bool madenoise;
		
			for(int s=0; s<4; s++){ //set the envolopes in case the tempo has changed
				mi::SetEnvelope(s);
				s_len[s] = pMasterInfo->SamplesPerTick * sl[s];
				}


			for(int i=0; i<numsamples; i++)
			{

				outL = inL = psamples[2*i];
				outR = inR = psamples[2*i+1];
				madenoise = true;

					if (!thru)
					{

					if (lcount[curr_slice] > (s_len[curr_slice] - dec_len[curr_slice])) bgdec = true; //chekc if decay shoudl start
					else bgdec = false;


						outL = outL * s_volume[curr_slice];
						outR = outR * s_volume[curr_slice];

						outL = outL * (1.0f - LRamt[curr_slice]) + (inR * RLamt[curr_slice]); //pan stuff
						outR = outR * (1.0f - RLamt[curr_slice]) + (inL * LRamt[curr_slice]);
						
						if (!eoatk)  //attack stuff
						{
								if(atk_len[curr_slice] > 0)
								{
								outL = outL * mi::AttackEnv(curr_slice);
								outR = outR * mi::AttackEnv(curr_slice);
								atk_pos[curr_slice]++; //incr atkpost
								}
								if (atk_pos[curr_slice] >= atk_len[curr_slice])
								{
									atk_pos[curr_slice] = 0;
									eoatk = true;
								}
						}

						if (bgdec)  //decay stuff
						{
							dec_pos[curr_slice]++;
							outL = outL * mi::DecayEnv(curr_slice);
							outR = outR * mi::DecayEnv(curr_slice);

						}
					}

					
					outL *= ogain;
					outR *= ogain;
					

					//multi out stuff
					outbuf[0][i] = 0.0f;
					outbuf[1][i] = 0.0f;
					outbuf[2][i] = 0.0f;
					outbuf[3][i] = 0.0f;
					outbuf[4][i] = 0.0f;
					outbuf[5][i] = 0.0f;
					outbuf[6][i] = 0.0f;
					outbuf[7][i] = 0.0f;

					if (!thru)
					{
					outbuf[2*curr_slice][i] = outL/0x8000;
					outbuf[(2*curr_slice)+1][i] = outR/0x8000;
					}

					psamples[2*i]   = outL;		// left
					psamples[2*i+1] = outR;		// right
				
				
					if (outL > 0.0f)
						madenoise = true;
					else if (outR > 0.0f)
						madenoise = true;
				

					lcount[curr_slice] = lcount[curr_slice] + 1;//increment slice length counter
					
						if (lcount[curr_slice] >= s_len[curr_slice])
						{	
							dec_pos[curr_slice] = 0;
							bgdec = false;

							atk_pos[curr_slice] = 0;
							eoatk = true;

							mi::GetNextSlice(s_mode);
							mi::SetEnvelope(curr_slice);
							
							if (atk_len[curr_slice] > 0) eoatk = false;
							else eoatk = true;

							dec_pos[curr_slice] = 0;
			
						}
								
			}
				
			if (!madenoise) return false;
			else {
				return true;
				madenoise = false;}
		
		}//else
}//end of work




void mi::GetNextSlice(int const m)
{
	int temp;
	switch(m)
	{
	case 0:  //normal
		lcount[curr_slice] = 0;
		curr_slice = curr_slice + 1;
		if (curr_slice > 3) {
			curr_slice = 0;
			}
		lcount[curr_slice] = 0;
		
	break;
	case 1: //reverse
		lcount[curr_slice] = 0;
		curr_slice = curr_slice - 1;
		if (curr_slice < 0) {
			curr_slice = 3;
		}
		lcount[curr_slice] = 0;
	break;
	case 2: //classic 0101
		lcount[curr_slice] = 0;
		curr_slice = curr_slice + 1;
		if (curr_slice > 1) {
			curr_slice = 0;
			}
		lcount[curr_slice] = 0;
		
	break;
	case 4:
		if (gen_count < 4){
			lcount[curr_slice] = 0;
			curr_slice = curr_slice + 1;
			if (curr_slice > 1) {
				curr_slice = 0;
				}
			lcount[curr_slice] = 0;
			gen_count++;
		}
		else{
			lcount[curr_slice] = 0;
			curr_slice = curr_slice + 1;
			if (curr_slice > 3) {
				curr_slice = 2;
				}
			lcount[curr_slice] = 0;
			gen_count++;
		}
		if (gen_count >= 7) gen_count = 0;


	case 7: //random
		lcount[curr_slice] = 0;
		temp = (int)(rand() / ((RAND_MAX-1) /4));
		curr_slice = (int)(temp);
		lcount[curr_slice] = 0;

	break;
	default:
		lcount[curr_slice] = 0;
		curr_slice = curr_slice + 1;
		if (curr_slice > 3) {
			curr_slice = 0;
			lcount[curr_slice] = 0;
			}
		//printf ("\nWhat??!: : %d",curr_slice);
	break;

	}


}

float mi::AttackEnv(int cs)
{
	float c;

	c = 1.0f / atk_len[cs] * atk_pos[cs];

	return c;
}

float mi::DecayEnv(int cs)
{
	float c;

	c = 1.0f / dec_len[cs] * (dec_len[cs] - dec_pos[cs]);

	return c;
}

void mi::SetEnvelope(int cs)
{

	atk_len[cs] = (float)s_len[cs] * atk[cs];
	dec_len[cs] = (float)s_len[cs] * dec[cs];

}

void mi::Command(int const i)
{
}


char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	float translatedvolume = 0.0f;
	float envcalc = 0.0f;

	switch(param)
	{
	case 0:
		switch(value)
		{
		case 4:
			sprintf(txt, "thru");
		break;
		default:
			sprintf(txt, "%d", value);
		break;
		return txt;
		}
	break;
	case 1:
		switch(value)
		{
		case 0:
			sprintf(txt, "normal");
		break;
		case 1:
			sprintf(txt, "reverse");
		break;
		case 2:
			sprintf(txt, "Classic 0101");
		break;
		case 3:
			sprintf(txt, "0213");
		break;
		case 4:
			sprintf(txt, "01012323");
		break;
		case 5:
			sprintf(txt, "normal");
		break;
		case 6:
			sprintf(txt, "normal");
		break;
		case 7:
			sprintf(txt, "random");
		break;
		default:
			sprintf(txt, "%d", value);
		}
		return txt;
		break;
	case 2:							// Volume
		translatedvolume = (float)value / 200.0f;
		translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;
	case 3:
		sprintf(txt, "%i Tick %i/8", (int)(value / 8), value % 8);
		return txt;
		break;
	case 4:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 5:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 6:
		sprintf(txt, "%i", value - 100);
		return txt;
		break;
	case 7:							// Volume
		translatedvolume = (float)value / 200.0f;
		translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;
	case 8:
		sprintf(txt, "%i Tick %i/8", (int)(value / 8), value % 8);
		return txt;
		break;
	case 9:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 10:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 11:
		sprintf(txt, "%i", value - 100);
		return txt;
		break;
	case 12:							// Volume
		translatedvolume = (float)value / 200.0f;
		translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;
	case 13:
		sprintf(txt, "%i Tick %i/8", (int)(value / 8), value % 8);
		return txt;
		break;
	case 14:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 15:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 16:
		sprintf(txt, "%i", value - 100);
		return txt;
		break;
	case 17:							// Volume
		translatedvolume = (float)value / 200.0f;
		translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;
	case 18:
		sprintf(txt, "%i Tick %i/8", (int)(value / 8), value % 8);
		return txt;
		break;
	case 19:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 20:
		envcalc =  ((float)value / 254 * 100);
		sprintf(txt, "%.1f%%", envcalc);
		return txt;
		break;
	case 21:
		sprintf(txt, "%i", value - 100);
		return txt;
		break;
	case 22:
		translatedvolume = (float)value / 200.0f;
		translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;


	default:
		return NULL;
	}

	return txt;
}




// my fancy conversions

float const mi::SliderToVolume(float sliderpos)		// by Joachim
{
	float x = 0.0f;
	x = powf(sliderpos, 3.5f)*3.1748f;
	if (sliderpos<=0)  x=0.0f;
	return x;
}

float const mi::VolumeToSlider(float volume)		// by Joachim
{
	float x = 0.0f;
	if (x>0)  x = powf(volume/3.1748f, 1/3.5f);
	return x;
}

float const mi::VolumeToDB(float volume)			// by Joachim
{
	float x = -1000.0f;
	if (volume>0.0f)
	{
		x = logf(volume)/logf(2.0f)*6.0f;
	}
	return x;
}

float const mi::DBtoVolume(float volumedb)			// by Joachim
{
	float x = 0.0f;
	if (volumedb>0.0f)
	{
		x = logf(volumedb)/logf(2.0f)*6.0f;
	}
	return x;
}

/////////////////////////////////////////////MULTI-OUT////////////////////////////////////////////////////////




#pragma optimize ("", on)




int							M_loaded=0;
HMODULE						hm_dock=0;
int							mcount=0;

extern int midi_in_mode;


void (__cdecl *Offer)(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM, CMachineInfo *pInfo, void* pOpt);
int (__cdecl *Deoffer)(int opcode, CMachineInterface* pMI);
int (__cdecl *getListIndex)(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);
int (__cdecl *findListElement)(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);
void (__cdecl *Lock)(void);
void (__cdecl *Unlock)(void);


int __cdecl MachineCallback(CMachineInterface* pMI, int opcode, int iVal, int iVal2, float fVal, void *pV)
{	
	if (HIWORD(opcode)==POLAC)
	{		
		switch(LOWORD(opcode))
		{
		case MACH_WANT_MIDI:
			{
				mi* p=(mi*)pMI;
				if (pV)
				{
				return 0;			

				}
				
			}
			break;
		case MACH_MULTI_OUT:
			{		
				mi* p=(mi*)pMI;			
				if (iVal)
				{					
					void** ppV=(void**)iVal;
					*ppV=(void**)&p->outInfo;
					return 1;
				}								
			}
			break;	
		}
	}	
	return -1;
}

void __cdecl M_Lock(void)
{
	if (M_loaded)
		Lock();
}

void __cdecl M_Unlock(void)
{
	if (M_loaded)
		Unlock();
}

int __cdecl M_getListIndex(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return getListIndex(n, opcode, ppMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

int __cdecl M_findListElement(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return findListElement(opcode,pMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

void __cdecl M_Offer(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM,  CMachineInfo *pInfo, void* pOpt)
{
	if (M_loaded)
		Offer(opcode, pMI, callback, pM, pInfo, pOpt);
}

int __cdecl M_Deoffer(int opcode, CMachineInterface* pMI)
{
	if (M_loaded)
		return Deoffer(opcode, pMI);
	return -1;
}

bool __cdecl M_IsActive(void)
{
	if (M_loaded) return true;
	return false;
}

void __cdecl M_Save(CMachineDataOutput *po, GUID *machID)
{
	po->Write((void *)machID,sizeof(GUID));
}

void __cdecl M_Load(CMachineDataInput *pi, GUID *machID)
{
	pi->Read((void *)machID,sizeof(GUID));
}

void __cdecl M_LoadMachines(void) 
{	
	mcount++;
	if (mcount==1)
	{		
		char buzz_dock_dll[MAX_PATH];
		buzz_dock_dll[0]=0;
		HKEY hk;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Jeskola\\Buzz\\Settings", 0, KEY_QUERY_VALUE,&hk)==ERROR_SUCCESS)
		{		
			DWORD dwType;
			DWORD dwSize=MAX_PATH;					
			if (RegQueryValueEx(hk,"BuzzPath",NULL,&dwType,(BYTE *)buzz_dock_dll,&dwSize)==ERROR_SUCCESS)
			{							
				int len=strlen(buzz_dock_dll);
				if (buzz_dock_dll[len-1]!='\\') 
				{
					buzz_dock_dll[len]='\\';
					buzz_dock_dll[len+1]=0;
				}				
				strcat(buzz_dock_dll,"Gear\\Machines.dll");			
			}
			RegCloseKey(hk);	
		}
		hm_dock=LoadLibrary(buzz_dock_dll);		
		if (hm_dock)
		{		
			Offer=(void (__cdecl *)(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM, CMachineInfo *pInfo, void* pOpt))GetProcAddress(hm_dock,"Offer");
			Deoffer=(int (__cdecl *)(int opcode, CMachineInterface* pMI))GetProcAddress(hm_dock,"Deoffer");
			Lock=(void (__cdecl *)(void))GetProcAddress(hm_dock,"Lock");		
			Unlock=(void (__cdecl *)(void))GetProcAddress(hm_dock,"Unlock");		
			getListIndex=(int (__cdecl *)(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt))GetProcAddress(hm_dock,"getListIndex");
			findListElement=(int (__cdecl *)(int opcode, CMachineInterface* pMI,FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt))GetProcAddress(hm_dock,"findListElement");
			if (Offer&&Deoffer&&Lock&&Unlock&&getListIndex&&findListElement)
			{
				M_loaded=1;		
				return;
			}
		}	
		M_loaded=0;	
	}	
}



void __cdecl M_FreeMachines(void)
{
	mcount--;
	if (!mcount)
	{
		if (hm_dock)
		{
			FreeLibrary(hm_dock);
			hm_dock=0;
		}
	}
}

DLL_EXPORTS
