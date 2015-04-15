#pragma once
#include "Structs.h"

class GrainBase
{
public:
    GrainBase(void);
	virtual ~GrainBase(void);
public:
	int Length; //in frames
	float Rate;
	bool IsActive;

protected:
	sample nextSample;
	int glCounter;
public:
	virtual void GetNext();
	void Generate();
};
