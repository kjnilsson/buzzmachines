// This filter source code was created by FilterExplorer version 1.7
// (C)opyright 2002-2004 by Tobias Fleischer alias Tobybear
// This is a computer-generated file and no guarantee is given that it
// works as expected! - Use at your own risk!
// Author contact:
// web: www.tobybear.de   mail: tobybear@web.de

// Lowpass filter with resonance
// c0: cutoff frequency (0..1), 1 is Nyquist
// c1: resonance 0..1
// 
// taken from:
// "Cookbook formulae for audio EQ biquad filter coefficients"
// by Robert Bristow-Johnson  <robert@wavemechanics.com>
// (you can find this document on www.musicdsp.org)
// 
// adapted by Tobybear alias Tobias Fleischer
// www.tobybear.de
// tobybear@web.de

// Filter type: IIR, 2nd order
#define fpi2 3.141592f

inline float kd(float &val)
{
static const float anti_denormal = 1e-18f;
val += anti_denormal;
val -= anti_denormal;
return val;
}

class CookbookLowpass
{
public:
 CookbookLowpass();
 void init();
 void calc();
 inline void process(float &x);
 ~CookbookLowpass();
 float getC0();
 void setC0(float c);
 float getC1();
 void setC1(float c);
protected:
 float a0,a1,a2;
 float b1,b2;
 float t0,t1,t2;
 float c0,c1;
 float xold1,xold2;
 float yold1,yold2;
 float y;
 float samplerate;
};



CookbookLowpass::CookbookLowpass()
{
 samplerate=44100.0;
 c0=0.0;c1=0.0;
 t0=0.0;t1=0.0;t2=0.0;
 init();
}

CookbookLowpass::~CookbookLowpass()
{
}

void CookbookLowpass::init()
{
 // initialize values
 xold1=0.0;xold2=0.0;
 yold1=0.0;yold2=0.0;
 y=0;
 calc();
};

void CookbookLowpass::calc()
{
 // calculate temporary variables
 t0=cosf(fpi2*(0.99f*c0+0.01f));
 t1=sinf(fpi2*(0.99f*c0+0.01f))/(2.3f*(c1+0.001f));
 t2=1+t1;
 // calculate coefficients
 a0=(1-t0)/(2*t2);
 a1=(1-t0)/t2;
 a2=(1-t0)/(2*t2);
 b1=-2*t0/t2;
 b2=((1-t1)/t2);
};

inline void CookbookLowpass::process(float &x)
{
 // process input
 yold2=yold1;
 yold1=y;
 y=
 a2*xold2+
 a1*xold1+
 a0*x
 -b2*yold2
 -b1*yold1;
 xold2=xold1;
 xold1=x;
 x=y;
}

float CookbookLowpass::getC0()
{ return c0; }

void CookbookLowpass::setC0(float c)
{ c0=c;calc(); }

float CookbookLowpass::getC1()
{ return c1; }

void CookbookLowpass::setC1(float c)
{ c1=c;calc(); }
