#include "../mdk/mdk.h"
#include <windows.h>
#include <math.h>
#include <float.h>

#pragma optimize ("awy", on) 

CMachineParameter const paraMode =
{
	pt_byte,
	"Mode",
	"Mode",
	0,
	4,
	0xFF,
	MPF_STATE,
	0,
};
CMachineParameter const paraPGain =
{
	pt_byte,
	"Pre Gain",
	"Pre Gain",
	0,
	200,
	0xFF,
	MPF_STATE,
	72,
};
CMachineParameter const paraHThreshold =
{
	pt_word,
	"Higher Threshold",
	"High Threshold",
	0,
	0xFFFE,
	0xFFFF,
	MPF_STATE,
	0xFFFE,
};
CMachineParameter const paraLThreshold =
{
	pt_word,
	"Lower Threshold",
	"Low Threshold",
	0,
	0xFFFE,
	0xFFFF,
	MPF_STATE,
	0,
};

CMachineParameter const paraLPCutoff =
{
            pt_word,                        // Parameter data type
            "Dullness",                        // Parameter name as its shown in the parameter
                                                // window
            "LoPass Filter Cutoff",            // Parameter description as its shown in
                                                //the pattern view's statusbar
            0,                                    // Minimum value
            22051,                        // Maximum value
            0xFFFF,                        // Novalue, this value means "nothing
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            22051                        // the default slider value
};
CMachineParameter const paraHPCutoff =
{
            pt_word,                        // Parameter data type
            "Brightness",                        // Parameter name as its shown in the parameter 
                                                // window
            "HiPass Filter Cutoff",		// Parameter description as its shown in the 
                                                // pattern view's statusbar
            0,                                    // Minimum value
            22050,                        // Maximum value
            0xFFFF,                        // Novalue, this value means "nothing 
                                                // happened" in the mi::Tick procedure
            MPF_STATE,                        // Parameter options, MPF_STATE makes it 
                                                // appears as a slider
            0                        // the default slider value
};
CMachineParameter const paraOGain =
{
	pt_byte,
	"Post Gain",
	"Post Gain",
	0,
	200,
	0xFF,
	MPF_STATE,
	144,
};


CMachineParameter const *pParameters[] = { 
	&paraMode,
	&paraPGain,
	&paraHThreshold,
	&paraLThreshold,
	&paraLPCutoff,
	&paraHPCutoff,
	&paraOGain,
	
};



CMachineAttribute const *pAttributes[] = { NULL };

#pragma pack(1)		

class gvals
{
public:
	byte Mode;
	byte PGain;
	word HThreshold;
	word LThreshold;
	word lpcutoff;
	word hpcutoff;
	word OGain;
	

};

#pragma pack()


CMachineInfo const MacInfo = 
{
	MT_EFFECT,
	MI_VERSION,	
	MIF_DOES_INPUT_MIXING,
	0,										// min tracks
	0,										// max tracks
	7,										// numGlobalParameters
	0,										// numTrackParameters
	pParameters,
	0,
	pAttributes,
	"Intoxicat Fold",								// name
	"Fold",											// short name
	"Karl Nilsson",								// author
	"&About..."
};

class mi;

class miex : public CMDKMachineInterfaceEx 
{
	/*
public:
	virtual void AddInput(char const *macname, bool stereo);	// called when input is added to a machine
	virtual void DeleteInput(char const *macename);			
	virtual void RenameInput(char const *macoldname, char const *macnewname); 
	virtual void SetInputChannels(char const *macname, bool stereo); //{}
	virtual void Input(float *psamples, int numsamples, float amp); // if MIX_DOES_INPUT_MIXING
	virtual bool HandleInput(int index, int amp, int pan); //{ return false; }
public:

	mi *pmi;
	*/
};

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
	virtual char const *DescribeParam(int const param) { return NULL; }
	virtual float const SliderToVolume(float sliderpos);
	virtual float const VolumeToSlider(float sliderpos);
	virtual float const VolumeToDB(float volume);
	virtual float const DBtoVolume(float volumedb);

public:
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void OutputModeChanged(bool stereo) {}

	virtual void LPFilter();
	virtual void HPFilter();
public:
	miex ex;

