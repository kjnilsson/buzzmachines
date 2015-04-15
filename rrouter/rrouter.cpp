//#include "../mdk/mdk.h"
//#include "machinelist.h"
#include "stdafx.h"
#include "buzz.h"

//#include <windows.h>
       
#define SMALL_NO = (float)1.0E-20;
//#pragma optimize ("awy", on) 
HINSTANCE hAppInstance=0;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason==DLL_PROCESS_ATTACH)
	{									
	
	}		
	else if (fdwReason==DLL_PROCESS_DETACH)
	{		
	
	}
	hAppInstance=hinstDLL;
	return 1;
}

CMachineParameter const paraOChan =
{
            pt_byte,                        // Parameter data type
            "Ouput",                        // Parameter name as its shown in the parameter
                                                // window
            "Output Channel",            // Parameter description as its shown in
                                                //the pattern view's statusbar
            0,                                    // Minimum value
            4,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0                        // the default slider value
};



CMachineParameter const *pParameters[] = {
            &paraOChan,
    
};

CMachineAttribute const *pAttributes[] = { NULL };



CMachineInfo const MacInfo = 
{
            MT_EFFECT,                        // Machine type
            MI_VERSION,                        // Machine interface version
            MIF_DOES_INPUT_MIXING,            // Machine flags
            0,                                    // min tracks
            0,                                    // max tracks
            1,                                    // numGlobalParameters
            0,                                    // numTrackParameters
            pParameters,            // pointer to parameter stuff
            0,                                    // numAttributes
            pAttributes,            // pointer to attribute stuff
            "rrouter test",            // Full Machine Name
            "rrouter",                        // Short name
            "A BuzzDev Ex.",            // Author name
            "&About..."                        // Right click menu commands
};




mi::mi() {

	ex.pmi=this;	
	TrackVals = &tval;
	GlobalVals = &gval;
	AttrVals= (int*)&aval;

 }

mi::~mi() { 
	M_Lock();
		//M_Deoffer(MAKELONG(MACH_WANT_MIDI,POLAC),this);
		M_Deoffer(MAKELONG(MACH_MULTI_OUT,POLAC),this);
	M_Unlock();	
	M_FreeMachines();
	DeleteOutBufs();
}

char const *miex::DescribeParam(int const param)
{			
	return NULL;
}

void mi::SetNumTracks(int const n) 
{
	numTracks=n;		
}

void mi::CreateOutBufs(void)
{	
	outInfo.numOutputs=8;
	strcpy(outInfo.machineName,"rrouter");		
	for (int i=0;i<4;i++)
	{		
		outbuf[i*2]=new float[MAX_BUFFER_LENGTH];		//left
		ZeroMemory(outbuf[i*2],MAX_BUFFER_LENGTH*sizeof(float));
		outbuf[(i*2)+1]=new float[MAX_BUFFER_LENGTH];		//right
		ZeroMemory(outbuf[(i*2)+1],MAX_BUFFER_LENGTH*sizeof(float));
		char buf[80];
		sprintf(buf,"Out L %d",i+1);
		strcpy(outInfo.outputNames[i*2],buf);
		sprintf(buf,"Out R %d",i+1);
		strcpy(outInfo.outputNames[(i*2)+1],buf);
	}
	dummybuf=new float[MAX_BUFFER_LENGTH];
	ZeroMemory(dummybuf,MAX_BUFFER_LENGTH*sizeof(float));
	
	for (i=8;i<32;i++) //just zeroing the remaining buffers
		outbuf[i]=dummybuf;					
	
	outInfo.out=outbuf;
}

void mi::DeleteOutBufs(void)
{
	if (dummybuf) delete[] dummybuf;
	for (int i=0;i<8;i++)
		if (outbuf[i]) delete[] outbuf[i];	
}

