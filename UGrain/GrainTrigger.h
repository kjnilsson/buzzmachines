#pragma once
#include "windows.h"
#include "GrainGenerator.h"


#define GT_ASYNC 0
#define GT_SYNC 1


static int RandomInt(int start, int end)
{
	int r = rand();
	float f = ((float)r) / RAND_MAX;
	return (int)(end-start) * f + start;
}
static float RandomFloat()
{
	float r = (float)rand();
	return r / RAND_MAX;
}

class GrainTrigger
{
public:
	GrainTrigger(GrainGenerator * gen);
	~GrainTrigger(void);
	void Process(int numsamples);
	GrainGenerator * gen;

private:
		int counter;
		int state;
		int grains_per_second;
		int max_wait;
public:
	void SetTriggerType(int state);
	void SetGrainsPerSecond(int gps, int sps);
};
