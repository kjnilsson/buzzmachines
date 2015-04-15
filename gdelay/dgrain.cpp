#include "dgrain.h"
#include "math.h"

DGrain::DGrain()
{
	buffer = new float[88200];
	buffersize = 88200;
	Status = STAT_INACTIVE;
	probability = 0.75f;
	
}

DGrain::~DGrain()
{
	delete [] buffer;
}
void DGrain::Init(int bsize)
{
	delete [] buffer;
	buffer = new float[bsize];
	grainlen_counter = 0;
	delaylen_counter = 0;
	write_index = read_index = 0;
	hasread = false;
	amp = 0.75;
	probability = 0.8f;
	buffersize = bsize;
	Status = STAT_INACTIVE;
	lamp = ramp = 1.0f;
	write_offset = 0;
	driftvalue = 0;
	driftvalue_orig = 0;


	for(int i = 0; i< 128; i++)
	{
		envtable[i] = GaussEnvTbl(i, 128);
	}
	envtable[128] = 0.f;
}


void DGrain::Reset()
{
	for(int i = 0; i < buffersize; i++) {buffer[i] = 0.f;}
	grainlen_counter = 0;
	delaylen_counter = 0;
	write_index = read_index = 0;
	hasread = false;
	write_offset = 0;
}
void DGrain::Activate(int glen, int dlen, float a, float dec, float spread, int offset, int drift)
{
		Status = STAT_READ;
		grainlen_counter = 0;
		delaylen_counter = 0;
		grainlen = glen;
		delaylen = dlen;
		Reset();
		write_index = read_index = 0;
		amp = dec * 0.9f;
		probability = dec;
		hasread = false;
		write_offset = offset;
		panspread = spread;
		lamp = ramp = 1.0f;
		SetPan(frand());
		driftvalue_orig = drift * frand();//((float)gval.drift / (float)paraDrift.MaxValue);// - 0.5f; 0-1f
		driftvalue_orig -= (drift/2);
		driftvalue = 0;
		
}

bool DGrain::IsActive()
{
	return (bool)Status;
}

void DGrain::Read(float *psamples, int numsamples, const int mode)
{
	if(Status != STAT_READ) return;//not reading
	int gl = grainlen*2;
	
	if(mode & WM_READ)
	{
		for(int i = write_offset; i < numsamples && write_index < gl; i++)
		{
			int p = (int)(write_index * 0.5f);
			float f = GetEnvTbl(p);
			buffer[write_index] = psamples[i*2] * lamp * f;
			buffer[write_index+1] = psamples[i*2+1] * ramp * f;
			write_index+=2;//increment write index for grain buffer  
		}
		hasread = true;//if it gets here it must have written something to buffer
	}
	else
	{
		write_index += numsamples * 2;//stilll need to incr the buffer
	}
	
	write_offset = 0;

	if(write_index >= gl) 
	{
		Status = STAT_WAIT;//if finished reading set to wait to play
		if(!hasread) Status = STAT_INACTIVE;//recycle if nothing was read
	}
}


void DGrain::GenerateAdd(float *psamples, int numsamples)
{
	int offset = 0;
	delaylen_counter += numsamples;//add frames to counter

	if(Status <= STAT_READ) return; //return if the grain is still reading input or is inactive

	if(Status == STAT_WAIT)
	{
		if(delaylen_counter < delaylen) return; //if it is not time to make noise yet
		// NEW GRAIN TIME
		SetPan(frand());
		offset = delaylen_counter - delaylen;//to get the offset into the work call
		delaylen_counter = driftvalue - offset;//-offset;
		driftvalue += driftvalue_orig;
		if(driftvalue+delaylen < grainlen) driftvalue = grainlen+1;
		read_index = 0;//reset read index
	}

	//we should or are already playing some stuff
	Status = STAT_PLAY;

	grainlen_counter += numsamples;//counter for the grain length in frames
	if(grainlen_counter > grainlen)//if true this is the last work call of the grain play time 
	{
		Status = STAT_WAIT;//finished playing grain start waiting for the next one. Grain shoul dhave at least 256 extrac 0 samples in buffer to cover any overruns.
		grainlen_counter = 0;
		if(frand() > probability) Status = STAT_INACTIVE; //deactivate grain
	}

	for(int i = offset; i < numsamples;i++)
	{
		buffer[read_index] *= amp;
		buffer[read_index+1] *= amp;
		psamples[i*2] += buffer[read_index] * lamp;
		psamples[i*2+1] += buffer[read_index+1] * ramp;
		read_index += 2;//increment read index for grain buffer  TODO: need to check that it doesnt read past end of buffer.
	}
}

inline float sqrf(float x) {return x*x;}

float DGrain::GaussEnv(int pos)
{

	float fEnvParam = 0.23f;
	return expf( -sqrf( ((float)pos / (float)grainlen - 0.5f)/fEnvParam ) );
}

float DGrain::GaussEnvTbl(int pos, int len)
{
	float fEnvParam = 0.23f;
	return expf( -sqrf( ((float)pos / (float)len - 0.5f)/fEnvParam ) );
}


void DGrain::UpdateDecay(float r)
{
	if(r <= 1.0f)
	{
		probability = r;
		return;
	}
	return;
}

float DGrain::GetEnvTbl(int pos)
{
	int x, x2;
	float frac = ((float)pos / (float)grainlen) * 128.0f;
	x =	f2i(frac);
	x2 = x + 1;
	//if(x2 >= 128) x2 = 0;

	float f = envtable[x] + ((frac) - (float)x)*(envtable[x2] - envtable[x]);
	return f;
	
}

void DGrain::SetPan(float s)
{
	if(panspread == 0.0f) {ramp = lamp = 1.0f; return;}

	if(s <= 0.5f)
	{
		lamp = ((s*2) * panspread) + (1.0f - panspread);
		ramp = 1.0f;
	}
	else
	{
		ramp = (((s-0.5f)*2) * panspread) + (1.0f - panspread);
		lamp = 1.0f;
	}
}