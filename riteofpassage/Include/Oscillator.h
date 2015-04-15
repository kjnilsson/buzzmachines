#include "cmath"
#pragma once
class Oscillator
{
public:
	Oscillator(void);
	~Oscillator(void);
	virtual void Init(int sample_rate) = 0;
	virtual float GetNext(void) = 0;
	virtual void SetFrequency(float frequency) = 0;

	float inline static BuzzNoteToFreq(int buzz_note)
	{
		return 440.f * powf(2.0f,(BuzzNoteToMidiNote(buzz_note)-57) / 12.f);
	}
 
	int inline static BuzzNoteToMidiNote(int buzz_note)
	{
		return (((buzz_note / 16) * 12) + (buzz_note % 16)) - 5;//buzz C-4 = 65 midi C-4 = 60
	}
};

