
#include "../mdk/machineinterface.h"



#define POLAC						0

#define MACH_WANT_MIDI				0
#define MACH_MULTI_OUT				1
#define MACH_WINDOW					2
#define MACH_PATTERN_CHANGE			3

class OUTINFO
{
public:	
	char						unused;
	float						**out;
	char						machineName[80];
	int							numOutputs;
	char						outputNames[32][80];
	OUTINFO()
	{	
		unused=0;
		out=0;
		ZeroMemory(&machineName,80);
		numOutputs=0;
		ZeroMemory(&outputNames,(32*80));		
	};	
};

typedef int (*MACHINECALLBACK)(CMachineInterface*,int,int,int,float,void*);

void __cdecl							M_LoadMachines(void);
void __cdecl							M_FreeMachines(void);
bool __cdecl							M_IsActive(void);
void __cdecl							M_Load(CMachineDataInput *pi,GUID *machID);
void __cdecl							M_Save(CMachineDataOutput *po,GUID *machID);


// for synchronisation purpose
void __cdecl							M_Lock(void);
void __cdecl							M_Unlock(void);

int	__cdecl							MachineCallback(CMachineInterface* pMI, int opCode, int iVal, int iVal2, float fVal, void *pV);

//	the functions below still have to be used in combination with M_Lock()/M_Unlock()
//
//	example:
//	
//	M_Lock();
//		M_Offer(...);
//		M_getListIndex(...);
//	M_Unlock();
void __cdecl							M_Offer(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM, CMachineInfo *pInfo, void* pOpt);
int	__cdecl							M_Deoffer(int opcode, CMachineInterface* pMI);
int	__cdecl							M_getListIndex(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);
int	__cdecl							M_findListElement(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);



