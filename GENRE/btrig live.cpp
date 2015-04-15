#include "../mdk/mdk.h"
#include <windows.h>

#include "ahdenv.h"
#include "math.h"
#include "resource.h"

#pragma optimize ("awy", on) 

#define MAX_BUFF_LEN 441000
/*
CMachineParameter const paraInput =
{
            pt_switch,                        // Parameter data type
            "Input",                        // Parameter name as its shown in the parameter 
                                                // window
            "Input",// Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            1,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            0,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0                        // the default slider value
};*/
CMachineParameter const paraReTrigger =
{
            pt_byte,                        // Parameter data type
            "ReTrigLength",                        // Parameter name as its shown in the parameter
                                                // window
            "ReTrigger Length",            // Parameter description as its shown in
                                                //the pattern view's statusbar
            0,                                    // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0x20                        // the default slider value
};

CMachineParameter const paraTrigger =
{
            pt_switch,                        // Parameter data type
            "Trigger",                        // Parameter name as its shown in the parameter 
                                                // window
            "Trigger new Retrigger",// Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            1,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0                        // the default slider value
};

CMachineParameter const paraRTVol =
{
            pt_byte,            // Parameter data type
            "Retrigger Vol",// Parameter name as its shown in the parameter
                                    // window
            "Initial Volume of Retrigger",// Parameter description as its shown in the 
                                    // pattern view's statusbar
            0,                        // Minimum value
            0xFE,            // Maximum value
            0xFF,            // Novalue, this value means "nothing 
                                    // happened" in the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as 
                                    // a slider
            0x80           // the default slider value
};
CMachineParameter const paraBufferMode =
{
            pt_switch,            // Parameter data type
            "BufferMode",// Parameter name as its shown in the parameter
                                    // window
            "Buffer Mode, Overwrite or Add",// Parameter description as its shown in the 
                                    // pattern view's statusbar
            0,                        // Minimum value
            1,            // Maximum value
            0xFF,            // Novalue, this value means "nothing 
                                    // happened" in the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as 
                                    // a slider
            0            // the default slider value
};
CMachineParameter const paraEffectCommand =
{
            pt_byte,                        // Parameter data type
            "Effect Command",            // Parameter name as its shown in the parameter 
                                                // window
            "Retrigger Effect Command",            // Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0xFE                        // the default slider value
};

CMachineParameter const paraEffectArgument =
{
            pt_byte,            // Parameter data type
            "Argument",            // Parameter name as its shown in the parameter 
                                    // window
            "retrigger Argument",            // Parameter description as its shown in the pattern 
                                    // view's statusbar
            0,                        // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing happened" in 
                                    // the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as a 
                                    // slider
            0                        // the default slider value
};
CMachineParameter const paraAttack =
{
            pt_byte,            // Parameter data type
            "Attack",            // Parameter name as its shown in the parameter 
                                    // window
            "retrigger Attack",            // Parameter description as its shown in the pattern 
                                    // view's statusbar
            0,                        // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing happened" in 
                                    // the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as a 
                                    // slider
            0                        // the default slider value
};
CMachineParameter const paraDecay =
{
            pt_byte,            // Parameter data type
            "Decay",            // Parameter name as its shown in the parameter 
                                    // window
            "retrigger Decay",            // Parameter description as its shown in the pattern 
                                    // view's statusbar
            0,                        // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing happened" in 
                                    // the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as a 
                                    // slider
            0                        // the default slider value
};
CMachineParameter const paraDryVolume = 
{
	pt_byte,										// type
	"Dry Volume",
	"Dry Volume",										// description
	0,												// MinValue
	200,											// MaxValue
	255,											// NoValue
	MPF_STATE,										// Flags
	0
};

CMachineParameter const *pParameters[] = {
            //&paraInput,
            &paraReTrigger,
            &paraTrigger,
            &paraRTVol,
			&paraBufferMode,
            &paraEffectCommand,
			&paraEffectArgument,
			&paraAttack,
			&paraDecay,
			&paraDryVolume
};

CMachineAttribute const *pAttributes[] = { NULL };

#pragma pack(1)                        

