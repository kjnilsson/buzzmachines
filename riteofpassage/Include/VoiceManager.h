#include "Voice.h"
#include "Envelope.h"
#pragma once

class VoiceManager
{
public:
	VoiceManager(void);
	~VoiceManager(void);
	Voice* GetVoice();
	VoiceManager(int num, int samplerate);

private:
	Voice * voices;
	int num_voices;
	int next_voice;
	
public:
	void ProcessActiveVoicesWorkStereo(float* psamples, int numsamples);
	int ActiveVoiceCount();
	void UpdateEnvelope(EnvValues e);
	void UpdateFilterEnvelope(EnvValues e);
	void StopAllVoices();
	void InitiateReleaseForMidiNote(int value);
	void SetSeed(int s);
	void SetFilterRez(float r);
	void TriggerNewVoice(int note, float filterfreq, Voice* voice);
	void SetFilterAmount(float amt);


	static float GetFltFreq(int &note, float filter_freq, Voice* voice)
	{
		float fltfreq = filter_freq * helper::rate_from_buzznote(note) / (voice->sample_rate);
		if(fltfreq > 0.45f) fltfreq = 0.45f;
		return fltfreq;
	}
};

