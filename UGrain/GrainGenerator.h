#pragma once
#include <vector>
//#include "WaveTableReader.h"
#include "Grain.h"
typedef std::vector<Grain*> GrainVector;

class InputConnection;

class OutputConnection
{
public:
	OutputConnection()
	{
		input = NULL;
	}
	void Connect(InputConnection *input)
	{
		this->input = input;
	}

private:
	InputConnection  *input;
};
class InputConnection
{
public:
	InputConnection()
	{
		output = NULL;
	}
	void Connect(OutputConnection *output)
	{
		this->output = output;
	}

private:
	OutputConnection *output;

};


class GrainGenerator
{
public:
	GrainGenerator(CMICallbacks *pCB, CMasterInfo *pMasterInfo);
	~GrainGenerator(void);
	void Generate(sample * psamples, int numsamples);

	void ActivateGrain(int delay);
	void Play();
	void Stop();
	GrainSettings gs;

private:
	bool playing;
	int maxGrains;
	CMICallbacks *pCB;
	CMasterInfo *pMasterInfo;
	GrainVector grains;
	Grain* FindNextGrain();
};
