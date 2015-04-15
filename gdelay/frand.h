#include <time.h>

inline float frand()
// returns random value between 0 and 1
{
 	static long stat = time(NULL);//0x16BA2118; //GetTickCount (); //seed
 	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

/*
//gauss env
inline float sqrf(float x) {return x*x;}
pfEnvTable[0] = pfEnvTable[1] = pfEnvTable[ENVTABLE_LENGTH] = pfEnvTable[ENVTABLE_LENGTH+1] = 0.0f;

int i;
if (fEnvParam == 0.0f) fEnvParam = 0.000001f;
for (i=0; i<ENVTABLE_LENGTH; i++)
 	pfEnvTable[i+2] = expf( -sqrf( ((float)i / (float)ENVTABLE_LENGTH - 0.5f)/fEnvParam ) );
	
	
#include "stdafx.h"
#include <iostream>
#include "math.h"

inline float sqrf(float x) {return x*x;}

int _tmain(int argc, _TCHAR* argv[])
{
	float fEnvParam = 0.2f;
	if (fEnvParam == 0.0f) fEnvParam = 0.000001f;
	for (int i=0; i<32; i++)
	{
		std::cout << expf( -sqrf( ((float)i / (float)32 - 0.5f)/fEnvParam ) );
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}
*/

inline int f2i(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}