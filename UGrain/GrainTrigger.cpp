#include "GrainTrigger.h"
//#include "GrainGenerator.h"

GrainTrigger::GrainTrigger(GrainGenerator * gen)
{
	this->gen = gen;
	counter = 0;
	state = GT_ASYNC;
	grains_per_second = 10;
}

GrainTrigger::~GrainTrigger(void)
{
}
void GrainTrigger::SetGrainsPerSecond(int gps, int sps)
{
	grains_per_second = gps;
	max_wait = 2 * (sps/gps);
}
void GrainTrigger::Process(int numsamples)
{
	for(int i = 0; i < numsamples;i++)
	{
		counter--;
		if(counter < 0) 
		{
			gen->ActivateGrain(i);
			gen->gs.amp = 0.5f * RandomFloat() + 0.5f;
			if(state == GT_ASYNC)
			{
				counter = RandomInt(1, max_wait);
			}
			if(state == GT_SYNC)
				counter = max_wait / 2;
		}
		
	}
}


void GrainTrigger::SetTriggerType(int state)
{
	this->state = state;
}