void mi::MDKInit(CMachineDataInput * const pi)
{
            SetOutputMode( true ); // No mono sounds
			
			//AllocConsole(); freopen("CON", "w", stdout);

			thismachine = pCB->GetThisMachine();	
			pCB->SetnumOutputChannels(thismachine,2);
			pCB->SetMachineInterfaceEx((CMachineInterfaceEx *)&ex);	
			CMachineInfo *cminfo=(CMachineInfo *)pCB->GetMachineInfo(thismachine);
			M_LoadMachines();			
			
			CreateOutBufs();			//MACH_MULTI_OUT
			
			//CoCreateGuid(&machID);		//MACH_WANT_MIDI
			
			M_Lock();
				//M_Offer(MAKELONG(MACH_WANT_MIDI,POLAC),this,(FARPROC)MachineCallback,thismachine,cminfo,&machID);
				M_Offer(MAKELONG(MACH_MULTI_OUT,POLAC),this,(FARPROC)MachineCallback,thismachine,cminfo,0);
			M_Unlock();	
			//if (pi!=NULL) 	
			//	M_Load(pi,&machID);	//MACH_WANT_MIDI
			q = 0;
			s_mode = 0;
			//id = (float)1.0E-20;

}

void mi::MDKSave(CMachineDataOutput * const po) {
//M_Save(po,&machID);		//MACH_WANT_MIDI
 }

void mi::Tick() {
if (gval.ochan != paraOChan.NoValue)
{
	//outbuf[2*s_mode] = dummybuf;
	//utbuf[(2*s_mode)+1] = dummybuf;
	s_mode = (int)(gval.ochan);

}

}

bool mi::MDKWork(float *psamples, int numsamples, int const mode)
{
            return false;
}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
       

			if (mode==WM_WRITE)
                        return false;
            if (mode==WM_NOIO)
                        return false;
            if (mode==WM_READ)                        // <thru>
                        return true;

            float inL, inR, outL, outR;
            int            i;
			float id = (float)1.0E-20;
			

                        for( i=0; i<numsamples; i++ ) {
									
									

                                    outL = inL = psamples[2*i];
                                    outR = inR = psamples[2*i+1];


									
									outbuf[0][i] = 0.0f;
									outbuf[1][i] = 0.0f;
									outbuf[2][i] = 0.0f;
									outbuf[3][i] = 0.0f;
									outbuf[4][i] = 0.0f;
									outbuf[5][i] = 0.0f;
									outbuf[6][i] = 0.0f;
									outbuf[7][i] = 0.0f;

									outbuf[2*s_mode][i] = outL/0x8000;
									outbuf[(2*s_mode)+1][i] = outR/0x8000;
				
                                    psamples[2*i] = outL;
                                    psamples[2*i+1] = outR;
                        };

						
            return true;
}

void mi::Command(int const i)
{
            switch (i)
            {
            case 0:
                        MessageBox(NULL,"PDist 1.0\n\nThis is an example created from a buzzdev effects tutorial written by CyanPhase","About PDist",MB_OK|MB_SYSTEMMODAL);
                        break;
            default:
                        break;
            }
}
char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[32];
	strcpy(txt,"unused");
	return txt;
}

#pragma optimize ("", on)




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


int __cdecl MachineCallback(CMachineInterface* pMI, int opcode, int iVal, int iVal2, float fVal, void *pV)
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

void __cdecl M_Lock(void)
{
	if (M_loaded)
		Lock();
}

void __cdecl M_Unlock(void)
{
	if (M_loaded)
		Unlock();
}

int __cdecl M_getListIndex(int n, int opcode, CMachineInterface** ppMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return getListIndex(n, opcode, ppMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

int __cdecl M_findListElement(int opcode, CMachineInterface* pMI, FARPROC* pCallback, CMachine** ppM, CMachineInfo **ppInfo, void** ppOpt)
{
	if (M_loaded)
		return findListElement(opcode,pMI, pCallback, ppM, ppInfo, ppOpt);
	return -1;
}

void __cdecl M_Offer(int opcode, CMachineInterface* pMI, FARPROC callback, CMachine* pM,  CMachineInfo *pInfo, void* pOpt)
{
	if (M_loaded)
		Offer(opcode, pMI, callback, pM, pInfo, pOpt);
}

int __cdecl M_Deoffer(int opcode, CMachineInterface* pMI)
{
	if (M_loaded)
		return Deoffer(opcode, pMI);
	return -1;
}

bool __cdecl M_IsActive(void)
{
	if (M_loaded) return true;
	return false;
}

void __cdecl M_Save(CMachineDataOutput *po, GUID *machID)
{
	po->Write((void *)machID,sizeof(GUID));
}

void __cdecl M_Load(CMachineDataInput *pi, GUID *machID)
{
	pi->Read((void *)machID,sizeof(GUID));
}

void __cdecl M_LoadMachines(void) 
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



void __cdecl M_FreeMachines(void)
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

DLL_EXPORTS