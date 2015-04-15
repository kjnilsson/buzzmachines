#include "../mdk/mdk.h"
#include "machinelist.h"

class avals
{
public:
       			
};

class gvals
{
public:            
			byte ochan;		
};

class tvals
{
public:
		
};

#pragma pack()

class mi;
class miex : public CMDKMachineInterfaceEx { 
public:			
			virtual char const	*DescribeParam(int const param);			
			virtual void		GetSubMenu(int const i, CMachineDataOutput *pout){};			
			mi					*pmi;
};

class mi : public CMDKMachineInterface
{
public:
            mi();
            virtual ~mi();
            virtual void Tick();
            virtual void MDKInit(CMachineDataInput * const pi);
			//virtual void Init(CMachineDataInput * const pi);
            virtual bool MDKWork(float *psamples, int numsamples, int const mode);
            virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
            virtual void Command(int const i);
			virtual void SetNumTracks(int const n);
            virtual void MDKSave(CMachineDataOutput * const po);
            virtual char const *DescribeValue(int const param, int const value);
            virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
            virtual void OutputModeChanged(bool stereo) {}


			void				CreateOutBufs(void);
			void				DeleteOutBufs(void);
			
			CMachine*			thismachine;
			miex				ex;
			int					numTracks;
            avals				aval;
			gvals				gval;
			tvals				tval;
			int					noteDelay;
			GUID				machID;
			OUTINFO				outInfo;
			float				*outbuf[32];
			float				*dummybuf;
			float				*mybuf;
			int					q;

			int					s_mode;


};



                       

