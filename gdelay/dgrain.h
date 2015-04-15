#define STAT_INACTIVE 0
#define STAT_READ 4
#define STAT_WAIT 8
#define STAT_PLAY 16

#include "frand.h"

#define WM_NOIO					0
#define WM_READ					1
#define WM_WRITE				2
#define WM_READWRITE			3

class DGrain
{
public:

	DGrain();
	~DGrain();
	
	void Reset();
	void Init(int bsize);
	void Activate(int glen, int dlen, float a, float dec, float spread, int offset, int drift);
	void Read(float *psamples, int numsamples, const int mode);
	void GenerateAdd(float *psamples, int numsamples);
	bool IsActive();
	float GaussEnv(int pos);
	float GaussEnvTbl(int pos, int len);
	float GetEnvTbl(int pos);
	void UpdateDecay(float r);
	void SetPan(float s);


	int Status;
	

private:
	
	int grainlen_counter;
	int delaylen_counter;
	int grainlen;
	int delaylen;
	int write_index;
	int read_index;
	float amp;
	float * buffer;
	float probability;
	int buffersize;
	bool hasread;
	float lamp;
	float ramp;
	int write_offset;
	float envtable[129];
	float panspread;
	int driftvalue;
	int driftvalue_orig;

};