#pragma once
//#include "stdafx.h"
#include "MachineInterface.h"
#include "SyncNoiseOsc.h"
#include "Voice.h"
#include "VoiceManager.h"
#include <string>
#include <sstream>
#include <cassert>
using namespace std;

#define MAX_TRACKS 8

CMachineParameter const paraTimbre = 
{ 
	pt_word,										// type
	"Timbre",
	"Timbre",							// description
	0,												// MinValue	
	0xFFFE,												// MaxValue
	0xFFFF,												// NoValue
	MPF_STATE,										// Flags
	1
};
CMachineParameter const paraAttack = 
{ 
	pt_byte,										// type
	"Amp Attack",
	"Attack Amplitude",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	1
};
CMachineParameter const paraHold = 
{ 
	pt_byte,										// type
	"Amp Hold",
	"Hold Amplitude",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	1
};
CMachineParameter const paraDecay = 
{ 
	pt_byte,										// type
	"Amp Decay",
	"Decay Amplitude",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	40
};
CMachineParameter const paraSustain = 
{ 
	pt_byte,										// type
	"Amp Sustain",
	"Sustain Amplitude",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x80
};
CMachineParameter const paraRelease = 
{ 
	pt_byte,										// type
	"Amp Release",
	"Release Amplitude",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x40
};
CMachineParameter const paraFilterFreq = 
{ 
	pt_word,										// type
	"Filter Freq",
	"Filter Frequency",							// description
	0,												// MinValue	
	0xFFFE,											// MaxValue
	0xFFFF,												// NoValue
	MPF_STATE,										// Flags
	0x8000
};
CMachineParameter const paraFilterRez = 
{ 
	pt_byte,										// type
	"Filter Rez",
	"Filter Resonance",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x10
};
CMachineParameter const paraFilterAmt = 
{ 
	pt_byte,										// type
	"Filt Env Amt",
	"Filter Envelope Amount",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x80
};
CMachineParameter const paraFltAttack = 
{ 
	pt_byte,										// type
	"Filter Attack",
	"Attack Filter",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	1
};
CMachineParameter const paraFltHold = 
{ 
	pt_byte,										// type
	"Filter Hold",
	"Hold Filter",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	1
};
CMachineParameter const paraFltDecay = 
{ 
	pt_byte,										// type
	"Filter Decay",
	"Decay Filter",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	40
};
CMachineParameter const paraFltSustain = 
{ 
	pt_byte,										// type
	"Filter Sustain",
	"Sustain Filter",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x80
};
CMachineParameter const paraFltRelease = 
{ 
	pt_byte,										// type
	"Filter Release",
	"Release Filter",							// description
	0,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	0x40
};
CMachineParameter const paraNote = 
{ 
	pt_note,										// type
	"Note",
	"Note",							// description
	NOTE_MIN,												// MinValue	
	NOTE_MAX,											// MaxValue
	NOTE_NO,												// NoValue
	0,										// Flags
	NOTE_NO
};


static CMachineParameter const *pParameters[] = 
{ 
	&paraTimbre,
	&paraAttack,
	&paraHold,
	&paraDecay,
	&paraSustain,
	&paraRelease,
	&paraFilterFreq,
	&paraFilterRez,
	&paraFilterAmt,
	&paraFltAttack,
	&paraFltHold,
	&paraFltDecay,
	&paraFltSustain,
	&paraFltRelease,
	&paraNote


};

#pragma pack(1)


//inline float RateFromBuzzNote(int b)
//{
//	int n = ((int(b / 16) * 12) + (b % 16));
//	n -= 49;
//	return powf(2.0f, ((n) / 12.0f));
//}

class gvals
{
public:
	word timbre;
	byte attack;
	byte hold;
	byte decay;
	byte sustain;
	byte release;
	word filterfreq;
	byte filterrez;
	byte filteramt;
	byte fltattack;
	byte flthold;
	byte fltdecay;
	byte fltsustain;
	byte fltrelease;

};
class tvals
{
public:
	byte note;
};