class gvals
{
public:
            //byte input;
            byte retriglength;
            byte trigger;
            byte rtvol;
			byte buffermode;
            byte effectcommand;
			byte effectargument;
			byte attack;
			byte decay;
			byte dryvolume;
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
#pragma pack()

CMachineInfo const MacInfo = 
{
            MT_EFFECT,                        // Machine type
            MI_VERSION,                        // Machine interface version
            MIF_DOES_INPUT_MIXING,            // Machine flags
            0,                                    // min tracks
            0,                                    // max tracks
            9,                                    // numGlobalParameters
            0,                                    // numTrackParameters
            pParameters,            // pointer to parameter stuff
            0,                                    // numAttributes
            pAttributes,            // pointer to attribute stuff
            "GenRe - General Retrigger Machine",            // Full Machine Name
            "GR LIVE",                        // Short name
            "Karl Nilsson",            // Author name
            "&About..."                        // Right click menu commands
};

class ECommand
{
public:
	bool IsActive;
	int IntValue;
	float FloatValue;
};

class mi;

class miex : public CMDKMachineInterfaceEx { };

class mi : public CMDKMachineInterface
{
public:
            mi();
            virtual ~mi();
            virtual void Tick();
            virtual void MDKInit(CMachineDataInput * const pi);
            virtual bool MDKWork(float *psamples, int numsamples, int const mode);
            virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
            virtual void Command(int const i);
            virtual void MDKSave(CMachineDataOutput * const po);
            virtual char const *DescribeValue(int const param, int const value);
            virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
            virtual void OutputModeChanged(bool stereo) {}
			virtual void Stop();
///			virtual void SetNumTracks(const int n) {numTracks = n; };

  public:
            miex ex;

public:		
			gvals gval;

public: 
			AHDEnvelope Env;
			int C_Env;

public:
			void DoRetriggers(float l, float r);
			void SetCommands();
			
			void SetRetrigVals(int spt);
			void SetNewRetrigger();

			virtual float const SliderToVolume(float sliderpos);
			virtual float const VolumeToDB(float volume);
			virtual float const DBtoVolume(float volumedb);

public:	
			float *Buffer;
			int BufferLen;
			bool IsTriggering;
			bool ReadPhase;
			
			//to hold paramtervalues
			int PrRetrigLen;
			int PrTrigger;
			int PrRTVol;
			int PrSkipFirst;
			int PrEffectCommand;
			int PrEffectArgument;
			int PrAttack;
			int PrDecay;
			
			//calculated parmatervalues
			int RetrigLen;
			double ReadLen;
			int Trigger;
			float RTVol;
			bool SkipFirst;
			int EffectCommand;
			int EffectArgument;
			float Attack;
			float Decay;
			//int AtkLen;
			//int DecLen;
			float RLamt, LRamt;
			int PanPos;

			int BufferMode;

			bool Reverse;
			bool SetNextRtrg;
			//counters
			int C_RetrigLen;
			int C_ReadHead;
			float C_WriteHead;
			int RT_Count; //for counting the retrigger lengths....
			//int C_Atk, C_Dec;

			float DryVol;
			float PVolume;

			float rate;
			float rrate;
			float randlen;
			float rvol;

		//bool AtkPhase, DecPhase;
			
			int Offset;
			int OffsInc;

			//0F
			int Flen;
			int C_F;


public:		//command stuff
			ECommand EffCom[128]; 

