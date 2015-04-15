#include "SyncNoiseOsc.h"
#include "SynthFilter.h"
#pragma once

class Envelope;

class Voice
{
private:
	bool is_active;
	SynthFilter * filter;
	SyncNoiseOsc *Osc;
public:
	Voice(void);
	~Voice(void);
	void InitiateRelease(void);
	
	Envelope *Env;
	Envelope *FltEnv;
	
	void ProcessWork(float * psamples, int numsamples);
	void Init(int samplerate);
	bool IsActive(void);
	void Activate();
	void SetSeed(int timbre);
	void SetMidiNote(int value);
	int getMidiNote();
	void ProcessFilter(float * in, int num);
	bool CanBeUsed(void);
	bool InUse;
	void Free(void);
	void SetFrequency(float f, float flt_freq);
	int sample_rate;
	float filter_amt;
	void SetFilterRez(float r);
private:
	int midi_note;
	float filter_freq;
	
public:
	int GetOscSeed(void);
	bool random_seed;

};