#pragma pack()
CMachineInfo const MacInfo = 
{
	MT_GENERATOR,							// type
	MI_VERSION,
	MIF_STEREO_EFFECT,						// flags
	1,											// min tracks
	MAX_TRACKS,								// max tracks
	14,										// numGlobalParameters
	1,										// numTrackParameters
	pParameters,
	0, 
	NULL,
	"Rite of Passage",
	"romp",								// short name
	"Karl-Johan Nilsson", 						// author
	NULL
};

class Track
{
public:
	Track()
	{
		has_played_once = false;
	}
	Track(VoiceManager * m)
	{
		manager = m;
		has_played_once = false;
	}
private:
	Voice * current_voice;
	VoiceManager * manager;
	bool has_played_once;
	CMICallbacks * pCB;
public:
	void Init(CMICallbacks * pcb)
	{
		this->pCB = pcb;
		current_voice = manager->GetVoice();
	}
	void SetManager(VoiceManager* m)
	{
		manager = m;
	}
	void SetSeed(int timbre)
	{
		current_voice->SetSeed(timbre);
	}
	void StopNote()
	{
		Voice * _tmp = current_voice;
		current_voice = manager->GetVoice();
		_tmp->InitiateRelease();
	}

	void PlayNote(byte note, float filter_freq)
	{
		Voice * _tmp = current_voice;
		current_voice = manager->GetVoice();
		_tmp->InitiateRelease();
		manager->TriggerNewVoice(note, filter_freq, current_voice);
		has_played_once = true;

#ifdef _DEBUG
		char x[256];
		sprintf(x, "Filter freq: %f", filter_freq * helper::rate_from_buzznote(note));
		pCB->WriteLine(x);
#endif
	}
	void ProcessTick() {}
	void ProcessWork(float * psamples, int numsamples)
	{	}
};
class mi;

class miex : public CMachineInterfaceEx
{
public:
	mi *pmi;

};
class mi : public CMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Init(CMachineDataInput * const pi);
	virtual void Tick();
	virtual bool Work(float *psamples, int numsamples, int const mode);
	//virtual void Save(CMachineDataOutput * const po);
	virtual void OutputModeChanged(bool stereo) {};
	virtual void SetNumTracks(int const n);
	virtual void MidiNote(int const channel, int const value, int const velocity);
	virtual void Stop();
public:
	miex ex;
		
	gvals gval;
	tvals tval[MAX_TRACKS];

	int num_tracks;
	EnvValues evals;
	EnvValues fltevals;

	VoiceManager* voice_manager;
	Track* tracks;
	float filter_freq;

public:
	void PrintTickDebugMessages();
};

DLL_EXPORTS

mi::mi()
{
	ex.pmi = this;
	GlobalVals = &gval;
	TrackVals = &tval;

}

mi::~mi()
{ 
	//delete osc;
	delete [] tracks;
	delete voice_manager;
}

void mi::Init(CMachineDataInput *const pi)
{
	pCB->SetMachineInterfaceEx(&ex);

	voice_manager = new VoiceManager(32, pMasterInfo->SamplesPerSec);
	tracks = new Track[MAX_TRACKS];
	filter_freq = 2000.f;

	pCB->SetNumTracks(pCB->GetThisMachine(), 3);
	num_tracks = 3;

	for(int i = 0; i < MAX_TRACKS; i++)
	{
		tracks[i].SetManager(voice_manager);
		tracks[i].Init(pCB);
	}
}
void mi::SetNumTracks(int const n)
{
	if(n < num_tracks) 
	{
		//stop runnning tracks
		//tracks[n-1].Init();
	}
	num_tracks = n;
}