			void SetNewEffCom(int cmd, int arg);
			void ApplyEffComs();
};

	
/////////////////////////////////////////////////////////
mi::mi() {  GlobalVals = &gval; }
mi::~mi() { 
	delete [] Buffer;
}

void mi::SetNewEffCom(int cmd, int arg)
{
	int i = 0;
	switch(cmd)
	{
	case 0:
		C_WriteHead = 0.0f;
		RT_Count = 0;
		IsTriggering = true;
		ReadPhase = false;
//		AtkPhase = true;
		//DecPhase = false;
		break;
	case 1:
		EffCom[1].IsActive = true;
		EffCom[1].FloatValue = (float)arg / (float)0x80;
		break;
	case 2:
		EffCom[2].IsActive = true;
		EffCom[2].FloatValue = (float)arg / (float)0x80;
		break;
	case 3:
		EffCom[3].IsActive = true;
		EffCom[3].IntValue = (int)((float)arg / 16.0f) * pMasterInfo->SamplesPerTick;

		
		//EffCom[3].IntValue = 0;
		/*EffCom[3].IntValue = (int)(Offset * RetrigLen);
		if ((EffCom[3].IntValue % 2) != 0)
			EffCom[3].IntValue += 1;*/
		break;
	case 4:
		EffCom[4].IsActive = true;
		//EffCom[4].IntValue = (int)((arg - 0x80) * ((float)RetrigLen / (float)0xFE));
		EffCom[4].IntValue = (int)((arg / 256.0f) * RetrigLen);
		break;
	case 5:
		if (!EffCom[5].IsActive){
			EffCom[5].IsActive = true;

			EffCom[3].IntValue = 0;//turn off the offset for the time being.
			EffCom[3].IsActive = false;
		}
		else 
			EffCom[5].IsActive = false;
		//SkipFirst = true;
		break;
	case 6:
		EffCom[6].IsActive = true;
		EffCom[6].IntValue = arg;
		PanPos = EffCom[6].IntValue;
		
		break;
	case 7:
		EffCom[7].IsActive = true;
		//EffCom[6].IsActive = true;
		EffCom[7].IntValue = arg - 128;
		break;
	case 8: 
		EffCom[8].IsActive = true;
		EffCom[8].IntValue = arg;
		break;
	case 9:
		EffCom[9].IsActive = true;
		EffCom[9].FloatValue = (float)arg;
		break;
	case 10:
		EffCom[10].IsActive = true;
		EffCom[10].FloatValue = (float)arg;
		break;
	case 11:
		EffCom[11].IsActive = true;
		EffCom[10].IsActive = false;
		EffCom[11].FloatValue = (float)arg;
		break;
	case 12://Random Pitch. Argument = Amount.
		EffCom[12].IsActive = true;
		EffCom[12].FloatValue = (float)arg;
		break;
	case 13:
		EffCom[13].IsActive = true;
		if (arg == 0) EffCom[13].IntValue = 0x80;
		else EffCom[13].IntValue = arg;
		break;
	case 14:
		EffCom[14].IsActive = true;
		EffCom[14].FloatValue = (float)arg;
		break;
	case 15:
		EffCom[15].IsActive = true;
		Flen = (int)(pMasterInfo->SamplesPerTick * (arg/128.0f));
		C_F = 0;
		break;
	case 160:
		for(i = 0; i < 128; i++){
			EffCom[i].IsActive = false;
			EffCom[i].IntValue = 0;
			EffCom[i].FloatValue = 0.0f;
			}
			Reverse = false;
			Offset = OffsInc = 0;
			LRamt = RLamt = 0.0f;
			EffCom[6].IntValue = 128;
			rate = rrate = randlen = 1.0f;
		break;
	case 161://off vol per retrg
		EffCom[1].IsActive = false;
		EffCom[1].IntValue = 0;
		EffCom[1].FloatValue = 0.0f;
		break;
	case 162://off rtlen per retrg
		EffCom[2].IsActive = false;
		EffCom[2].IntValue = 0;
		EffCom[2].FloatValue = 0.0f;
		break;
	case 163://off offset
		EffCom[3].IsActive = false;
		EffCom[3].IntValue = 0;
		EffCom[3].FloatValue = 0.0f;
		break;
	case 164://off offs per rt
		EffCom[4].IsActive = false;
		EffCom[4].IntValue = 0;
		EffCom[4].FloatValue = 0.0f;
		break;	
	case 165://off reverse
		EffCom[5].IsActive = false;
		EffCom[5].IntValue = 0;
		EffCom[5].FloatValue = 0.0f;
		break;
	case 166://off man pan
		EffCom[6].IsActive = false;
		EffCom[6].IntValue = 0;
		EffCom[6].FloatValue = 0.0f;
		break;	
	case 167://off pan per rtg
		EffCom[7].IsActive = false;
		EffCom[7].IntValue = 0;
		EffCom[7].FloatValue = 0.0f;
		break;
	case 168://off rand pan
		EffCom[8].IsActive = false;
		EffCom[8].IntValue = 0;
		EffCom[8].FloatValue = 0.0f;
		break;
	case 169://off rand vol
		EffCom[9].IsActive = false;
		EffCom[9].IntValue = 0;
		EffCom[9].FloatValue = 0.0f;
		break;
	case 170://off man pitch
		EffCom[10].IsActive = false;
		EffCom[10].IntValue = 0;
		EffCom[10].FloatValue = 0.0f;
		break;
	case 171://off pitch per rtg
		EffCom[11].IsActive = false;
		EffCom[11].IntValue = 0;
		EffCom[11].FloatValue = 0.0f;
		break;
	case 172://off rand pitch
		EffCom[12].IsActive = false;
		EffCom[12].IntValue = 0;
		EffCom[12].FloatValue = 0.0f;
		break;
	case 173://off nothing
		EffCom[13].IsActive = false;
		EffCom[13].IntValue = 0;
		EffCom[13].FloatValue = 0.0f;
		break;
	case 174://off rand rtg len
		EffCom[14].IsActive = false;
		EffCom[14].IntValue = 0;
		EffCom[14].FloatValue = 0.0f;
		break;
	case 175://off man declick
		EffCom[15].IsActive = false;
		EffCom[15].IntValue = 0;
		EffCom[15].FloatValue = 0.0f;
		break;

	default:
		NULL;
	}
}

int ReturnRandPan(int range)//range is a value between 0 and 255
{
float r = (float)rand() / RAND_MAX;
return (int)(range*r-(range/2));
} 

float ReturnRandRate(float amt)
{
float r = (float)rand() / RAND_MAX;
return 	1.0f + ((amt*r) - (amt/2));
}	



void mi::ApplyEffComs()
{
	

	if (EffCom[1].IsActive){ //volume
		if (EffCom[1].FloatValue == 0) RTVol *= 1.0f;
		else RTVol *= EffCom[1].FloatValue;
	}
	
	if (EffCom[2].IsActive){ //retirgger length
		if (EffCom[2].FloatValue == 0) RetrigLen = (int)((float)RetrigLen * 1);
		else RetrigLen = (int)((float)RetrigLen * EffCom[2].FloatValue);
	}
	if (EffCom[3].IsActive){
		Offset = EffCom[3].IntValue;// = (int)(Offset * RetrigLen);
		if ((Offset % 2) != 0)
			Offset += 1;
		if (Offset > RetrigLen){
			Offset = 0;
			EffCom[3].IsActive = false;
		}
	}
	if (EffCom[4].IsActive){
		OffsInc = OffsInc + EffCom[4].IntValue;
		if (OffsInc > RetrigLen) OffsInc = 0;
		//if (OffsInc < -C_Env) OffsInc = -C_Env;
	}else{
		OffsInc = 0;
	}
	if (EffCom[5].IsActive){
		EffCom[3].IsActive = false;//turn off offset
		Reverse = true;
	}
	else Reverse = false;

	if (EffCom[6].IsActive){
		if (PanPos > 128){	//rght pan
			LRamt = (float)(PanPos - 128.0f) / 128.0f;
			RLamt = 0.0f;}
		else {
			RLamt = 1.0f - ((float)PanPos / 128.0f);
			LRamt = 0.0f;}
	}
	
	if (EffCom[7].IsActive){
		PanPos += EffCom[7].IntValue;
		if (PanPos > 255) PanPos = 255; //dont fall off the edge
		if (PanPos < 0) PanPos = 0;

		if (PanPos > 128){	//rght pan
			LRamt = (float)(PanPos - 128.0f) / 128.0f;
			RLamt = 0.0f;}
		else {
			RLamt = 1.0f - ((float)PanPos / 128.0f);
			LRamt = 0.0f;}
	}
	if (EffCom[8].IsActive){
		PanPos = EffCom[6].IntValue;
		PanPos += EffCom[7].IntValue + ReturnRandPan(EffCom[8].IntValue);
		if (PanPos > 255) PanPos = 255; //dont fall off the edge
		if (PanPos < 0) PanPos = 0;

		if (PanPos > 128){	//rght pan
			LRamt = (float)(PanPos - 128.0f) / 128.0f;
			RLamt = 0.0f;}
		else {
			RLamt = 1.0f - ((float)PanPos / 128.0f);
			LRamt = 0.0f;}
	//	printf ("\ne!: : %d",ReturnRandPan(EffCom[8].IntValue));
	}
	if (EffCom[9].IsActive){
		rvol = ReturnRandRate(EffCom[9].FloatValue / 256.0f);
	}
	if (EffCom[10].IsActive){// man pitch
		
		if (EffCom[10].FloatValue == 0) EffCom[10].FloatValue = 128.0f;
		rate = EffCom[10].FloatValue / 128.0f;
	}
	if (EffCom[11].IsActive){//Increase or decrease the Pitch each retrigger
		EffCom[10].IsActive = false;
		rate *= EffCom[11].FloatValue / 128.0f;
	}	
	if (EffCom[12].IsActive){//Random Pitch. Argument = Amount.
		 
		rrate = ReturnRandRate(EffCom[12].FloatValue / 256.0f);
	}
	if (EffCom[13].IsActive){ //pitch in semitones
		 
		if (EffCom[13].IntValue == 0) EffCom[13].IntValue = 0x80;

		rate = powf(2.0f, ((EffCom[13].IntValue-0x80) / 12.0f));
	}
	if (EffCom[14].IsActive){
		 
		randlen = ReturnRandRate(EffCom[14].FloatValue / 256.0f);
		//RetrigLen *= randlen;
	}


}



void mi::MDKInit(CMachineDataInput * const pi)
{
			//numTracks = 1;
			//AllocConsole(); freopen("CON", "w", stdout);

            SetOutputMode( true ); // No mono sounds
			
			Buffer = NULL;
	
			RetrigLen = 0;
			Trigger = 0;
			PrRTVol = 0x80;
			RTVol = 1.0f;
			RT_Count = 0;
			SkipFirst = 1;
			EffectCommand = 0;
			EffectArgument = 0;
//			AtkLen = DecLen = C_Atk = C_Dec = 0;
			Attack = Decay = 0.0f;
			
			C_RetrigLen = C_ReadHead = RT_Count = 0;
			C_WriteHead = 0.0f;
			IsTriggering = false;
			ReadPhase = false;
			SkipFirst = true;
			Reverse = false;
			SetNextRtrg = false;
			Offset = 0;
			OffsInc = 0;
			
			PanPos = 80;
			LRamt = RLamt = 0.0f;

			DryVol = 0.0f;

			rate = rrate = randlen = rvol = 1.0f;

			BufferMode = 0;
			PrSkipFirst = 1;
			BufferLen = 441000;
			ReadLen = BufferLen;
			Buffer = new float[BufferLen*2];//*2 because of interleaved samples
			for (int j =0; j < BufferLen*2; j++){
				Buffer[j] = 0.0f;
			}
			
			//command stuff
			for(int i = 0; i < 128; i++){
				EffCom[i].IsActive = false;
				EffCom[i].IntValue = 0;
				EffCom[i].FloatValue = 0.0f;
			}
			EffCom[6].IntValue = 128;

			Env.Init();
			C_Env = 0;

			Flen = 0;
			C_F = 0;
		

}			

void mi::MDKSave(CMachineDataOutput * const po) { }

void mi::Tick() {

			

			if (gval.retriglength != paraReTrigger.NoValue){

				PrRetrigLen = gval.retriglength;// * pMasterInfo->SamplesPerTick;

				if (PrRetrigLen == 0) PrRetrigLen = 256; //16 tick retriggers
				SetNextRtrg = true;

				//printf ("\nPRrtlen ! : : %i",PrRetrigLen);
			}
			if (gval.trigger != paraTrigger.NoValue){
				PrTrigger = gval.trigger;

				if (PrTrigger == 1){
					RetrigLen = (int)((PrRetrigLen / 16.0f) * pMasterInfo->SamplesPerTick);
					
					IsTriggering = true;
					ReadPhase = true;
				
				SkipFirst = true;
				

				RTVol = ((float)PrRTVol /  (float)0x80);

				}
				else{
					IsTriggering = false;
					ReadPhase = false;

					//AtkPhase = DecPhase = false;
				}
				
				Env.C_Env = 0;
				Env.SetEnvParams(C_Env, RetrigLen, Attack, Decay);
				
				C_WriteHead = 0.0f;
				RT_Count = 0;
				C_ReadHead = 0;


			}

			if (gval.rtvol != paraRTVol.NoValue){
				PrRTVol = gval.rtvol;
				RTVol = ((float)PrRTVol /  (float)0x80);
			}
			if (gval.buffermode != paraBufferMode.NoValue){
				/*
				PrSkipFirst = gval.skipfirst;
				if (PrSkipFirst == 1) SkipFirst = true;
				else */
				BufferMode = gval.buffermode;

				SkipFirst = true;
			}
			
			
			if (gval.effectcommand != paraEffectCommand.NoValue){//commands

				PrEffectCommand = gval.effectcommand;
				PrEffectArgument = 0; //in case there is nothing in the argument column.

					if (gval.effectargument != paraEffectArgument.NoValue){

						PrEffectArgument = gval.effectargument;
					}
					mi::SetNewEffCom(PrEffectCommand, PrEffectArgument);
			}

			if (gval.attack != paraAttack.NoValue){
				PrAttack = gval.attack;
				Attack = ((float)PrAttack / (float)0xFE);
				//AtkLen = (int)(Attack * (RetrigLen - EffCom[3].IntValue));
				
			}
			if (gval.decay != paraDecay.NoValue){
				PrDecay = gval.decay;
				Decay = ((float)PrDecay / (float)0xFE);
				//DecLen = (int)(Decay * (RetrigLen - EffCom[3].IntValue));
			}
			
			if (gval.dryvolume != paraDryVolume.NoValue){
				PVolume = (float)(gval.dryvolume);
				DryVol = PVolume / 200.0f;

				DryVol = mi::SliderToVolume(DryVol);
			}
			
			/*
			printf ("\ne!: : %f",Env.Envelope(C_Env));
			printf ("\nAtkLen!: : %i",Env.AtkLen);*/
			//printf ("\nOFF!: : %i",OffsInc);
			//printf ("\nEFFC4!: : %i",EffCom[4].IntValue);

//	printf ("\nrtlen ! : : %i",RetrigLen);
//	printf ("\nWrite ! : : %i",C_WriteHead);
//	printf ("\nREad ! : : %i",pMasterInfo->SamplesPerTick);
//	printf ("\nRTCOunt ! : : %i",RT_Count);
	
}

float FDecay(int flen, int fpos )
{
	float c;

	c = 1.0f / flen * (flen - fpos);
//printf ("\nEFFC4!: : %i",flen);
	return c;
}

inline int f2i(double d)
{
  const double magic = 6755399441055744.0; // 2^51 + 2^52
  double tmp = (d-0.5) + magic;
  return *(int*) &tmp;
}



bool mi::MDKWork(float *psamples, int numsamples, int const mode)
{
            return false;
}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
	if (mode==WM_NOIO){
		return false;
	}