public:

	gvals gval;


	float HThreshold;
	float LThreshold;
	float NegHThreshold;
	float NegLThreshold;
	float NegLThreshold2;
	float PGain;
	float lastPGain;
	float OGain;
	float pvol;
	float ovol;
	int SelMode;
	int holdval;
	bool isfilteroff;
	bool ishpfilteroff;

	float id, id2;

public:
	            // Filter stuff
            float param_lpcutoff, param_hpcutoff;
            float filtCoefTab[5];
            float lx1, lx2, ly1, ly2; // Left sample history
            float rx1, rx2, ry1, ry2; // Right sample history

			float p, hp;
};
/*
void miex::AddInput(char const *macname, bool stereo) { }
void miex::DeleteInput(char const *macename) { }
void miex::RenameInput(char const *macoldname, char const *macnewname) { }
void miex::SetInputChannels(char const *macname, bool stereo) { }
void miex::Input(float *psamples, int numsamples, float amp) {
	int i;
	for (i = 0; i < numsamples; i++) {
		pmi->thearray[i] = pmi->thearray[i] + (((pmi->leftgain * *psamples++) + (pmi->rightgain * *psamples++)) / 2.0f) * amp;
	}
}

*/
//bool miex::HandleInput(int index, int amp, int pan) { return false; }

mi::mi() {  GlobalVals = &gval; }
mi::~mi() { }

void mi::MDKInit(CMachineDataInput * const pi)
{
	SetOutputMode( true );	//	If true, the MDKWork will never be called, meaning that Buzz will convert a mono signal to
							//	stereo itself and call MDKWorkStereo insted.
							//	If false, MDKWork will be called in mono cases, and the output should be mono
	PGain = 1;
	pvol = 1;
	ovol = 1;
	lastPGain = 1;
	OGain = 1;
	HThreshold = 0xFFFE;
	LThreshold = 0;
	NegHThreshold = -0xFFFE;
	NegLThreshold = 0;
	NegLThreshold2 = 0;
	SelMode = 0;
	holdval = 0;
	isfilteroff = true;
	ishpfilteroff = true;
	//denormal test
	id = (float)1.0E-20;
	id2 = (float)1.0E-20;
	
	LPFilter();
	HPFilter();
    lx1 = lx2 = ly1 = ly2 = 0.0f;
    rx1 = rx2 = ry1 = ry2 = 0.0f;
	p = 0.0f;
	hp = 0.0f;
};

void mi::MDKSave(CMachineDataOutput * const po) { }

void mi::Tick() {
	
	if (gval.Mode != 0xFF) {
		 SelMode = (int)gval.Mode;
		 //DescribeValue(int const param, int const value);
	 }
	if (gval.PGain != 0xFF) PGain = (float)gval.PGain;	{
		pvol = PGain / 100.0f;
		pvol = mi::SliderToVolume(pvol);
	}
	
	if (gval.HThreshold != 0xFFFF)
	 {
		 HThreshold = (float)gval.HThreshold;
		 NegHThreshold = 0 - HThreshold;
		 //AGain = (1 + (HThreshold / 0xFFFE));

	}
	if (gval.LThreshold != 0xFFFF)
	 {
		 LThreshold = (float)gval.LThreshold;
		 NegLThreshold = 0 - LThreshold;
		 NegLThreshold2 = 0 - (0xFFFE - LThreshold);
		 //AGain = (1 + (HThreshold / 0xFFFE));

	}
	
	 if (gval.OGain != 0xFF) OGain = (float)gval.OGain; {
		ovol = OGain / 200.0f;
		ovol = mi::SliderToVolume(ovol);
	}



	  if (gval.lpcutoff != 0xFFFF) {
		if ((float)gval.lpcutoff > 22050.0f) isfilteroff = true;
		else isfilteroff = false;
        param_lpcutoff = (float)pow(((float)gval.lpcutoff / 22050.0f), 2) * 22050.0f;
		//param_lpcutoff = (float)gval.lpcutoff;
        LPFilter();
     };

	  if (gval.hpcutoff != 0xFFFF) {
		if ((float)gval.hpcutoff < 1.0f) ishpfilteroff = true;
		else ishpfilteroff = false;
        param_hpcutoff = powf(((float)gval.hpcutoff / 22050.0f), 2) * 22050.0f;
        HPFilter();
     };
}


