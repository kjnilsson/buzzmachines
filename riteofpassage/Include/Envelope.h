#include "Voice.h"
#pragma once

//#define unsigned int uint;
struct EnvValues
{
	int a, h, d, r;
	float s;
};
enum EnvelopeState
{
	Attack,
	Hold,
	Decay,
	Sustain,
	Release,
	Off
};

class Envelope
{
public:
	Envelope(void);
	~Envelope(void);
	float GetNext();
	void Init(int samplerate, Voice* v);
	void Reset();

private:
	int sample_rate;
	unsigned int position;
	EnvelopeState state;
	float sustain;
	unsigned int attack;
	unsigned int decay;
	unsigned int hold;
	unsigned int release;
	float last_value;
	float release_beg;
	float e;
	float out;
	Voice* voice;
public:
	// input in milliseconds
	void SetAttack(const int a);
	void SetHold(const int h);
	void SetDecay(const int d);
	// 0-1
	void SetSustain(const float s);
	void SetRelease(const int r);
	void InitiateRelease();
};