	float inL, inR, outL, outR, ppos, frac, ftmp;
	int             i, x, x2;
	float			e;

	bool madenoise = false;

	e = 1.0f;
	ftmp = 1.0f;

	for( i=0; i<numsamples; i++ ) {
		
		if (IsTriggering){

			if(mode&WM_READ){

				outL = inL = psamples[2*i];
				outR = inR = psamples[2*i+1];
			}	
			else inL = inR = outL = outR = 0.0f;

			if (ReadPhase){ //reading into buffer
						
						if (BufferMode == 1){
							Buffer[2*C_ReadHead] += inL;
							Buffer[2*C_ReadHead+1] += inR;
						}else{
							Buffer[2*C_ReadHead] = inL;
							Buffer[2*C_ReadHead+1] = inR;
						}
					}

			if(mode&WM_WRITE){
				
				ppos = (C_WriteHead + ((Offset + OffsInc)*rate)*rrate);

				if (!SkipFirst){
					x =	f2i(ppos)<<1;
					x2 = x + 2;
					if(x2 >= BufferLen*2) x2 = 0;
					frac = ((ppos) - (float)f2i(ppos));

					outL = Buffer[x] + frac*(Buffer[x2] - Buffer[x]);
					x++;
					x2++;
					outR = Buffer[x] + frac*(Buffer[x2] - Buffer[x]);

					//outL = Buffer[2*(int)ppos]; psamples[i] = pwl->pSamples[x] + frac*(pwl->pSamples[x2] - pwl->pSamples[x]);
					//outR = Buffer[2*(int)ppos + 1];
				}
				else {outL = outR = 0.0f;}

				//volume stuff
				outL *= RTVol * rvol;
				outR *= RTVol * rvol;

				Env.CheckActive();
				e = Env.Envelope(C_Env);
				outL *= e; 
				outR *= e;
				
				//0F stuff
				if (EffCom[15].IsActive)
				{
					C_F++;
					if (C_F > (pMasterInfo->SamplesPerTick - Flen))
					{
						//C_F++;
					ftmp = FDecay(Flen, C_F-(pMasterInfo->SamplesPerTick-Flen));//len, pos
					outL *= ftmp;
					outR *= ftmp;

					}
					if (C_F > pMasterInfo->SamplesPerTick){
						EffCom[15].IsActive = false;
						C_F = 0;
					}
				};

				outL = outL * (1.0f - LRamt);// + (inR * RLamt); //pan stuff
				outR = outR * (1.0f - RLamt);// + (inL * LRamt);
				
				outL += (inL * DryVol);
				outR += (inR * DryVol);
				
						
				if (outL || outR != 0.0f) madenoise = true;//HACK but it works 
				
				psamples[2*i] = outL;
				psamples[2*i+1] = outR;
			} //WM_WRITE

			

			C_ReadHead++; //incr read counter
				if (C_ReadHead >= BufferLen){ // check counter
					ReadPhase = false;
					C_ReadHead = 0;
					}
			
			if (Reverse){ //reverse
				C_WriteHead -= 1.0f * rate * rrate;
				RT_Count--;
					if (C_WriteHead < 0){

						if (SetNextRtrg){

							RetrigLen = (int)((PrRetrigLen / 16.0f) * pMasterInfo->SamplesPerTick);
							SetNextRtrg = false;
						}

						C_WriteHead = (float)(RetrigLen * randlen);//NOTE: do I need to do this not to reverse l/r??
						RT_Count = (int)(RetrigLen*randlen);
						SkipFirst = false;


						Env.C_Env = 0;
						Env.SetEnvParams(C_Env, (int)(RetrigLen*randlen) - Offset, Attack, Decay);
						
						mi::ApplyEffComs();

						}
						
			}
			else{ 
				C_WriteHead += (1.0f * rate * rrate);
				RT_Count++;
					if (RT_Count > (int)(RetrigLen * randlen)){
						if (SetNextRtrg){
							
							RetrigLen = (int)((PrRetrigLen / 16.0f) * pMasterInfo->SamplesPerTick);
							SetNextRtrg = false;
							//printf ("\nset next ! : : %f",PrRetrigLen);
						}

						C_WriteHead = 0.0f;
						RT_Count = 0;
						SkipFirst = false;
						mi::ApplyEffComs();

						Env.C_Env = 0;
						Env.SetEnvParams(C_Env, (int)(RetrigLen*randlen), Attack, Decay);
						
						}
			}
				if (C_WriteHead + Offset + OffsInc > ReadLen || C_WriteHead + Offset + OffsInc > BufferLen){//in case the write goes off the edge and needs to be silenced til nex retrigger
					SkipFirst = true;
				}


		}//end of IsTriggering
		else if (mode==WM_READWRITE){
			psamples[2*i] = psamples[2*i] * DryVol;
			psamples[2*i+1] = psamples[2*i+1] * DryVol;
			if (DryVol > 0.0f) madenoise = true;
			
		}
	}//end of for

