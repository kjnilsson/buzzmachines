#include "SynthFilter.h"
#include <cmath>


SynthFilter::SynthFilter(void)
{
	// reset filter coeffs
	b0a0=b1a0=b2a0=a1a0=a2a0=0.0;
	// reset in/out history
	ou1=ou2=in1=in2=0.0f;
}


SynthFilter::~SynthFilter(void)
{
}

void SynthFilter::Init(int samplerate)
{
	sample_rate = samplerate;
	b0a0=b1a0=b2a0=a1a0=a2a0=0.0;
	ou1=ou2=in1=in2=0.0f;
};

void SynthFilter::Process(float& in0)
{
	// filter
	float const yn = b0a0*in0 + b1a0*in1 + b2a0*in2 - a1a0*ou1 - a2a0*ou2;
	// push in/out buffers
	in2=in1;
	in1=in0;
	ou2=ou1;
	ou1=yn;

	in0 = yn;
}


void SynthFilter::SetFrequency(float c)
{
	if(c > 0.45) c = 0.45f;
	frequency = c;
	CalcCoefs();
}


void SynthFilter::SetResonance(float c)
{
	resonance = c;
	CalcCoefs();
}


void SynthFilter::CalcCoefs()
{
	// temp pi
	double const temp_pi=3.1415926535897932384626433832795;
	// temp coef vars
	double alpha,a0,a1,a2,b0,b1,b2;

	double const omega = 2.0*temp_pi*frequency;
	double const tsin = sin(omega);
	double const tcos = cos(omega);
	//if(q_is_bandwidth)
		alpha=tsin*sinh(log(2.0)/2.0*resonance*omega/tsin);
	//else
	//alpha=tsin/(2.0*resonance);


	b0=(1.0-tcos)*0.5;
	b1=1.0-tcos;
	b2=(1.0-tcos)*0.5;
	a0=1.0+alpha;
	a1=-2.0*tcos;
	a2=1.0-alpha;

	// set filter coeffs
	double _a0 = 1.0 / a0;
	b0a0=(b0*_a0);
	b1a0=(b1*_a0);
	b2a0=(b2*_a0);
	a1a0=(a1*_a0);
	a2a0=(a2*_a0);
}
