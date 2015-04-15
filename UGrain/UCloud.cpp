#pragma once
//#include "UCloud.h"
#include "..\..\buzzfiles\MachineInterface.h"
#include "GrainTrigger.h"


CMachineParameter const paraGrainSize = 
{ 
	pt_word,										// type
	"Grain Size",
	"Grain Size",							// description
	100,												// MinValue	
	0xFFFE,											// MaxValue
	0xFFFF,												// NoValue
	MPF_STATE,										// Flags
	1000
};
CMachineParameter const paraDensity = 
{ 
	pt_byte,										// type
	"Density GPS",
	"Density Grains Per Sec",							// description
	1,												// MinValue	
	0xFE,											// MaxValue
	0xFF,												// NoValue
	MPF_STATE,										// Flags
	10
};
CMachineParameter const paraWave = 
{ 
	pt_byte,										// type
	"Wave",
	"Wave",							// description
	WAVE_MIN,												// MinValue	
	WAVE_MAX,											// MaxValue
	WAVE_NO,												// NoValue
	MPF_WAVE,										// Flags
	WAVE_MIN
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


static CMachineParameter const *pParameters[] = { 
	// track
	&paraGrainSize,
	&paraDensity,
	&paraWave,
	&paraNote,
};

#pragma pack(1)


inline float RateFromBuzzNote(int b)
{
	int n = ((int(b / 16) * 12) + (b % 16));
	n -= 49;
	return powf(2.0f, ((n) / 12.0f));
}
class Cloud
{
public:
	Cloud(void)
	{
		is_active = false;
	}
	~Cloud(void)
	{
		delete trigger;
		delete grain_gen;
	}
public:
	GrainTrigger *trigger;
	GrainGenerator *grain_gen;
	void Play();
	void Stop();
	bool is_active;
};
class gvals
{
public:
	word grain_size;
	byte density;
	byte wave;

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
	MIF_PLAYS_WAVES | MIF_STEREO_EFFECT,						// flags
	1,											// min tracks
	8,								// max tracks
	3,										// numGlobalParameters
	1,										// numTrackParameters
	pParameters,
	0, 
	NULL,
	"UGrain",
	"UGrain",								// short name
	"Karl Nilsson", 						// author
	NULL
};

class mi;

class miex : public CMachineInterfaceEx
{
public:

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
	virtual void Save(CMachineDataOutput * const po);
	virtual void OutputModeChanged(bool stereo) {};
	virtual void SetNumTracks(int const n);


public:
	miex ex;
	
	
	gvals gval;
	tvals tval[8];
	Cloud clouds[8];

	int num_tracks;

	//GrainGenerator *graingen;
	//GrainTrigger *trigger;
};

DLL_EXPORTS

mi::mi()
{
	ex.pmi = this;
	GlobalVals = &gval;
	TrackVals = &tval;

}

mi::~mi()
{}
void mi::Init(CMachineDataInput *const pi)
{
	pCB->SetMachineInterfaceEx(&ex);
	for(int i = 0 ; i < 8; i++)
	{
		clouds[i].grain_gen = new GrainGenerator(pCB, pMasterInfo);
		clouds[i].trigger = new GrainTrigger(clouds[i].grain_gen);
	}
	//graingen = new GrainGenerator(pCB, pMasterInfo);
	//trigger = new GrainTrigger(graingen);

	pCB->SetNumTracks(pCB->GetThisMachine(), 3);
	//trigger->SetTriggerType(1);
	
	
}
void mi::SetNumTracks(int const n)
{
	for(int i=0;i<n;i++)
	{
		clouds[i].is_active = true;
	}
	num_tracks = n;
}
void mi::Tick()
{
	for(int i = 0; i < num_tracks; i++)
	{
		if(gval.grain_size != paraGrainSize.NoValue)
		{
			clouds[i].grain_gen->gs.length = gval.grain_size;
		}
		if(gval.wave != paraWave.NoValue)
		{
			clouds[i].grain_gen->gs.wave = gval.wave;
		}
		if(gval.density != paraDensity.NoValue)
		{
			clouds[i].trigger->SetGrainsPerSecond(gval.density, pMasterInfo->SamplesPerSec);
		}
		if(tval[i].note != paraNote.NoValue && clouds[i].is_active)
		{			
			clouds[i].grain_gen->gs.rate = RateFromBuzzNote(tval[i].note);
			clouds[i].grain_gen->Play();
		}
		else if(tval[i].note == NOTE_OFF)
			clouds[i].grain_gen->Stop();

	}
}

bool mi::Work(float *psamples, int numsamples, const int mode)
{
	memset(psamples, 0, 2 * sizeof(float) * numsamples);
	for(int t = 0; t < num_tracks; t++)
	{
		if(clouds[t].is_active)
		{
			clouds[t].trigger->Process(numsamples);
			clouds[t].grain_gen->Generate((sample*)psamples, numsamples);
		}
	}
	
	for(int i = 0;i<numsamples*2;i++)
		if(psamples[i] != 0.f) return true;
	return false;
}
void mi::Save(CMachineDataOutput *const po)
{
}