	if (madenoise) return true;
	else return false;

}//end mi::work

void mi::Stop()
{
IsTriggering = false;
}


HINSTANCE dllInstance;
mi *g_mi;

BOOL WINAPI DllMain ( HANDLE hModule, DWORD fwdreason, LPVOID lpReserved )
{
   switch (fwdreason) {
   case DLL_PROCESS_ATTACH:
      dllInstance = (HINSTANCE) hModule;
      break;

   case DLL_THREAD_ATTACH: break;
   case DLL_THREAD_DETACH: break;
   case DLL_PROCESS_DETACH: break;
   }
   return TRUE;
}

BOOL APIENTRY AboutDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   char abouttxt[10000];  // make that array as big as needed

   switch(uMsg) {
   case WM_INITDIALOG: 

      sprintf(abouttxt,"Intoxicat Genre - A General Retrigger Machine\r\n");
      sprintf(abouttxt,"%s\r\n",abouttxt);
      sprintf(abouttxt,"%sversion 1.0\r\n",abouttxt);
      sprintf(abouttxt,"%s\r\n",abouttxt);
      sprintf(abouttxt,"%sEffect Commands:\r\n",abouttxt);
      sprintf(abouttxt,"%s\r\n",abouttxt);
      sprintf(abouttxt,"%s00 - Set the machine to read from the beginning of the buffer\r\n",abouttxt);
      sprintf(abouttxt,"%s01 - Increase or decrease the volume each retrigger. 80 = 1\r\n",abouttxt);
      sprintf(abouttxt,"%s02 - Increase or decrease the retrigger length each retrigger. 80 = 1\r\n",abouttxt);
      sprintf(abouttxt,"%s03 - Manual offset Tick = Argument/16\r\n",abouttxt);
      sprintf(abouttxt,"%s04 - Increase or decrease the offset each retrigger 80 = 1\r\n",abouttxt);
	  sprintf(abouttxt,"%s05 - Reverse. No argument needed.\r\n",abouttxt);
	  sprintf(abouttxt,"%s06 - Manual pan. 80 = Centre\r\n",abouttxt);
	  sprintf(abouttxt,"%s07 - Move Pan position by an amount each retrigger. 80 = 1\r\n",abouttxt);
	  sprintf(abouttxt,"%s08 - Random Pan. Argument = Amount.\r\n",abouttxt);
	  sprintf(abouttxt,"%s09 - Random Volume\r\n",abouttxt);
	  sprintf(abouttxt,"%s0A - Manual Pitch\r\n",abouttxt);
	  sprintf(abouttxt,"%s0B - Increase or decrease the Pitch each retrigger\r\n",abouttxt);
	  sprintf(abouttxt,"%s0C - Random Pitch. Argument = Amount.\r\n",abouttxt);
	  sprintf(abouttxt,"%s0D - Manual pitch in semi-tones. 80 = root\r\n",abouttxt);
	  sprintf(abouttxt,"%s0E - Randomise retrigger length. Argument = Amount.\r\n",abouttxt);
	  sprintf(abouttxt,"%s0F - Manual end declicker. Argument is the fraction of a tick to ramp down.\r\n",abouttxt);
	  sprintf(abouttxt,"%sA0 - Turns all Effects off\r\n",abouttxt);
	  sprintf(abouttxt,"%sAx - Turns individual effects off. E.g. A5 would turn only the reverse off whereas AA would only turn the manual pitch off.\r\n",abouttxt);

      SetDlgItemText (hDlg, IDC_ABOUTTEXT, abouttxt);
      return 1;

   case WM_SHOWWINDOW: 
      return 1;

   case WM_CLOSE:
      EndDialog (hDlg, TRUE);
      return 0;

   case WM_COMMAND:
      switch (LOWORD (wParam))
      {
      case IDOK:
         EndDialog(hDlg, TRUE);
         return 1;
      default:
         return 0;
      }
      break;
   }
   return 0;
}