bool mi::MDKWork(float *psamples, int numsamples, int const mode)
{

	return false;
}

void mi::LPFilter() {
	float x;
	
	if (param_lpcutoff < 20.0f) { param_lpcutoff = 20.0f; };
    if (param_lpcutoff > 22000.0f) { param_lpcutoff = 22000.0f; };

	x = 2.0f * (float)PI * param_lpcutoff/pMasterInfo->SamplesPerSec;
  	p = (float)((2-cos(x)) - sqrt(pow((2-cosf(x)),2) - 1));
	
}

void mi::HPFilter() {
	float x;
	
	if (param_hpcutoff < 20.0f) { param_hpcutoff = 20.0f; };
    if (param_hpcutoff > 22000.0f) { param_hpcutoff = 22000.0f; };

	x = 2.0f * (float)PI * param_hpcutoff/pMasterInfo->SamplesPerSec;
  	hp = (2+cosf(x)) - sqrtf(powf((2+cosf(x)),2) - 1);
	
}


bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{

	

	if (mode==WM_WRITE)
		return false;
	if (mode==WM_NOIO)
		return false;
	if (mode==WM_READ)		// <thru>
		return true;

	int i, div;
	float inL, inR, outL, outR, finL, finR, Range, NegRange, remain;
	float temp_y;

	for (i = 0; i < numsamples; i++) {

		inL = psamples[2*i];
		inR = psamples[2*i+1];
		
		//if (PGain != lastPGain) //dont wanna call stuff unless needed.
		//{
		/*
		pvol = PGain / 100.0f;
		pvol = mi::SliderToVolume(pvol);
		lastPGain = PGain;
		//}
		*/
		//pvol = PGain;

		inL = inL * pvol;
		inR = inR * pvol;

        outL = inL;
        outR = inR;


		switch (SelMode)
		{
		case 0:  //Farl
			//work out the range of the thresh
			Range =  HThreshold + (0xFFFE - LThreshold);
			NegRange = 0 - Range;
			//check if input is outside of range
			if (outL > HThreshold || outL < NegLThreshold2)
			{
				if (outL > HThreshold){
				//work out the div rate and convert to integer
					div = (int)((inL - HThreshold) / Range);
					//ugly float modulus
					remain = (inL - HThreshold) - (Range * div);
				
					if ((div % 2) == 0)
						outL = HThreshold - remain;
					else 
						outL = NegLThreshold2 + remain;
				}

				else if (outL < NegLThreshold2){
					div = (int)((inL - NegLThreshold2) / Range);
					//ugly float modulus
					remain = (inL - NegLThreshold2) - (Range * div);	
					if ((div % 2) == 0)
						outL = NegLThreshold2 - remain;
					else outL = HThreshold + remain;
				}

			}
			
			if (outR > HThreshold || outR < NegLThreshold2)
			{
				if (outR > HThreshold){
				//work out the div rate and convert to integer
					div = (int)((inR - HThreshold) / Range);
					//ugly float modulus
					remain = (inR - HThreshold) - (Range * div);
				
					if ((div % 2) == 0)
						outR = HThreshold - remain;
					else 
						outR = NegLThreshold2 + remain;
				}

				else if (outR < NegLThreshold2){
					div = (int)((inR - NegLThreshold2) / Range);
					//ugly float modulus
					remain = (inR - NegLThreshold2) - (Range * div);	
					if ((div % 2) == 0)
						outR = NegLThreshold2 - remain;
					else outR = HThreshold + remain;
				}

			}


				break;

		case 1:   // the devil's ears...
			//work out the range of the thresh
				Range = HThreshold - LThreshold;
				NegRange = NegHThreshold + LThreshold;

				// if sample is positive or negtive
				if (outL > HThreshold){
					outL = HThreshold - (inL - HThreshold);
					//work out the div rate and convert to integer
					div = (int)((inL - HThreshold) / Range);
					//ugly float modulus
					remain = (inL - HThreshold) - (Range * div);
					if ((div % 2) == 0)
						outL = HThreshold - remain;
					else 
						outL = LThreshold + remain;
				}

				if (outL < NegHThreshold){
					outL = NegHThreshold - (inL + HThreshold); //should be ok
					//work out the div rate and convert to integer
					div = (int)((inL + HThreshold) / NegRange);
					//ugly float modulus
					remain = (inL + HThreshold) - (NegRange * div);
					if ((div % 2) == 0)
						outL = NegHThreshold - remain;
					else 
						outL = NegLThreshold + remain;
				}

				if (outR > HThreshold){
					outR = HThreshold - (inR - HThreshold);
					//work out the div rate and convert to integer
					div = (int)((inR - HThreshold) / NegRange);
					//ugly float modulus
					remain = (inR - HThreshold) - (NegRange * div);
					if ((div % 2) == 0)
						outR = HThreshold - remain;
					else 
						outR = LThreshold + remain;
				}
				
				if (outR < NegHThreshold){	
					outR = NegHThreshold - (inR + HThreshold);
					//work out the div rate and convert to integer
					div = (int)((inR + HThreshold) / NegRange);
					//ugly float modulus
					remain = (inR + HThreshold) - (NegRange * div);
					if ((div % 2) == 0)
						outR = NegHThreshold - remain;
					else 
						outR = NegLThreshold + remain;
				}

				break;
			
		case 2: //wrap
					//work out the range of the thresh
			Range =  HThreshold + (0xFFFE - LThreshold);
			NegRange = 0 - Range;
			//check if input is outside of range
			if (outL > HThreshold || outL < NegLThreshold2)
			{
				if (outL > HThreshold){
				//work out the div rate and convert to integer
					div = (int)((inL - HThreshold) / Range);
					//ugly float modulus
					remain = (inL - HThreshold) - (Range * div);
				
					if ((div % 2) == 0)
						outL = NegLThreshold2 + remain;
					else 
						outL = HThreshold - remain;
				}

				else if (outL < NegLThreshold2){
					div = (int)((inL - NegLThreshold2) / Range);
					//ugly float modulus
					remain = (inL - NegLThreshold2) - (Range * div);	
					if ((div % 2) == 0)
						outL = HThreshold + remain;
					else outL = NegLThreshold2 - remain;
				}

			}

			if (outR > HThreshold || outR < NegLThreshold2)
			{
				if (outR > HThreshold){
				//work out the div rate and convert to integer
					div = (int)((inR - HThreshold) / Range);
					//ugly float modulus
					remain = (inR - HThreshold) - (Range * div);
				
					if ((div % 2) == 0)
						outR = NegLThreshold2 + remain;
					else 
						outR = HThreshold - remain;
				}

				else if (outR < NegLThreshold2){
					div = (int)((inR - NegLThreshold2) / Range);
					//ugly float modulus
					remain = (inR - NegLThreshold2) - (Range * div);	
					if ((div % 2) == 0)
						outR = HThreshold + remain;
					else outR = NegLThreshold2 - remain;
				}

			}


				break;

		case 3:  //Egg
			//work out the range of the thresh
			Range =  HThreshold + (0xFFFE - LThreshold);
			NegRange = 0 - Range;
			//check if input is outside of range
			if (outL > HThreshold || outL < NegLThreshold2)
			{
				if (outL > HThreshold){
				//work out the div rate and convert to integer
					div = (int)(((inL - HThreshold)*2) / Range);
					//ugly float modulus
					remain = ((inL - HThreshold)*2) - (Range * div);
				
					if ((div % 2) == 0)
						outL = HThreshold - remain;
					else 
						outL = NegLThreshold2 + remain;
				}

				else if (outL < NegLThreshold2){
					div = (int)(((inL - NegLThreshold2)*2) / Range);
					//ugly float modulus
					remain = ((inL - NegLThreshold2)*2) - (Range * div);	
					if ((div % 2) == 0)
						outL = NegLThreshold2 - remain;
					else outL = HThreshold + remain;
				}

			}

			if (outR > HThreshold || outR < NegLThreshold2)
			{
				if (outR > HThreshold){
				//work out the div rate and convert to integer
					div = (int)((inR - HThreshold)*2 / Range);
					//ugly float modulus
					remain = (inR - HThreshold)*2 - (Range * div);
				
					if ((div % 2) == 0)
						outR = HThreshold - remain;
					else 
						outR = NegLThreshold2 + remain;
				}

				else if (outR < NegLThreshold2){
					div = (int)((inR - NegLThreshold2)*2 / Range);
					//ugly float modulus
					remain = (inR - NegLThreshold2)*2 - (Range * div);	
					if ((div % 2) == 0)
						outR = NegLThreshold2 - remain;
					else outR = HThreshold + remain;
				}

			}

			break;
		case 4:
			{
			if (outL > HThreshold) outL = HThreshold;
			if (outL < NegLThreshold2) outL = NegLThreshold2;
			if (outR > HThreshold) outR = HThreshold;
			if (outR < NegLThreshold2) outR = NegLThreshold2;

			break;
			}
		}
		
		finL = outL; finR = outR;
		
		if (!isfilteroff)
		{
			//left LP
			temp_y = (1-p)*finL + p*lx1;
			lx1 = temp_y; outL = temp_y;
			//right LP
			temp_y = (1-p)*finR + p*rx1;
			rx1 = temp_y; outR = temp_y;

			//id;
		}

		finL = outL; finR = outR;

		if (!ishpfilteroff)
		{
			//left HP
			temp_y = (hp-1)*finL - hp*lx2;
			lx2 = temp_y; outL = temp_y;
			//right HP
			temp_y = (hp-1)*finR - hp*rx2;
			rx2 = temp_y; outR = temp_y;

			//id2;
		}
		//id = -id;
		psamples[2*i] = outL * ovol;
		psamples[2*i+1] = outR * ovol;

	}

	return true;
};

