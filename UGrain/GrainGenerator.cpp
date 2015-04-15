#include "GrainGenerator.h"



GrainGenerator::GrainGenerator(CMICallbacks *pCB, CMasterInfo *pMasterInfo)
{
	this->pCB = pCB;
	this->pMasterInfo = pMasterInfo;
	maxGrains = 256;
	for(int i = 0; i < maxGrains; i++)
	{
		Grain *g = new Grain(pCB, pMasterInfo);
		grains.push_back(g);
	}
	playing = false;
}

GrainGenerator::~GrainGenerator(void)
{
	for(int i = 0; i < maxGrains; i++)
	{
		delete grains.at(i);
	}
}

Grain * GrainGenerator::FindNextGrain()
{
	//GrainVector::iterator it = grains.begin();
	//for(it; it != grains.end(); it++)
	//	if(!((*it)->IsActive)) return (*it);
	//return NULL;

	int size = grains.size();
	for(int i =0;i<size;i++)
	{
		if(!grains[i]->IsActive) return grains[i];
	}
	return NULL;
}

void GrainGenerator::ActivateGrain(int delay)
{
	Grain* it = FindNextGrain();
	if(it == NULL) return;
	it->Init(gs);
	it->IsActive = true;
	it->delay = delay;
}
void GrainGenerator::Generate(sample * psamples, int numsamples)
{
	if(!playing) return;
	GrainVector::iterator it = grains.begin();
	for(it; it != grains.end(); it++)
	{
		if((*it)->IsActive) (*it)->Process(psamples, numsamples);
	}
}
void GrainGenerator::Play()
{
	playing = true;
}
void GrainGenerator::Stop()
{
	playing = false;
}
