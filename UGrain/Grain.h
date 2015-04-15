#pragma once
#include "..\..\buzzfiles\MachineInterface.h"
#include "GrainBase.h"
#include "WaveTableReader.h"
#include "GrainEnvelope.h"


struct GrainSettings
{
	GrainSettings(void)
	{
		length = 1000;
		wave = 1;
		rate = amp = 1.f;
	}
	int length;
	int wave;
	float rate;
	float amp;

};
class Grain : public GrainBase
{
public:
	Grain(void);
	~Grain(void);
	Grain(CMICallbacks *pCB, CMasterInfo *pMasterInfo);

	void Process(sample * psamples, int numsamples);
	void Init(GrainSettings gs);
	virtual void GetNext();
	int delay;
	
private:
	WaveTableReader reader;
	CMICallbacks *pCB;
	CMasterInfo *pMasterInfo;
	GrainSettings grainSettings;
	GrainEnvelope envelope;
	float tmp_last_e;
	sample tmp_last_s;
};
