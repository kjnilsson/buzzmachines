//#include "StdAfx.h"
#include "SyncNoiseOsc.h"
#include <cassert>

SyncNoiseOsc::SyncNoiseOsc(void)
{
	phase = 0;
	phase_incr = 1.0f;
	seed = 1;
	target_samplerate = 44100;
	SetFrequency(440.f);
	last_int_phase = 0;
	current_rf = next_rf = 0.f;
}


SyncNoiseOsc::~SyncNoiseOsc(void)
{
}

float SyncNoiseOsc::GetNext()
{
	float result = 0.f;
	phase += phase_incr;

	if(phase >= last_int_phase + 1) //new next randfloat
	{
		current_rf = next_rf;
		next_rf = RandFloat();
	}
	using namespace helper;
	result = lerpf(current_rf, next_rf, phase - (int)phase);
	last_int_phase = (int)phase;

	if(phase > period) 
	{
		rand_seed = seed;
		next_rf = RandFloat();
		phase =  phase - period;
#ifdef _DEBUG
	assert(phase >= 0);
#endif
		last_int_phase = 0;
	}
#ifdef _DEBUG
	assert(result <= 1.0f);
#endif
	return result;
}

int SyncNoiseOsc::getSeed()
{
	return seed;
}

void SyncNoiseOsc::Init(int sample_rate)
{
	phase = 0;
	if(target_samplerate > sample_rate)
		phase_incr = 1.f;
	else
		phase_incr = (float)target_samplerate / sample_rate;
}
void SyncNoiseOsc::SetFrequency(float frequency)
{
	period = target_samplerate / frequency;

}


void SyncNoiseOsc::SetSeed(int new_seed)
{
	seed = new_seed;
}
