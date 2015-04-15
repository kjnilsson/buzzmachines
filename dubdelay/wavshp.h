#include "math.h"

class waveshaper1
{
public:
	waveshaper1()
	{
		k = 0.0f;
		div = 1.0f/65535.0f;
	}
public:
	void set(float a)
	{
		k = 2*a/(1-a);
	}
	void process(float &in)
	{
		in *= div;
		in = (1.f+k)*in/(1.f+k*fabs(in));
		in *= 65353.f;
	}
private:
	float k, div;
};

//x = x-(pow(x,amount)/amount)//;with amount being somewhere between 1 .. 4 and x 0 .. 1.
class waveshaper2
{
public:
	waveshaper2()
	{
		k = 0.0f;
		div = 1.0f/(65535.0f*2);
	}
public:
	void set(float a)
	{
		a = a * 4;
		k = 1.0f / a;
	}
	void process(float &in)
	{
		in *= div;
		in += 0.5f;		
		in = in-(pow(in,a)/a);
		in *= 65353.f;
	}
private:
	float k, div;
};

/*

float saturate(float x, float t)
{
if(fabs(x) return x
else
{
if(x > 0.f);
return t + (1.f-t)*tanh((x-t)/(1-t));
else
return -(t + (1.f-t)*tanh((-x-t)/(1-t)));
}
}
*/