void mi::Command(int const i)
{
            switch (i)
            {
            case 0:
						g_mi=this;
						DialogBox(dllInstance, MAKEINTRESOURCE (IDD_MACABOUT), GetForegroundWindow(), (DLGPROC) &AboutDialog);
                        //MessageBox(NULL,"Intoxicat GenRe 0.9\n\nA General Retrigger Machine","About Genre",MB_OK|MB_SYSTEMMODAL);
                        break;
            default:
                        break;
            }
}
char const *mi::DescribeValue(int const param, int const value)
{
			float t;
			int v;
			float translatedvolume = 0.0f;
            static char txt[16];
            switch(param)
            {
            case 0:
				sprintf(txt, "%d Tick [%d/16]", value/16, value % 16);
				return txt;
				break;
            case 1:
				if (value==0) sprintf(txt, "%d [On]",value);
				if (value==1) sprintf(txt, "%d [Off]",value);
				return txt;
				break;
            case 2:
				v = value;
				t =  ((float)v / 128 * 100);
				sprintf(txt, "%d [%.1f%%]", v, t);
				return txt;
				break;
            case 3:
				if (value==0) sprintf(txt, "%d [New]",value);
				if (value==1) sprintf(txt, "%d [Add]",value);
				return txt;
				break;
            case 4:
				if (value==0) sprintf(txt, "%d[<- Buffer]",value);
				else if (value==1) sprintf(txt, "%d[*Vol/Rtrg]",value);
				else if (value==2) sprintf(txt, "%d[*Len/Rtrg]",value);
				else if (value==3) sprintf(txt, "%d[Offset T/16]",value);
				else if (value==4) sprintf(txt, "%d[*Offs/Rtrg]",value);
				else if (value==5) sprintf(txt, "%d[Reverse]",value);
				else if (value==6) sprintf(txt, "%d[ManualPan]",value);
				else if (value==7) sprintf(txt, "%d[*Pan/Rtrg]",value);
				else if (value==8) sprintf(txt, "%d[RandPan]",value);
				else if (value==9) sprintf(txt, "%d[RandVol]",value);
				else if (value==10) sprintf(txt, "%d[ManPitch]",value);
				else if (value==11) sprintf(txt, "%d[*Pitch/Rtrg]",value);
				else if (value==12) sprintf(txt, "%d[Rand Pitch]",value);
				else if (value==13) sprintf(txt, "%d[SemiPitch]",value);
				else if (value==14) sprintf(txt, "%d[RandRtgLen]",value);
				else if (value==15) sprintf(txt, "%d[ManDeClk]",value);
				else if (value==160) sprintf(txt, "%d[All Off]",value);

				else if (value>160 && value < 176) sprintf(txt, "%d[Off %d]",value, value -160);


				else sprintf(txt, "%d [No Cmd]",value);
				return txt;
				break;
            case 5:
				sprintf(txt, "%d [Arg]",value);
				return txt;
				break;
            case 6:
				v = value;
				t =  ((float)v / 255 * 100);
				sprintf(txt, "%d [%.1f%%]", v, t);
				return txt;
				break;
            case 7:
				v = value;
				t =  ((float)v / 255 * 100);
				sprintf(txt, "%d [%.1f%%]", v, t);
				return txt;
				break;
			case 8:
				translatedvolume = (float)value / 200.0f;
				translatedvolume = mi::VolumeToDB(mi::SliderToVolume(translatedvolume));
				if (translatedvolume>-1000)
				{
					sprintf(txt, "%.1f dB", translatedvolume );
				} else
				{
					sprintf(txt, "-inf dB");
				}
				return txt;
				break;

            default:
                        return NULL;
            }
}


float const mi::SliderToVolume(float sliderpos)		// by Joachim
{
	float x = 0.0f;
	x = powf(sliderpos, 3.5f)*3.1748f;
	if (sliderpos<=0)  x=0.0f;
	return x;
}

float const mi::DBtoVolume(float volumedb)			// by Joachim
{
	float x = 0.0f;
	if (volumedb>0.0f)
	{
		x = logf(volumedb)/logf(2.0f)*6.0f;
	}
	return x;
}
float const mi::VolumeToDB(float volume)			// by Joachim
{
	float x = -1000.0f;
	if (volume>0.0f)
	{
		x = logf(volume)/logf(2.0f)*6.0f;
	}
	return x;
}
#pragma optimize ("", on)

DLL_EXPORTS