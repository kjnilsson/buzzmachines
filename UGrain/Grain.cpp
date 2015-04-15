
#include "Grain.h"
#include "assert.h"

Grain::Grain(CMICallbacks *pCB, CMasterInfo *pMasterInfo)
{
	this->pCB = pCB;
	this->pMasterInfo = pMasterInfo;
	reader.Init(pCB, pMasterInfo);
	delay = 0;
}

Grain::~Grain(void)
{
}

void Grain::Process(sample * psamples, int numsamples)
{
	if(!reader.WaveIsValid())
	{
		Grain::IsActive = false;
		return;
	}
	glCounter += numsamples;
	if(glCounter > grainSettings.length)
	{
		IsActive = false;
		glCounter = 0;
	}
		
	for(int i = 0; i < numsamples; i++)
	{
		if(delay <= 0)
			GetNext();
		//else
		//	memset(&nextSample, 0, sizeof(sample));
		tmp_last_s = nextSample;
		psamples[i].L += nextSample.L;
		psamples[i].R += nextSample.R;
		delay--;
	}
}

void Grain::GetNext()
{
	reader.GetOne(nextSample);
	float e = envelope.GetOne();
	
	//assert(tmp_last_e > 0.2f && e == 0.f);
	tmp_last_e = e;
	nextSample.L *= e;
	nextSample.R *= e;
}
void Grain::Init(GrainSettings gs)
{
	this->grainSettings = gs;
	reader.SetNewWave(gs.wave);
	reader.SetRate(gs.rate);
	glCounter = 0;
	envelope.Init(gs.length);
	nextSample.L = 0.f;
	nextSample.R = 0.f;
}
