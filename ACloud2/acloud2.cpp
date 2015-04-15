#define MACHINE_NAME "saw and square"
#define AUTHOR "jmmcd"
#define DEBUG_STRING "(debug build)"
#ifdef _DEBUG
#define FULL_NAME AUTHOR " " MACHINE_NAME " " DEBUG_STRING
#else
#define FULL_NAME AUTHOR " " MACHINE_NAME
#endif
#define MACHINE_VERSION "0.1"
#define ABOUT_TEXT FULL_NAME " " MACHINE_VERSION \
		"\nby Karl Nilsson" \
		"\nkjnilsson@gmail.com" \
		"\nwww.intoxicat.co.uk"

#define TRACK_LIMIT 8


#include <windows.h>
#include <math.h>
#include "../mdk/mdk.h"
#include "CGrain.h"


// does this do anything?
#pragma optimize ("awy", on) 


CMachineParameter const paraNote = 
{
    pt_note, // parameter data type
	"Note", // short parameter name
	"Note", // parameter description
	NOTE_MIN, // Minimum value
	NOTE_MAX, // Maximum value
	NOTE_NO, // "Nothing happened" value
    0, // parameter options, MPF_STATE makes it appear as a slider?
    0  // the default slider value
};

CMachineParameter const paraVeloc = 
{
    pt_byte, // parameter data type
	"Vel", // short parameter name
	"Velocity", // parameter description
	0x00, // Minimum value
	0xFE, // Maximum value
	0xFF, // "Nothing happened" value
    0, // Parameter options
    0  // the default slider value
};




CMachineAttribute const *pAttributes[] = {NULL}; // ie no attributes

// name the parameters like this: paraName
CMachineParameter const *pParameters[] = 
{
	&paraNote,
	&paraVeloc
};

// the machine information thing
CMachineInfo const MacInfo = 
{
    MT_GENERATOR, // type
	MI_VERSION, 
	0, // flags
	1, // min tracks
    8, // max tracks
    0, // numGlobalParameters
    2, // numTrackParameters
    pParameters,
    0, // number of attributes
    pAttributes,
	FULL_NAME, // name
    MACHINE_NAME, // short name
    AUTHOR, // author
    "About " MACHINE_NAME // first special right-click menu item
};

// i don't understand the pragmas, but let's leave them in for now.
#pragma pack(1)

// gvals contains global parameter values
class gvals 
{
public:
	
};

// tvals contains track parameter values
class tvals 
{
public:
    byte Note;byte Veloc;
};	

// avals contains attribute values
class avals 
{
};

// i guess this means "stop doing whatever pack() has been doing since 15 lines ago"
#pragma pack()

class miex : public CMDKMachineInterfaceEx{};
// mi inherits from CMachineInterface
class mi: public CMDKMachineInterface 
{
public:
	// buzz needs to know about these methods
    mi();
    virtual ~mi();   
    virtual void MDKInit(CMachineDataInput * const pi);
    virtual void Tick();
    virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
	virtual void MDKSave(CMachineDataOutput * const po) {};
    virtual void SetNumTracks(int const n) {} // unimplemented since max tracks is 1.
    virtual void Stop();
    virtual char const *mi::DescribeValue(int const param, int const value);
	virtual void AttributesChanged();
	virtual void Command(int const command);
	virtual void OutputModeChanged(bool stereo) {}
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	
	CMachine *ThisMachine;
	
	miex ex;

private:
	// these variables are only used within mi
	


    avals aval; // attribute values
    gvals gval; // global machine parameter values
    tvals tval[TRACK_LIMIT]; // track parameter values

public:
	
};

// what does this do, and can it be somewhere else?
DLL_EXPORTS

// buzz calls this to fill in the descriptions on the right of the parameter window
char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[16];

	switch (param) {
	default:
	case 0:
       	break; 
	}
	return txt;
}

// buzz calls this with command == 0 when user selects our first special item
// from the machine right-click menu.
void mi::Command(int const command) {
	switch(command)
	{
	case 0:
		MessageBox(NULL, // dunno what this is
					ABOUT_TEXT, // message text
					"About " MACHINE_NAME, // window title
					MB_OK|MB_SYSTEMMODAL); // an ok button?
		break;
	default:
		break;
	}
}

// constructor
mi::mi() 
{
	// i think GlobalVals, TrackVals, and AttrVals exist "up in buzz" itself,
	// so we tell buzz where we're keeping the values
    GlobalVals = &gval;
    TrackVals = tval;
    AttrVals = (int *)&aval;
}


mi::~mi() {}


void mi::MDKInit(CMachineDataInput * const pi) 
{  	
	ThisMachine = pCB->GetThisMachine();
	pCB->SetnumOutputChannels(ThisMachine,2);


}


void mi::AttributesChanged() {}


void mi::Stop() 
{

}

// buzz calls Tick() once per tick: use it to check for new notes or data in the pattern
void mi::Tick() 
{
	/*
	// and check for track changes (only in track 0 for now!)
	if (tval[0].Note != paraNote.NoValue) 
	{
            int note = ((tval[0].Note >> 4) * 12) + (tval[0].Note & 0x0f) - 1;
			// frequency formula
           float frequency = CZERO_FREQ * (float) pow(2.0f, ((float) note) / 12.0f);		
			rhod.noteOn(frequency, 1);

		if(tval[0].Note == NOTE_OFF) 
		{
			
		} 
		note_on = true;

    }
	*/
}

// buzz calls this "often": it passes in a pointer to some samples, and you assign
// them values. if your machine is a generator you can ignore the mode variable.
// your output samples should be in the range -ZERODBDFS TO +ZERODBDFS
bool mi::MDKWork(float *psamples, int numsamples, int const mode) {return false;}


bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode) 
{
    

    for (int i = 0; i < numsamples; i++) 
	{
		

    }

    return false;
    
}
