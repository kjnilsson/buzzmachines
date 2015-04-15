#ifndef __BTDSYS_HERMITE_H
#define __BTDSYS_HERMITE_H

inline float GetSampleM(const float Pos, const short *pSamples, const int numSamples);
inline float GetSampleL(const float Pos, const short *pSamples, const int numSamples);
inline float GetSampleR(const float Pos, const short *pSamples, const int numSamples);

inline float GetSampleM(const float Pos, const float *pSamples, const int numSamples);
inline float GetSampleL(const float Pos, const float *pSamples, const int numSamples);
inline float GetSampleR(const float Pos, const float *pSamples, const int numSamples);

inline int f2i(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}

inline float hermite(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

inline float GetSampleM(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],pSamples[iPos+2]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[1],pSamples[2]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleL(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],pSamples[iPos2+4]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[2],pSamples[4]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleR(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],pSamples[iPos2+5]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[1],pSamples[3],pSamples[5]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],0.0f,0.0f);
	else
		return 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float GetSampleM(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],pSamples[iPos+2]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[1],pSamples[2]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleL(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],pSamples[iPos2+4]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[2],pSamples[4]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleR(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = f2i(Pos);
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],pSamples[iPos2+5]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[1],pSamples[3],pSamples[5]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],0.0f,0.0f);
	else
		return 0.0f;
}
















#endif
