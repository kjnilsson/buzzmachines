//#include "StdAfx.h"
#include "VoiceManager.h"
#include <cstdlib>


VoiceManager::VoiceManager(void)
{
	next_voice = 0;
}


VoiceManager::~VoiceManager(void)
{
	delete [] voices;
}


VoiceManager::VoiceManager(int num, int samplerate)
{
	voices = new Voice[num];
	for(int i = 0; i < num; i++)
		voices[i].Init(samplerate);
	num_voices = num;
	this->next_voice = 0;
}

void VoiceManager::SetFilterRez(float r)
{
	for(int i = 0; i < num_voices; i++)
		voices[i].SetFilterRez(r);
}
void VoiceManager::UpdateEnvelope(EnvValues e)
{
	for(int i=0; i < num_voices; i++)
	{
		voices[i].Env->SetAttack(e.a);
		voices[i].Env->SetHold(e.h);
		voices[i].Env->SetDecay(e.d);
		voices[i].Env->SetSustain(e.s);
		voices[i].Env->SetRelease(e.r);
	}
}
void VoiceManager::UpdateFilterEnvelope(EnvValues e)
{
	for(int i=0; i < num_voices; i++)
	{
		voices[i].FltEnv->SetAttack(e.a);
		voices[i].FltEnv->SetHold(e.h);
		voices[i].FltEnv->SetDecay(e.d);
		voices[i].FltEnv->SetSustain(e.s);
		voices[i].FltEnv->SetRelease(e.r);
	}
}

void VoiceManager::TriggerNewVoice(int note, float filterfreq, Voice* voice)
{
		if(voice->random_seed) voice->SetSeed(rand());
		float fltfreq = GetFltFreq(note, filterfreq, voice);
		voice->SetFrequency(helper::buzznote_to_freq(note), fltfreq);
		voice->Activate();
}
void VoiceManager::ProcessActiveVoicesWorkStereo(float* psamples, int numsamples)
{
	for(int i=0; i < num_voices; i++)
	{
		if(voices[i].IsActive())
			voices[i].ProcessWork(psamples, numsamples);
	}
}

Voice* VoiceManager::GetVoice()
{
	for(int i=0; i < num_voices; i++)
	{
		if(voices[i].CanBeUsed())
		{
			voices[i].InUse = true;
			return &voices[i];
		}
	}
	next_voice++;
	if(next_voice >= num_voices) next_voice = 0;
	voices[next_voice].InUse = true;
	return &voices[next_voice];
}

void VoiceManager::StopAllVoices()
{
	for(int i = 0; i < num_voices; i++)
		voices[i].InitiateRelease();
}

void VoiceManager::SetFilterAmount(float amt)
{
	for(int i = 0; i < num_voices; i++)
		voices[i].filter_amt = amt;
}

int VoiceManager::ActiveVoiceCount()
{
	int count = 0;
	for(int i = 0; i < num_voices; i++)
		if(voices[i].IsActive()) count++;
	return count;
}

void VoiceManager::InitiateReleaseForMidiNote(int value)
{
	for(int i = 0; i < num_voices; i++)
		if(voices[i].getMidiNote() == value) voices[i].InitiateRelease();
}

void VoiceManager::SetSeed(int s)
{
	for(int i = 0; i < num_voices; i++)
	{
		if(s==0) voices[i].random_seed = true;
		else voices[i].random_seed = false;
		voices[i].SetSeed(s);
		
	}
}
