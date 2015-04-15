#pragma once


inline float sqr(const int i)
{
	return (float)i*i;
}
class GaussianEnvelope
{
public:
	GaussianEnvelope(int size, int width, float offset, float maxval);
	~GaussianEnvelope(void);
public:
	float * data;
};