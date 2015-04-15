//#include "StdAfx.h"
#include <stdio.h>
#include "Envelope.h"


Envelope::Envelope(void)
{
	sustain = 0.5f;
	attack = 1;
	decay = 500;
	hold = 100;
	release = 500;
	position = 0;
	last_value = e = 0.f;
	sample_rate = 44100;
	release_beg = 1.f;
	state = Off;
	out = 0.f;
	Reset();
}

Envelope::~Envelope(void)
{
}

void Envelope::Init(int samplerate, Voice * v)
{
	voice = v;
	//rcall = pCall;
	sample_rate = samplerate;
}

void Envelope::Reset()
{
	state = attack == 0 ? Hold : Attack;
	position = 0;
	out = 0.f;
}

//void (Envelope::*rcall)() = 0;
// input in milliseconds
float Envelope::GetNext()
{
	//float out = 0.f;
	switch (state)
	{
		case Attack:
		{
			if(position == 0) e = 1.f / attack;
			if(position >= attack) 
			{
				state = Hold; position = 0;
				e = 0.f;
			}
		} break;
		case Hold:
		{
			if(position >= hold) 
			{
				state = Decay; position = 0;
				e = -(1.f / decay) * (1.f - sustain);
			}
			
		} break;
		case Decay:
		{
			if(position >= decay) 
			{state = Sustain; position = 0; e = 0.f;}
		} break;
		case Sustain:
		{
			//e = 0.f;
		} break;
		case Release:
		{
			if(position == 0)
				 e = -(release_beg * 1.f / release);
			if(position >= release)
			{
				if(voice != NULL)
					voice->Free();
				state = Off;
				e = out = 0.f;
				position = 0;
			} 
		} break;
		default:
			return 0.f;
	}
	out += e;
	position++;
	last_value = out;
	return out;

}
void Envelope::SetAttack(const int a)
{
	attack = a * (sample_rate/1000);
	if(attack == 0) attack = 1;
	if(state == Attack)
		e = 1.f / attack;
}
void Envelope::SetHold(const int h)
{
	hold = h * (sample_rate/1000);
}
void Envelope::SetDecay(const int d)
{
	decay = d * (sample_rate/1000);
	if(decay==0) decay = 1;
}
// 0-1
void Envelope::SetSustain(const float s)
{
	sustain = s;
}
void Envelope::SetRelease(const int r)
{
	
	release = r * (sample_rate/1000);
	if(release==0) release = 1;	
	if(state == Release)
		e = -(release_beg * 1.f / release);
}

void Envelope::InitiateRelease()
{
	release_beg = last_value;
	state = Release;
	position = 0;
}
