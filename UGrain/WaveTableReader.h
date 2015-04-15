#pragma once
#include "..\..\buzzfiles\MachineInterface.h"
#include "Structs.h"

class WaveTableReader
{
public:
	WaveTableReader(void);
	~WaveTableReader(void);
	void Init(CMICallbacks *pCB, CMasterInfo *pMasterInfo);
	bool PlayWave(int waveNumber, float rate);
	bool SetNewWave(int waveNumber);
private:
	inline void GetHermite(sample &s);
	inline void GetLinear(sample &s);
public:
	inline void GetOne(sample &s);
	bool ProcessBuffer(sample *psamples, int numsamples);

private:
	CMICallbacks *pCB;
	CMasterInfo *pMasterInfo;
	const CWaveLevel *pwl;
	CWaveInfo *winfo;
	bool isStereo;
	bool playing;
	float spsFactor;
	float rate;
	float pos;
public:
	bool WaveIsValid(void);
private:
	unsigned int waveNSamples;
public:
	void SetRate(float r);
};








