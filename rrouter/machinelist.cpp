#include "stdafx.h"

#include "buzz.h"

int							M_loaded=0;
HMODULE						hm_dock=0;
int							mcount=0;

extern int midi_in_mode;


void (__cdecl *Offer)(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM, CMachineInfo *pInfo, void* pOpt);
int (__cdecl *Deoffer)(int opcode, CMachineInterface* pMI);
int (__cdecl *getListIndex)(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);
int (__cdecl *findListElement)(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt);
void (__cdecl *Lock)(void);
void (__cdecl *Unlock)(void);


int MachineCallback(CMachineInterface* pMI, int opcode, int iVal, int iVal2, float fVal, void *pV)
{	
	if (HIWORD(opcode)==POLAC)
	{		
		switch(LOWORD(opcode))
		{
		case MACH_WANT_MIDI:
			{
				mi* p=(mi*)pMI;
				if (pV)
				{
					BYTE *mData=(BYTE*)pV;
					p->noteDelay=iVal;
					BYTE status=(mData[0]&0xF0);
					BYTE channel=(mData[0]&0x0F);
					if ((status==0x90)||(status==0x80))
						p->MidiNote(channel,mData[1],mData[2]);
					else if (status==0xB0)		
						p->ex.MidiControlChange(mData[1],channel,mData[2]);
					else if (status==0xD0)
						p->ex.MidiControlChange(252,channel,mData[2]);
					else if (status==0xA0)
						p->ex.MidiControlChange(253,channel,((mData[1]<<8)+mData[2]));
					else if (status==0xC0)
						p->ex.MidiControlChange(254,channel,mData[1]);
					else if (status==0xE0)
						p->ex.MidiControlChange(255,channel,mData[1]+(mData[2]<<7));			
					return 1;
				}
				
			}
			break;
		case MACH_MULTI_OUT:
			{		
				mi* p=(mi*)pMI;			
				if (iVal)
				{					
					void** ppV=(void**)iVal;
					*ppV=(void**)&p->outInfo;
					return 1;
				}								
			}
			break;	
		}
	}	
	return -1;
}

void M_Lock(void)
{
	if (M_loaded)
		Lock();
}

void M_Unlock(void)
{
	if (M_loaded)
		Unlock();
}

int M_getListIndex(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return getListIndex(n, opcode, ppMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

int M_findListElement(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return findListElement(opcode,pMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

void M_Offer(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM,  CMachineInfo *pInfo, void* pOpt)
{
	if (M_loaded)
		Offer(opcode, pMI, callback, pM, pInfo, pOpt);
}

int M_Deoffer(int opcode, CMachineInterface* pMI)
{
	if (M_loaded)
		return Deoffer(opcode, pMI);
	return -1;
}

bool M_IsActive(void)
{
	if (M_loaded) return true;
	return false;
}

void M_Save(CMachineDataOutput *po, GUID *machID)
{
	po->Write((void *)machID,sizeof(GUID));
}

void M_Load(CMachineDataInput *pi, GUID *machID)
{
	pi->Read((void *)machID,sizeof(GUID));
}

void M_LoadMachines(void) 
{	
	mcount++;
	if (mcount==1)
	{		
		char buzz_dock_dll[MAX_PATH];
		buzz_dock_dll[0]=0;
		HKEY hk;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Jeskola\\Buzz\\Settings", 0, KEY_QUERY_VALUE,&hk)==ERROR_SUCCESS)
		{		
			DWORD dwType;
			DWORD dwSize=MAX_PATH;					
			if (RegQueryValueEx(hk,"BuzzPath",NULL,&dwType,(BYTE *)buzz_dock_dll,&dwSize)==ERROR_SUCCESS)
			{							
				int len=strlen(buzz_dock_dll);
				if (buzz_dock_dll[len-1]!='\\') 
				{
					buzz_dock_dll[len]='\\';
					buzz_dock_dll[len+1]=0;
				}				
				strcat(buzz_dock_dll,"Gear\\Machines.dll");			
			}
			RegCloseKey(hk);	
		}
		hm_dock=LoadLibrary(buzz_dock_dll);		
		if (hm_dock)
		{		
			Offer=(void (__cdecl *)(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM, CMachineInfo *pInfo, void* pOpt))GetProcAddress(hm_dock,"Offer");
			Deoffer=(int (__cdecl *)(int opcode, CMachineInterface* pMI))GetProcAddress(hm_dock,"Deoffer");
			Lock=(void (__cdecl *)(void))GetProcAddress(hm_dock,"Lock");		
			Unlock=(void (__cdecl *)(void))GetProcAddress(hm_dock,"Unlock");		
			getListIndex=(int (__cdecl *)(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt))GetProcAddress(hm_dock,"getListIndex");
			findListElement=(int (__cdecl *)(int opcode, CMachineInterface* pMI,FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt))GetProcAddress(hm_dock,"findListElement");
			if (Offer&&Deoffer&&Lock&&Unlock&&getListIndex&&findListElement)
			{
				M_loaded=1;		
				return;
			}
		}	
		M_loaded=0;	
	}	
}



void M_FreeMachines(void)
{
	mcount--;
	if (!mcount)
	{
		if (hm_dock)
		{
			FreeLibrary(hm_dock);
			hm_dock=0;
		}
	}
}