void mi::PrintTickDebugMessages()
{
	std::ostringstream stm;
	std::string s("Active Voice Count: ");
	stm << voice_manager->ActiveVoiceCount();
	s.append(stm.str());
	pCB->WriteLine(s.c_str());

}
void mi::Stop()
{
	voice_manager->StopAllVoices();
}
void mi::Tick()
{
#if _DEBUG
	PrintTickDebugMessages();
#endif

	if(gval.timbre != paraTimbre.NoValue)
	{
		voice_manager->SetSeed(gval.timbre);
		
		//for(int i = 0; i < num_tracks; i++)
		//	tracks[i].SetSeed(gval.timbre);
	}

	if(gval.attack != paraAttack.NoValue)
	{
		evals.a =  gval.attack * 5;
		voice_manager->UpdateEnvelope(evals);
	}
	if(gval.hold != paraHold.NoValue)
	{
		evals.h =  gval.hold * 5;
		voice_manager->UpdateEnvelope(evals);
	}
	if(gval.decay != paraDecay.NoValue)
	{
		evals.d =  gval.decay * 5;
		voice_manager->UpdateEnvelope(evals);
	}
	if(gval.sustain != paraSustain.NoValue)
	{
		evals.s =  (float)gval.sustain / paraSustain.MaxValue;
		voice_manager->UpdateEnvelope(evals);
	}
	if(gval.release != paraRelease.NoValue)
	{
		evals.r =  gval.release * 10;
		voice_manager->UpdateEnvelope(evals);
	}
	if(gval.filterrez != paraFilterRez.NoValue)
	{
		voice_manager->SetFilterRez(1.f - (float)gval.filterrez / paraFilterRez.MaxValue);
	}
	if(gval.filterfreq != paraFilterFreq.NoValue)
	{
		float _f = ((float)gval.filterfreq / paraFilterFreq.MaxValue);
		_f *= _f;
		filter_freq = _f * 22050.f;
	}
	if(gval.filteramt != paraFilterAmt.NoValue)
	{
		voice_manager->SetFilterAmount((float)gval.filteramt / paraFilterAmt.MaxValue);
	}

	if(gval.fltattack != paraFltAttack.NoValue)
	{
		fltevals.a =  gval.fltattack * 5;
		voice_manager->UpdateFilterEnvelope(fltevals);
	}
	if(gval.flthold != paraFltHold.NoValue)
	{
		fltevals.h =  gval.flthold * 5;
		voice_manager->UpdateFilterEnvelope(fltevals);
	}
	if(gval.fltdecay != paraFltDecay.NoValue)
	{
		fltevals.d =  gval.fltdecay * 5;
		voice_manager->UpdateFilterEnvelope(fltevals);
	}
	if(gval.fltsustain != paraFltSustain.NoValue)
	{
		fltevals.s =  (float)gval.fltsustain / paraFltSustain.MaxValue;
		voice_manager->UpdateFilterEnvelope(fltevals);
	}
	if(gval.fltrelease != paraFltRelease.NoValue)
	{
		fltevals.r =  gval.fltrelease * 10;
		voice_manager->UpdateFilterEnvelope(fltevals);
	}

	//Track values
	for(int i = 0; i < num_tracks; i++)
	{
		if(tval[i].note != paraNote.NoValue)
		{
			if(tval[i].note == NOTE_OFF)
				tracks[i].StopNote();
			else tracks[i].PlayNote(tval[i].note, filter_freq);
		}
	}
}

bool mi::Work(float *psamples, int numsamples, const int mode)
{
	int ns = numsamples*2;
	for(int i =0; i < ns; i++)
		psamples[i] = 0.f;

	voice_manager->ProcessActiveVoicesWorkStereo(psamples, numsamples);

	
	for(int i =0; i < ns; i++)
		if(fabs(psamples[i]) > 0.f) return true;
	
	return false;
}


void mi::MidiNote(int const channel, int const value, int const velocity) 
{
	Voice * v;
	if(velocity > 0) 
	{
		v = voice_manager->GetVoice();
		v->SetMidiNote(value);
		voice_manager->TriggerNewVoice(helper::midinote_to_buzz(value), filter_freq, v);
	}
	if(velocity == 0)
	{
		voice_manager->InitiateReleaseForMidiNote(value);
	}
}
