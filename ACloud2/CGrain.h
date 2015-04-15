//#ifndef __CGRAIN_H__
//#define __CGRAIN_H__

#include "AHD4.h"

#include "../mdk/mdk.h"

//#include "themi.h"


class mi;
//class gEnv;

class CGrain
{
public:
	CGrain();
	~CGrain();
	
private:
	bool IsActive;
	bool Reverse;
	bool IsStereo;

	unsigned int Duration; //in  frames
	unsigned int numFrames;
	unsigned int Offset; //0.0f - 1.0f
	float Rate;
	float Amp;
	unsigned int CountDown;
	int lastNumSamples;
	
	const CWaveLevel	*	pwv;
	int * pSPS;
	float spsMix;
	float Pos;
	float outL, outR;

	
	
	int EnvType;
	ahd4 ahd;

	float GlissFactor;

	float PanPos; 
	float RAmp, LAmp;
	
	unsigned int EnvLen, GCount;

	void GetNextFrame();
	void GetNextFrameLinear();

	
public:	
	void SetGrain(	const CWaveLevel *pwl, //pointer to wavelevel structure - pCB->GetWaveLevel(wave,0)
					int countdown, //in frames, will activate grain but not start playing until count has finished
					int duration, //in frames
					float offset, //0.0f - 1.0f
					bool stereo,//stero flag (pCB->GetWave(w)->Flags & WF_STEREO)
					bool reverse, //if true it will play the grain backwards
					float rate, //0.0f-8.0f
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
				);
		
	inline void GenerateAdd(float * psamples, int numsamples);
	inline void Generate(float * psamples, int numsamples);

	void SetSPSPointer(int * sps);

};


