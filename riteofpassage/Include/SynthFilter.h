#pragma once

#define fpi2 3.141592f

class SynthFilter
{
public:
	SynthFilter(void);
	~SynthFilter(void);
	void Process(float& x);
public:

	void Init(int samplerate);
protected:
	double frequency, resonance;
	int sample_rate;
// filter coeffs
	double b0a0,b1a0,b2a0,a1a0,a2a0;
	// in/out history
	float ou1,ou2,in1,in2;
public:
	void SetFrequency(float c);
	void SetResonance(float c);
	void CalcCoefs();
};

