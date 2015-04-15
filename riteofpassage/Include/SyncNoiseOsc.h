#include "oscillator.h"
#include "helper.h"
#pragma once

using namespace std;
class SyncNoiseOsc : public Oscillator
{
public:
	SyncNoiseOsc(void);
	~SyncNoiseOsc(void);

	void Init(int sample_rate);
	float GetNext(void);
	void SetFrequency(float frequency);
	void SetSeed(int new_seed);
	int getSeed();
private:
	int rand_seed;

	int target_samplerate; 
	float phase;
	float phase_incr;
	float seed;
	float period;
    int last_int_phase;
	float current_rf;
	float next_rf;

	inline float RandFloat()
	{
	    rand_seed *= 16807;
	    return (float)rand_seed * 4.6566129e-010f;
	};

	
};

