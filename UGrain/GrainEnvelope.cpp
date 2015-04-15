#include "GrainEnvelope.h"

GrainEnvelope::GrainEnvelope(void)
	: count(0)
{
	length = 0;
	incr = 0.f;
	count = 0.f;
	g1 = new GaussianEnvelope(GE_SIZE+1, GE_SIZE*16, 0.5, 1.1f);
	
}

GrainEnvelope::~GrainEnvelope(void)
{
	delete g1;
}


bool GrainEnvelope::Init(int length)
{
	this->length = length;
	incr = ((float)GE_SIZE) / length;
	count = 0.f;

	return false;
}

//todo optimise lerp
float GrainEnvelope::GetOne(void)
{
	count += incr;
	if(count >= GE_SIZE) return 0.f;
	int i = (int)count;
	float x = g1->data[i];
	float x2 = g1->data[i+1];
	float frac = count - i;
	return x + ((x2-x) * frac);
}
