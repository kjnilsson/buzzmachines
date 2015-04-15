#include "WaveTableReader.h"

inline int float2int(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}

inline float hermite4(float frac_pos, float xm1, float x0, float x1, float x2)
{
   const float    c     = (x1 - xm1) * 0.5f;
   const float    v     = x0 - x1;
   const float    w     = c + v;
   const float    a     = w + v + (x2 - x0) * 0.5f;
   const float    b_neg = w + a;

   return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}

WaveTableReader::WaveTableReader(void)
		: waveNSamples(0)
{
	spsFactor = 1.f;
	rate = 1.f;
	pos = 0.f;
	playing = false;
	pwl = NULL;
	waveNSamples = 0;
}
WaveTableReader::~WaveTableReader(void){}

void WaveTableReader::Init(CMICallbacks *pCB, CMasterInfo *pMasterInfo)
{
	this->pCB = pCB;
	this->pMasterInfo = pMasterInfo;	
}

bool WaveTableReader::PlayWave(int waveNumber, float rate)
{
	if(!SetNewWave(waveNumber)) return false;
	//if(pwl->numSamples < 1) return false;
	
	this->rate = rate;
	return true;
}

bool WaveTableReader::SetNewWave(int waveNumber)
{
	pwl = pCB->GetWaveLevel(waveNumber, 0);
	if(!pwl) return false;
	isStereo = pCB->GetWave(waveNumber)->Flags & WF_STEREO;
	spsFactor = (float)pwl->SamplesPerSec / pMasterInfo->SamplesPerSec;
	this->waveNSamples = pwl->numSamples;
	playing = true;
	pos = 0.f;
	return true;

}
inline void WaveTableReader::GetHermite(sample &s)
{
	int x;
	float frac;
	short *p = pwl->pSamples;

	frac = ((pos) - (float)float2int(pos));
	if (isStereo)
	{
		x =	float2int(pos)<<1;//pos is in frames not samples
		
		s.L = hermite4(frac, p[x-2], p[x], p[x+2], p[x+4]);
		x++;
		s.R = hermite4(frac, p[x-2], p[x], p[x+2], p[x+4]);
	}
	else
	{
		//frac = ((pos) - (float)float2int(pos));
		x =	float2int(pos);
		s.L = hermite4(frac, p[x-1], p[x], p[x+1], p[x+2]);
		s.R = s.L;
	}
}
inline void WaveTableReader::GetLinear(sample &s)
{
	int x, x2;
	float frac;

	if (isStereo)
	{
		x =	float2int(pos)<<1;
		frac = ((pos) - (float)float2int(pos));
		x2 = x+2;
		s.L = pwl->pSamples[x] + frac*(pwl->pSamples[x2] - pwl->pSamples[x]);
		x++;

		s.R = pwl->pSamples[x] + frac*(pwl->pSamples[x2] - pwl->pSamples[x]);

	}
	else
	{
		x =	float2int(pos);
		x2 = x + 1;
		if(x2 >= pwl->numSamples) x2 = 0;

		s.L = pwl->pSamples[x] + ((pos) - (float)x)*(pwl->pSamples[x2] - pwl->pSamples[x]);
		s.R = s.L;
	}
}
inline void WaveTableReader::GetOne(sample &s)
{
	if(!playing) 
	{
		s.L = s.R = 0.f;
		return;
	}
	GetHermite(s);
	pos += spsFactor * rate;
	if(pos > pwl->numSamples)
	{
		pos = 0.f;
		playing = false;
	}
};

bool WaveTableReader::ProcessBuffer(sample *psamples, int numsamples)
{
	if(!playing) return false;
	sample s;
	for(int i = 0; i < numsamples; i++)
	{
		GetOne(s);
		psamples[i].L += s.L;
		psamples[i].R += s.R;
	}
	
	return true;
}

bool WaveTableReader::WaveIsValid(void)
{
	if(pwl == NULL || pwl->numSamples == 0) return false;
	return pwl->numSamples == waveNSamples;
}

void WaveTableReader::SetRate(float r)
{
	rate = r;
}
