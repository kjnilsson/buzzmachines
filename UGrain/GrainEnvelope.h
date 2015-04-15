#pragma once
#include <cmath>

#include "GaussianEnvelope.h"



#define GE_SIZE 256

class GrainEnvelope
{
public:
	GrainEnvelope(void);
	~GrainEnvelope(void);


private:
	int length;
	GaussianEnvelope * g1;
public:
	bool Init(int length);
	float incr;
	float GetOne(void);
	float count;
};