void mi::Command(int const i)
{
	switch (i)
	{
	case 0:
		MessageBox(NULL,"Intoxicat Fold\n\n\nFold your hands child you walk like a peasant","About Fold",MB_OK|MB_SYSTEMMODAL);
		break;
	default:
		break;
	}
}
char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	float translatedvolume = 0.0f;
	float translatedvolume2 = 0.0f;

	switch(param)
	{
	case 0:
		{
		if (value == 0) sprintf(txt,"Farl");
		if (value == 1) sprintf(txt,"Devil Ears");
		if (value == 2) sprintf(txt,"Wrap");
		if (value == 3) sprintf(txt,"Egg");
		if (value == 4) sprintf(txt,"Clip");
		return txt;
		holdval = value;
		break;
		}
	case 1:
		{
		translatedvolume = (float)value / 100.0f;
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
		}
	case 2:
		return NULL;
	case 3: 
		{
		sprintf(txt,"%i/%i", (value - 0xFFFE),value);

		return txt;
		break;
		}
	case 4:
		if (value <= 22050)
		sprintf(txt,"%.1f", ((float)pow((value / 22050.0f), 2) * 22050.0f));
		else
		sprintf(txt,"Off");
        return txt;
        break;
	case 5:
		if (value >= 1)
		sprintf(txt,"%.1f", ((float)pow((value / 22050.0f), 2) * 22050.0f));
		else
		sprintf(txt,"Off");
        return txt;
        break;
	case 6:
		{
		translatedvolume2 = (float)value / 200.0f;
		translatedvolume2 = mi::VolumeToDB(mi::SliderToVolume(translatedvolume2));
		if (translatedvolume>-1000)
		{
			sprintf(txt, "%.1f dB", translatedvolume2 );
		} else
		{
			sprintf(txt, "-inf dB");
		}
		return txt;
		break;
		}
	default:
		return NULL;
	}
}		


#pragma optimize ("", on) 
// my fancy conversions

float const mi::SliderToVolume(float sliderpos)		// by Joachim
{
	float x = 0.0f;
	x = powf(sliderpos, 3.5f)*3.1748f;
	if (sliderpos<=0)  x=0.0f;
	return x;
}

float const mi::VolumeToSlider(float volume)		// by Joachim
{
	float x = 0.0f;
	if (x>0)  x = powf(volume/3.1748f, 1/3.5f);
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

float const mi::DBtoVolume(float volumedb)			// by Joachim
{
	float x = 0.0f;
	if (volumedb>0.0f)
	{
		x = logf(volumedb)/logf(2.0f)*6.0f;
	}
	return x;
}
DLL_EXPORTS

