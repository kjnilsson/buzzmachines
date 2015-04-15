#pragma once
#include "GaussianEnvelope.h"
#include <cmath>

GaussianEnvelope::GaussianEnvelope(int size, int width, float offset, float maxval)
{
	data = new float[size];
	int ioffset = size * offset;
	for(int i = 0; i < size; i++)
	{
		data[i] = maxval * exp(-sqr(i-ioffset)/width);
		if(data[i] > 1.0f) data[i] = 1.0f;
	}
}
GaussianEnvelope::~GaussianEnvelope(void)
{
	delete [] data;
}