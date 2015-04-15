//#include "StdAfx.h"
#include "Voice.h"
#include "Envelope.h"
#include "windows.h"


Voice::Voice(void) : Osc()
	, InUse(false)
	, midi_note(0)
{
	Osc = new SyncNoiseOsc();
	Env = new Envelope();
	FltEnv = new Envelope();
	filter = new SynthFilter();
	is_active = false;
	InUse = false;
	sample_rate = 0;
	midi_note = 0;
	random_seed = false;
	filter_amt = 0.0f;

}


Voice::~Voice(void)
{
	delete Env;
	delete FltEnv;
	delete filter;
	delete Osc;
}

void Voice::InitiateRelease(void)
{
	Env->InitiateRelease();
	FltEnv->InitiateRelease();
	InUse = false;
}

void Voice::SetSeed(int timbre)
{
	if(random_seed)
	{
		Osc->SetSeed(rand());
	}
	else
	{
		Osc->SetSeed(timbre);
	}
}

void Voice::ProcessWork(float * psamples, int numsamples)
{
	int q = 4;
	for(int i = 0; i < numsamples; i++)
	{
		float o = Osc->GetNext();
		float fe = FltEnv->GetNext();
		if(q == 0)
		{
			filter->SetFrequency(filter_freq + ((filter_freq * 4 * filter_amt) * fe));
			q = 4;
		}
		q--;
		ProcessFilter(&o, numsamples);
		float e = Env->GetNext();
		e *= (float)0x8000;
		psamples[i*2] += o * e;
		psamples[(i*2)+1] += o * e;
	}
}
void Voice::Free(void)
{
	is_active = false;
	InUse = false;
	//filter->Init(sample_rate);

}

void Voice::SetFilterRez(float r)
{
	filter->SetResonance(r);
}

void Voice::Init(int samplerate)
{
	sample_rate = samplerate;
	Osc->Init(samplerate);
	Env->Init(samplerate, this);
	FltEnv->Init(samplerate, NULL);
	filter->Init(sample_rate);
	is_active = false;
}

bool Voice::IsActive(void)
{
	return is_active; 
}

void Voice::Activate(void)
{
	Env->Reset();
	FltEnv->Reset();
	filter->Init(sample_rate);
	filter->CalcCoefs();
	is_active = true;
	Osc->Init(sample_rate);
}

void Voice::ProcessFilter(float * in, int num)
{
	filter->Process(*in);
}

void Voice::SetFrequency(float f, float flt_freq)
{
	float _f = 0.f;
	Osc->SetFrequency(f);
	if(f > 22000.f) _f = 22000.f; 
	filter_freq = flt_freq;
	filter->SetFrequency(flt_freq);//TODO key tracking
	//filter->SetResonance(filter->resonance);
}

int Voice::GetOscSeed(void)
{
	return Osc->getSeed();
}

bool Voice::CanBeUsed(void)
{
	return (!InUse && !is_active);
}

void Voice::SetMidiNote(int value)
{
	midi_note = value;
}

int Voice::getMidiNote()
{
	return midi_note;
}
