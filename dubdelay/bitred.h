#include "math.h"

class bitreducer
{
public:
	bitreducer()
	{
		step = 65535.0f/powf(32,2);
		step2 = 1.0f/step;
		bit = 32.0f;
		div = 1.0f/65535.0f;
		m=0;
		mf=0.0f;
		count = 0.0f;
		rate = 1.0f;
		last = lastL = lastR = 0.0f;
	}
private:
	float step,step2;
	float bit, div;
	long int m;
	float mf, count, rate, last, lastL, lastR;

	
public:
	
	void Setbits(float b, float r)
	{
		bit = b;
		step = 65535.0f/powf(b,2.0f);
		step2 = 1.0f/step;
		m=1<<((int)b-1);
		mf=(float)m;
		rate = r;
	}

	inline void Process(float &in)
	{
		if(bit < 32.0f){
			in = step * floorf(in/step);
		}
	}

	inline void Process2(float &in)
	{
		count += rate;
		if(count >= 1.0f){
			count -= 1.0f;
			if(bit < 32.0f){
				in *=div;
				 last=(long int)(in*m)/(float)m;
				 last *=65535.0f;
			}else{last=in;}
		}
		in = last;
	}

	inline void ProcessStereo(float &inL, float &inR)
	{
		count += rate;
		if(count >= 1.0f){
			count -= 1.0f;
			if(bit < 32.0f){
				inL *= div;
				inR *= div;
				lastL = (long int)(inL*m)/mf;//precalc div?
				lastR = (long int)(inR*m)/mf;
				lastL *= 65535.0f;
				lastR *= 65535.0f;
			}else{lastL=inL;lastR=inR;}
		}
		inL = lastL;
		inR = lastR;
	}

};