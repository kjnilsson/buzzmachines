//#include "gAHDEnv.h"
#include "CGrain.h"
#include "windows.h"
#include "hermite.h"


//void Set(int dur, float offs, int envt, float rate, float pan);
/*
void CGrain::Set(int dur, double offs, int envt, float rate, float pan)
{

	//Duration = dur;
	Offset = offs;
	Rate = rate;
	fEnvLen = 1.0f / dur; // just to ensure that the env len is set to something
	Duration = EnvLen = dur;
	GCount = 0;
	//CDown = 0;
	Pos = 0;
	
	//pan 0-.1.0
	LPan = RPan = 1.0f;
	if (pan < 0.5) RPan = pan*2;
	if (pan > 0.5) LPan = (1.0f - pan)*2;
	
//	Wave = wave;
}

void CGrain::SetWave(int wave, int stereo, const CWaveLevel *pW)//should be void
{
	Wave = wave;
	IsStereo = stereo;
	pwv = pW;
	
	if (!pwv) return;
	
	spsMix = (float)pwv->SamplesPerSec / (float)*pSPS;
//	spsMix = (float)*pSPS/(float)pwv->SamplesPerSec;
	
//	Pos = (float)pwv->numSamples * Offset;
	Pos = float(Offset);

	lastNumSamples = pwv->numSamples;
	
	int iPos = (int)Pos;
	
	if ((Duration+(iPos/Rate)) > pwv->numSamples/Rate){//mono check.
                //Duration -= (Duration*Offset);//
		Duration = pwv->numSamples-(pwv->numSamples-iPos);//Hmmm....
		if (Rate > 1.0) Duration = (int)((pwv->numSamples-iPos)/Rate);
		}
	//if (IsStereo && (Duration+(iPos*0.5f)) > (int)(pwv->numSamples*Rate)*0.5f){
   
	//	Duration = (int)((pwv->numSamples-iPos)*0.5f);
	//	if (Rate > 1.0) Duration = (int)(((pwv->numSamples-iPos)*0.5f)/Rate);
	//	}
		   
	fEnvLen = 1.0f / Duration;
       
}


inline int f2i(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}
*/
void CGrain::SetSPSPointer(int * sps)
{
	pSPS = sps;
}

void CGrain::GetNextFrameLinear()//(const CWaveLevel *pwv, int SPS)
{
//return 0.0f;//	NULL;

	int x, x2;
	float frac;//, s;//?, envelope, samplerate mix
	float e = 1.0f;
	
	if(!IsActive){ //to add zeroes when passing the end of the grain within a work()
		outL = outR = 0.0f;
		return;
	}
//	pwv = pW;
	//set pointer to waveleve structure here...
	//e = gEnv.Envelope2((float)GCount * fEnvLen, 1);//precalculate envelope variable
	e = ahd.ProcEnv();

	if (IsStereo)
	{
		x =	f2i(Pos)<<1;
		x2 = x + 2;
		if(x2 >= pwv->numSamples*2) x2 = 0;

		frac = ((Pos) - (float)f2i(Pos));

		outL = pwv->pSamples[x] + frac*(pwv->pSamples[x2] - pwv->pSamples[x]);
		x++;
		x2++;
		outR = pwv->pSamples[x] + frac*(pwv->pSamples[x2] - pwv->pSamples[x]);

	}
	else //mono - hmm branching - shoudl this be replaced with if (!IsStereo)
	{
		x =	f2i(Pos);
		x2 = x + 1;
		if(x2 >= pwv->numSamples) x2 = 0;

		outL = pwv->pSamples[x] + ((Pos) - (float)x)*(pwv->pSamples[x2] - pwv->pSamples[x]);
		outR = outL;
	}

	//envelope
	outL *= (e * Amp * LAmp);
	outR *= (e * Amp * RAmp);
	
	//move ahead a bit
	Pos += spsMix * Rate;
	GCount++;//increment grain len counter
	//Rate += -0.002f;//glissando (0.0-2.0)  could go here if wanted but the envelope length fEnvLen would have to be update as well
	
	//don't fall off the end of the wave data
	if (GCount > Duration || Pos > pwv->numSamples) 
	{
		Pos = 0;
		IsActive = false;
		GCount = 0;
		//outL=outR=0.0f;//if the work call is not done yet
	}
	//return 1;	 
}

