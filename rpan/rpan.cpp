#include "../mdk/mdk.h"
#include <windows.h>

#pragma optimize ("awy", on) 

CMachineParameter const paraPanPos =
{
            pt_byte,                        // Parameter data type
            "Pan",                        // Parameter name as its shown in the parameter
                                                // window
            "PanPosition",            // Parameter description as its shown in
                                                //the pattern view's statusbar
            0,                                    // Minimum value
            200,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            100                        // the default slider value
};

CMachineParameter const paraBottomTresh =
{
            pt_word,                        // Parameter data type
            "Bottom-T",                        // Parameter name as its shown in the parameter 
                                                // window
            "Bottom Treshold",// Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            0xFFFE,                        // Maximum value
            0xFFFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0xFFFE                        // the default slider value
};

CMachineParameter const paraTopClamp =
{
            pt_word,            // Parameter data type
            "Top-Clamp",// Parameter name as its shown in the parameter
                                    // window
            "Top Clamp",// Parameter description as its shown in the 
                                    // pattern view's statusbar
            0,                        // Minimum value
            0xFFFE,            // Maximum value
            0xFFFF,            // Novalue, this value means "nothing 
                                    // happened" in the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as 
                                    // a slider
            0xFFFE            // the default slider value
};

CMachineParameter const paraBottomClamp =
{
            pt_word,                        // Parameter data type
            "BottomClamp",            // Parameter name as its shown in the parameter 
                                                // window
            "Bottom Clamp",            // Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            0xFFFE,                        // Maximum value
            0xFFFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0xFFFE                        // the default slider value
};

CMachineParameter const paraDryOut =
{
            pt_byte,            // Parameter data type
            "Dry Out",            // Parameter name as its shown in the parameter 
                                    // window
            "Dry Out",            // Parameter description as its shown in the pattern 
                                    // view's statusbar
            0,                        // Minimum value
            0xFE,                        // Maximum value
            0xFF,                        // Novalue, this value means "nothing happened" in 
                                    // the mi::Tick procedure
            MPF_STATE,            // Parameter options, MPF_STATE makes it appears as a 
                                    // slider
            0                        // the default slider value
};

CMachineParameter const *pParameters[] = {
            &paraPanPos,
            &paraBottomTresh,
            &paraTopClamp,
            &paraBottomClamp,
            &paraDryOut
};
CMachineAttribute const *pAttributes[] = { NULL };

#pragma pack(1)                        

class gvals
{
public:
            byte panpos;
            word bottomtresh;
            word topclamp;
            word bottomclamp;
            byte dryout;
};

#pragma pack()

CMachineInfo const MacInfo = 
{
            MT_EFFECT,                        // Machine type
            MI_VERSION,                        // Machine interface version
            MIF_DOES_INPUT_MIXING,            // Machine flags
            0,                                    // min tracks
            0,                                    // max tracks
            5,                                    // numGlobalParameters
            0,                                    // numTrackParameters
            pParameters,            // pointer to parameter stuff
            0,                                    // numAttributes
            pAttributes,            // pointer to attribute stuff
            "Ome PDist",            // Full Machine Name
            "PDist",                        // Short name
            "A BuzzDev Ex.",            // Author name
            "&About..."                        // Right click menu commands
};


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

public:
            miex ex;

public:
            int pan_position;
			float RLamt, LRamt;

            gvals gval;
};

mi::mi() {  GlobalVals = &gval; }
mi::~mi() { }

void mi::MDKInit(CMachineDataInput * const pi)
{
	//AllocConsole(); freopen("CON", "w", stdout);
	
            SetOutputMode( true ); // No mono sounds
			pan_position = 100;	
}

void mi::MDKSave(CMachineDataOutput * const po) { }

void mi::Tick() {
	if (gval.panpos != 0xFF){
		pan_position = (int)gval.panpos;
		if (pan_position > 100){	//rght pan
			LRamt = (float)(pan_position - 100.0f) / 100.0f;
			RLamt = 0.0f;}
		else {
			RLamt = 1.0f - ((float)pan_position / 100.0f);
			LRamt = 0.0f;}
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

                        for( i=0; i<numsamples; i++ ) {

                                    inL = psamples[i*2];
                                    inR = psamples[i*2+1];

                                    outL = inL;
                                    outR = inR;
									
									outL = outL * (1.0f - LRamt) + (inR * RLamt);
									outR = outR * (1.0f - RLamt) + (inL * LRamt);
			
                                    psamples[i*2] = outL;
                                    psamples[i*2+1] = outR;
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
            static char txt[16];
            switch(param)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                        sprintf(txt,"%.1f", (float)value );
                        return txt;
                        break;
            case 4:
                        sprintf(txt,"%.1f%%", ((float)value / 254.0f * 100.0f) );
                        return txt;
                        break;
            default:
                        return NULL;
            }
}

#pragma optimize ("", on)

DLL_EXPORTS