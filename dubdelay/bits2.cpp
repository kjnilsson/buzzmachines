#include <windows.h>
#include "../mdk/mdk.h"
//#include <float.h>
//#include <stdlib.h>
#include "hermite.h"
#include "bitred.h"
#include "denormalnoise.h"
#include "cbhp.h"
#include "cblp.h"
#include "saturate1.h"
#include "iparameter.h"


////////////////////////////////////////////////////////////////////////
// Parameters
CMachineParameter const paraInput =
{
	pt_byte,			//Type
	"Input",			//Short name
	"Input Volume",//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0xFE				//Default value
};
CMachineParameter const paraDelayLen =
{
	pt_word,			//Type
	"Delay Len",				//Short name
	"Delay Length",		//Long name
	0x0001,				//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x3000	
	//Default value
};
CMachineParameter const paraFeedback =
{
	pt_byte,			//Type
	"Feedback",			//Short name
	"Feedback Amount",//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};
CMachineParameter const paraGlide =
{
	pt_byte,			//Type
	"Glide Amt",			//Short name
	"Glide Amount",//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};
CMachineParameter const paraBits =
{
	pt_byte,			//Type
	"Bit Rate",			//Short name
	"Bit Rate",//Long name
	0x01,				//Min value
	32,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	32				//Default value
};
CMachineParameter const paraSamplerate =
{
	pt_word,			//Type
	"Samplerate",			//Short name
	"Samplerate",//Long name
	0,				//Min value
	44101,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	44101				//Default value
};
CMachineParameter const paraSaturate =
{
	pt_byte,			//Type
	"Saturation",			//Short name
	"Saturation Threshold",//Long name
	10,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0xFE				//Default value
};
CMachineParameter const paraLowpass =
{
	pt_word,			//Type
	"LowPass",			//Short name
	"LowPass Frequency",//Long name
	20,				//Min value
	22050,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	22050				//Default value
};
CMachineParameter const paraLPResonance =
{
	pt_byte,			//Type
	"LP Q",			//Short name
	"LowPass Resonance",//Long name
	1,				//Min value
	100,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	20				//Default value
};
CMachineParameter const paraHighpass =
{
	pt_word,			//Type
	"HighPass",			//Short name
	"HighPass Frequency",//Long name
	20,				//Min value
	22050,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	20				//Default value
};
CMachineParameter const paraHPResonance =
{
	pt_byte,			//Type
	"HP Q",			//Short name
	"HighPass Resonance",//Long name
	1,				//Min value
	100,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	20				//Default value
};
CMachineParameter const paraDelayout =
{
	pt_byte,			//Type
	"Delay Out",			//Short name
	"DelayOut Volume",	//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0xFE				//Default value
};
CMachineParameter const paraDryout =
{
	pt_byte,			//Type
	"Dry Out",			//Short name
	"DryOut Volume",	//Long name
	0x00,				//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x00				//Default value
};

////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraInput,
	&paraDelayLen,
	&paraFeedback,
	&paraGlide,
	&paraBits,
	&paraSamplerate,
	&paraSaturate,
	&paraLowpass,
	&paraLPResonance,
	&paraHighpass,
	&paraHPResonance,
	&paraDelayout,
	&paraDryout
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

enum {
	P_INPUT,
	P_DELAYLEN,
	P_FEEDBACK,
	P_GLIDE,
	P_BITS,
	P_SAMPLERATE,
	P_SATURATE,
	P_LOWPASS,
	P_LPRESONANCE,
	P_HIGHPASS,
	P_HPRESONANCE,
	P_DELAYOUT,
	P_DRYOUT
};	
#pragma pack(1)

class gvals
{
public:
	byte input;
	word delaylen;
	byte feedback;
	byte glideamt;
	byte bits;
	word samplerate;
	byte saturate;
	word lowpass;
	byte lprez;
	word highpass;
	byte hprez;
	byte delayout;
	byte dryout;

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
	13,								//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"Intoxicat Dub Delay",		//Full name
	"Bits",						//Short name
	"Karl Nilsson",			//Author
	"&About...\nPanic!"						//Commands
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
	
	//general vars
	float srfactor;
	bool IsOn;
	double OffCount;
	//delay related vars
	unsigned int writeh;
	double dellen, newdellen;
	float lastreadL, lastreadR;
	bool glidelonger, glideshorter;
	double glidefrac, glidelen;
	float fbamt, fbincr, fbtarget, glideamt, samplerate, bitrate;	
	bool fbglide;
	
	unsigned int buflen;
	IParameter dryout, delout;
	
	float invol, intarget, inincr;
	bool inglide;
	

	float* bufferL;
	float* bufferR;

	//effects
	bitreducer btr;
	//waveshaper1 ws;
	saturate1 str;
	bool saton;
	CookbookHighpass hpL;
	CookbookHighpass hpR;
	CookbookLowpass lpL;
	CookbookLowpass lpR;
	bool lpon, hpon;

	denormalnoise noise;
	

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

////////////////////////////////////////////////////////////////////////

#define MAC_VERSION	1

void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	pCB->SetnumOutputChannels(pCB->GetThisMachine(), 2);
	
	//general
	srfactor = pMasterInfo->SamplesPerSec / 44100.0f;
	IsOn = false;
	OffCount = 0;
	//delay inits
	writeh = 0;
	dellen = newdellen = 122880.0f * srfactor;
	lastreadL = lastreadR = 0.0f;
	glidelonger = glideshorter = false;
	bitrate = 32.0f;
	samplerate = (float)pMasterInfo->SamplesPerSec;
	glidefrac = glidelen = 0.0f;
	fbamt = fbtarget = glideamt = 0.5f;
	fbincr = 0.0f;
	fbglide = false;
	invol = 1.0f;
	inglide = false;
	intarget = 1.0f;
	inincr = 0.0f;
	dryout = 0.0f;
	dryout.Init(0.0f,1,pMasterInfo->SamplesPerTick);
	delout.Init(1.0f,1,pMasterInfo->SamplesPerTick);
	buflen = 0xFFFF * 10;
	bufferL = new float[buflen];
	bufferR = new float[buflen];
	memset(bufferL, 0, buflen * sizeof(float));
	memset(bufferR, 0, buflen * sizeof(float));

	//effects
	btr.Setbits(32.0f, 1.0f);
	str.set(1.0f);
	saton = false;
	hpL.setC0(20/22050.0f);
	hpR.setC0(20/22050.0f);
	lpL.setC0(1.0f);
	lpR.setC0(1.0f);
	hpL.setC1(0.2f);
	hpR.setC1(0.2f);
	lpL.setC1(0.2f);
	lpR.setC1(0.2f);
	lpon = hpon = false;


	//AllocConsole(); freopen("CON", "w", stdout);
	//printf("\nlenc: : %d",pCB->GetPlayPos());
	
}

void mi::MDKSave(CMachineDataOutput * const po)
{
	NULL;
}

////////////////////////////////////////////////////////////////////////

void mi::Tick()
{
	srfactor = pMasterInfo->SamplesPerSec / 44100.0f; //call this every tick as sample rates might change mid session
	//printf("\nlenc: : %.4f",pMasterInfo->TicksPerSec);
	dryout.SetLen(pMasterInfo->SamplesPerTick);
	delout.SetLen(pMasterInfo->SamplesPerTick);

	if(gval.input != paraInput.NoValue){
		inglide = true;
		intarget = (float)gval.input / 0xFE; //should add inertia here
		inincr = (intarget - invol) / pMasterInfo->SamplesPerTick;
	}
	if (gval.delaylen != paraDelayLen.NoValue){
		float f = (float)gval.delaylen/ (float)(paraDelayLen.MaxValue);//scale
		float m = (paraDelayLen.MaxValue * srfactor) * 2.0f;//this should equate almost 3 seconds
		newdellen = (f*f)*m;
		glidelen = dellen * glideamt;//0.0 - 1.0
			if(newdellen < dellen){
				glidefrac = (dellen - newdellen) / glidelen;
				glideshorter = true;
			}else{
				glidefrac = (newdellen - dellen) / glidelen;
				glidelonger = true;
			}
	}
	if(gval.glideamt != paraGlide.NoValue){
		glideamt = (float)gval.glideamt / 0xFE;
	}
	if(gval.feedback != paraFeedback.NoValue){
		fbglide = true;
		fbtarget = (float)gval.feedback / 0xFE;
		fbincr = (fbtarget - fbamt) / pMasterInfo->SamplesPerTick;
	}
	if(gval.bits != paraBits.NoValue){
		bitrate = (float)gval.bits;
		btr.Setbits(bitrate, samplerate/(float)pMasterInfo->SamplesPerSec);
	}
	if(gval.samplerate != paraSamplerate.NoValue){
		if (gval.samplerate == 44101) samplerate = (float)pMasterInfo->SamplesPerSec;
		else samplerate = gval.samplerate;
		btr.Setbits(bitrate, samplerate/(float)pMasterInfo->SamplesPerSec);
	}
	if(gval.saturate != paraSaturate.NoValue){
		if (gval.saturate == 0xFE) saton = false;
		else saton = true;
		str.set(	(float)gval.saturate / 508.0f ); //2 * 0xFE 0- 0.5
	}
	if(gval.lowpass != paraLowpass.NoValue){
		if (gval.lowpass < 22050.0f) lpon = true;
		else lpon = false;
		float f =(float)gval.lowpass/22050.0f;
		lpL.setC0(f*f);
		lpR.setC0(f*f);
	}
	if(gval.lprez != paraLPResonance.NoValue){
		float f = (gval.lprez/100.0f);
		lpR.setC1(f);
		lpL.setC1(f);
	}
	if(gval.highpass != paraHighpass.NoValue){
		if (gval.highpass > 20) hpon = true;
		else hpon = false;
		float f = (float)gval.highpass/22050.0f;
		hpL.setC0(f*f*f*f*f);
		hpR.setC0(f*f*f*f*f);
	}
	if(gval.hprez != paraHPResonance.NoValue){
		float f = (gval.hprez/100.0f);
		hpR.setC1(f);
		hpL.setC1(f);
	}
	if(gval.delayout != paraDelayout.NoValue){
		delout = (float)gval.delayout / 0xFE;
	}
	if(gval.dryout != paraDryout.NoValue){
		dryout = (float)gval.dryout / 0xFE;
	}
}

////////////////////////////////////////////////////////////////////////:

inline float ReadPos(float dl, int wh, int bl)
{
	float t = wh-dl;
	if(t < 0.0f) return bl + t;
	else return t;
}
float waveshape_distort( float in ) {
	in *= 1.6f;
return (1.5f * in - 0.5f * in *in * in) * 0.4f;
}

inline float kill_denormal(float &val)
{
	static const float anti_denormal = 1e-18f;
	val += anti_denormal;
	val -= anti_denormal;
	return val;
}
float const VolumeToDB(float volume)			// by Joachim
{
	float x = -1000.0f;
	if (volume>0.0f)
	{
		x = logf(volume)/logf(2.0f)*6.0f;
	}
	return x;
}

////////////////////////////////////////////////////////////////////////

//The Work() function is called after Input() has been called a bunch of
//times. This is where we output what we collected, and get ready for the
//next pass.
bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{
	if(mode == WM_NOIO) return false;
	
	float inL = 0.0f;
	float inR = 0.0f;
	float outL = 0.0f;
	float outR = 0.0f;
	float madenoise = 0.0f;
	float rp = 0.0f;

	for(int i=0;i< numsamples;i++)
	{
		//inertia
		if(fbglide){
			fbamt += fbincr;
			if (fbincr < 0.0f && fbamt < fbtarget) fbglide = false;
			if (fbincr > 0.0f && fbamt > fbtarget) fbglide = false;
		}
		if(inglide){
			invol += inincr;
			if (inincr < 0.0f && invol < intarget) inglide = false;
			if (inincr > 0.0f && invol > intarget) inglide = false;		
		}
		delout.Work();
		dryout.Work();

		
		bufferL[writeh] = bufferR[writeh] = 0.0f;
		if(mode & WM_READ){
			//if(invol > 0.0f){
				IsOn = true;
				OffCount = 0;
		//	}
			bufferL[writeh] = inL = (psamples[i*2]	   * invol);//should we not set ison if invol == 0.0f
			bufferR[writeh] = inR = (psamples[(i*2)+1] * invol);
		}

		if(IsOn){
			//fb inertie
			bufferL[writeh] += lastreadL;
			bufferR[writeh] += lastreadR;

			lastreadL = lastreadR = 0.0f;
			
			rp = ReadPos((float)dellen, writeh, buflen);
			outL =  GetSampleM(rp, bufferL, buflen) * fbamt;
			outR =  GetSampleM(rp, bufferR, buflen) * fbamt;

			//effects
			btr.ProcessStereo(outL,outR);

			//do waveshaping here
			if(saton){
				outL = str.saturate(outL);
				outR = str.saturate(outR);
			}
			//add denormal noise
			float n = noise.get(i);
			outL += n;
			outR += n;
			//filters
			if(hpon){
				hpL.process(outL);
				hpR.process(outR);
			}
			if(lpon){
				lpL.process(outL);
				lpR.process(outR);
			}
			//remove noise after filter processing
			outL -= n;
			outR -= n;
			//feedback
			lastreadL = kill_denormal(outL);
			lastreadR = kill_denormal(outR);
			//dryout
			psamples[i*2] 		= (outL*delout.Get()) + inL * dryout.Get();
			psamples[(i*2)+1] 	= (outR*delout.Get()) + inR * dryout.Get();

			madenoise += (unsigned)(psamples[i*2] + psamples[(i*2)+1]);//test change from outside of bracjet
		}
		
		

		writeh++;
		if(writeh >= buflen) writeh -= (int)buflen;
		
		if (glidelonger){
			dellen += glidefrac;
			if(dellen > newdellen){
				glidelonger = false;
				dellen = newdellen;
			}
		}
		else if (glideshorter){
			dellen -= glidefrac;
			if(dellen < newdellen){
				glideshorter = false;
				dellen = newdellen;
			}
		}
	}

	if(madenoise == 0.0f){
		OffCount += numsamples;
		if(OffCount > dellen){ 
			if(IsOn){
				memset(bufferL, 0, buflen * sizeof(float));
				memset(bufferR, 0, buflen * sizeof(float));
			}
			IsOn = false;
		}
		return false;
	}
	OffCount = 0;
	return true;
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
		"Intoxicat Dub Delay v1.0\n"
		"Yohey! \n\n"
		);
		pCB->MessageBox(txt);
		break;
	case 1:
		IsOn = false;
		memset(bufferL, 0, buflen * sizeof(float));
		memset(bufferR, 0, buflen * sizeof(float));
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
	case P_INPUT: //input
		sprintf(txt,"%.f%% %.1fdB", value / 2.540f, VolumeToDB(value / 254.0f)); return txt;
		break;
	case P_DELAYLEN:
		f = value/(float)(paraDelayLen.MaxValue);
		m = (paraDelayLen.MaxValue * srfactor) * 2.0f;
		f = (f*f)*m;
		sprintf(txt,"%.3fs %.2f", f/(float)pMasterInfo->SamplesPerSec, (f/(float)pMasterInfo->SamplesPerSec)*pMasterInfo->TicksPerSec); 
		return txt;
		break;
	case P_FEEDBACK:
		sprintf(txt,"%.2f%%", value / 2.54f); return txt;
		break;
	case P_GLIDE:
		sprintf(txt,"%.2f%%", value / 2.54f);
		return txt;
		break;
	case P_BITS:
		if (value == 32) sprintf(txt,"OFF");
		else sprintf(txt,"%d", value); return txt;
		break;
	case P_SAMPLERATE:
		if (value == 44101) sprintf(txt,"OFF");
		else sprintf(txt,"%d", value); return txt;
		break;
	case P_SATURATE:
		if (value == 0xFE) sprintf(txt,"OFF");
		else sprintf(txt,"%.2f", (value / 254.0f) * 32767.0f); return txt;
		break;
	case P_LOWPASS:
		if (value == 22050) sprintf(txt,"OFF");
		else {
			float f =(float)value/22050.0f;
			sprintf(txt,"%.2f Hz", (f*f)*22050.f);
		}
		return txt;
		break;
	case P_LPRESONANCE:
		sprintf(txt,"%.3f", value / 100.0f); return txt;
		break;
	case P_HIGHPASS:
		if (value == 20) sprintf(txt,"OFF");
		else {
			float f =(float)value/22050.0f;
			sprintf(txt,"%.2f Hz", (f*f*f*f*f)*22050.f);
		}
		return txt;
		break;	
	case P_HPRESONANCE:
		sprintf(txt,"%.3f", value / 100.0f); return txt;
		break;
	case P_DELAYOUT:
		sprintf(txt,"%.f%% %.1fdB", value / 2.540f, VolumeToDB(value / 254.0f));
		return txt;
		break;
	case P_DRYOUT:
		sprintf(txt,"%.f%% %.1fdB", value / 2.540f, VolumeToDB(value / 254.0f));
		return txt;
		break;		
	default:
		return NULL;
	}
	return txt;
}