void CGrain::GetNextFrame()//(const CWaveLevel *pwv, int SPS)
{
	float e = 1.0f;
	
	if(!IsActive){ //to add zeroes when passing the end of the grain within a work()
		outL = outR = 0.0f;
		return;
	}

	e = ahd.ProcEnv(); //envelope

	if (IsStereo)
	{
		outL = GetSampleL(Pos, pwv->pSamples, pwv->numSamples);
		outR = GetSampleR(Pos, pwv->pSamples, pwv->numSamples);
		//do filters here
	}
	else //mono - hmm branching - shoudl this be replaced with if (!IsStereo)
	{
		outL = outL = GetSampleM(Pos, pwv->pSamples, pwv->numSamples);
		// do filters here
		outR = outL;
	}

	//envelope, amp and pan
	outL *= (e * LAmp); //LAmp and RAmp includes Pan calculations
	outR *= (e * RAmp);
	
	//move ahead a bit
	Pos += spsMix * Rate;
	//Pos += GlissFactor;//
	GCount++;//increment grain len frame counter
	
	
	//don't fall off the end of the wave data
	if (GCount > Duration || Pos > pwv->numSamples) 
	{
		Pos = 0;
		IsActive = false;
		GCount = 0;
	} 
}
/*
void CGrain::SetAmp(float t, float b, float wa)//top, bottom
{
	float a = (float)rand()/RAND_MAX;
	Amp = ((t-b)*a+b) * wa;//wa is the wave amp...
}

void CGrain::SetEnv(int length, float amt, float skew)
{
	//gEnv.SetEnvParams(amt, skew);
	ahd.SetEnv(length, amt, skew);
}
*/
inline void CGrain::GenerateAdd(float * psamples, int numsamples)
{
	if (!pwv || lastNumSamples != pwv->numSamples){
		IsActive = false;
		return;
	}
	
	outL = outR = 0.0f;
			
	for (int i=0; i<numsamples*2; i+=2){
	
		CountDown--;
		if (CountDown < 0) GetNextFrame();

		psamples[i] += outL;
		psamples[i+1] += outR;
		
	}
}

inline void CGrain::Generate(float * psamples, int numsamples)
{
	if (!pwv || lastNumSamples != pwv->numSamples){
		IsActive = false;
		return;
	}

	outL = outR = 0.0f;
	
	for (int i=0; i<numsamples*2; i+=2){
	
		CountDown--;
		if (CountDown < 0) GetNextFrame();
		
		psamples[i] = outL;
		psamples[i+1] = outR;
		
	}
}

void CGrain::SetGrain(	const CWaveLevel *pwl, //pointer to wavelevel structure - pCB->GetWaveLevel(wave,0)
				int countdown, //in frames, will activate grain but not start playing until count has finished
				int duration, //in frames
				float offset, //0.0f - 1.0f
				bool stereo,//stero flag (pCB->GetWave(w)->Flags & WF_STEREO)
				bool reverse, //if true it will play the grain backwards
				float rate, //0.0f-8.0f or something
				int gliss_cents, // hmm.. could pass the cents to slide in the duration of the grain -2400 - 2400 or something
				float amp, //0.0f-1.0f
				float pan, // 0.0f-1.0f
				int env_type, //0 = none, 1 = AHD, 2=custom
				float env_amt, //0.0f-1.0f
				float env_skew, //0.0f-1.0f
				float env_smooth, //0.0f-1.0f for AHD envelope smooting (essentially a lp filter cutoff value)
				float * cenv, //pointer to custom envelope table
				int cenv_len, //length of custom env table
				int filt_type,//0=none, 1=lp, 2=hp, 3=bp - 12db oct
				float filt_cutoff, //0.0f-1.0f
				float filt_resonance //0.0f-1.0f
			)
{
	if(!pwl) return;
	pwv = pwl;
	
	if(stereo) numFrames = pwv->numSamples/2;
	else numFrames = pwv->numSamples;
	
	CountDown = countdown;
	Duration = duration;
	Offset = (unsigned int)(numFrames * offset);
	
	if(Offset + Duration > numFrames)
		Duration = numFrames - Offset; //ensure that duration does not go into nowwheresland
	
	
	IsStereo = stereo;
	Reverse = reverse;
	Rate = rate;
	//TODO: gliss
	//amp and pan
	LAmp = RAmp = 1.0f;
	if (pan < 0.5f) RAmp = pan * 2.0f;
	if (pan > 0.5f) LAmp = (1.0f - pan) * 2.0f;
	LAmp *= amp;
	RAmp *= amp;
	//end amp and pan
	
	//TODO: envelope stuff
	
	//counters and stuff
	Pos = (float)Offset;
	
	spsMix = (float)pwv->SamplesPerSec / (float)*pSPS;
	GCount = 0; //main grain len counter
	lastNumSamples = pwv->numSamples; //to check in case wave changes during grain.
	
}