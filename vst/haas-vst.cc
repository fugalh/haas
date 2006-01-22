#include "haas-vst.h"

//----------------------------------------------------------------------
Haas::Haas(audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, 1, 6),  // 1 program, 6 params
      delay(0.5f),
      balance(0.5f),
      detune(0.5f),
      lpf(0.5f),
      lpf_freq(0.5f),
      predelay(0.f)
{
    setNumInputs(2);     // stereo in
    setNumOutputs(2);    // stereo out
    setUniqueID('Haas'); // identify
    canMono();           // makes sense to feed both inputs with the same signal
    canProcessReplacing(); // supports both accumulating and replacing output
    strcpy(programName, "Default"); // default program name
}

//----------------------------------------------------------------------
Haas::~Haas()
{
    // noop
}

//----------------------------------------------------------------------
void Haas::setProgramName(char *name)
{
    strcpy (programName, name);
}

//----------------------------------------------------------------------
void Haas::getProgramName(char *name)
{
    strcpy (name, programName);
}

//----------------------------------------------------------------------
void Haas::setParameter(long index, float value)
{
	// TODO
}

//----------------------------------------------------------------------
float Haas::getParameter(long index)
{
    switch(index)
    {
	case 0:
	    return delay;
	case 1:
	    return balance;
	case 2:
	    return detune;
	case 3:
	    return lpf;
	case 4:
	    return lpf_freq;
	case 5:
	    return predelay;
    }
    return 0.f;	// bogus
}

//----------------------------------------------------------------------
void Haas::getParameterName(long index, char *label)
{
    switch (index)
    {
	case 0:
	    strcpy(label, "Delay");
	    break;
	case 1:
	    strcpy(label, "Pan");
	    break;
	case 2:
	    strcpy(label, "Detune");
	    break;
	case 3:
	    strcpy(label, "Low-pass Filter");
	    break;
	case 4:
	    strcpy(label, "Low-pass Filter Frequency");
	    break;
	case 5:
	    strcpy(label, "Pre-delay");
	    break;
    }
}

//----------------------------------------------------------------------
void Haas::getParameterDisplay(long index, char *text)
{
    switch (index)
    {
	case 0:	// delay
	    dB2string((2.0 * delay - 1.0) * 45.0, text);
	    break;
	case 1:	// balance
	    dB2string(2.0 * balance - 1.0, text);
	    break;
	case 2:	// detune
	    dB2string(2.0 * detune - 1.0, text);
	    break;
	case 3:	// lpf
	    dB2string(2.0 * detune - 1.0, text);
	    break;
	case 4:	// lpf_freq
	    dB2string(1.0 + (lpf_freq * 17.0), text);
	    break;
	case 5:	// predelay
	    dB2string(predelay * 100.0, text);
	    break;
    }
}

//----------------------------------------------------------------------
void Haas::getParameterLabel(long index, char *label)
{
    switch (index)
    {
	case 0:
	    strcpy(label, "ms");
	    break;
	case 1:
	    label[0] = 0;
	    break;
	case 2:
	    strcpy(label, "cents");
	    break;
	case 3:
	    label[0] = 0;
	    break;
	case 4:
	    strcpy(label, "KHz");
	    break;
	case 5:
	    strcpy(label, "ms");
	    break;
    }
}

//----------------------------------------------------------------------
bool Haas::getEffectName(char* name)
{
    strcpy (name, "Haas Effect");
    return true;
}

//----------------------------------------------------------------------
bool Haas::getProductString (char* text)
{
    strcpy (text, "Haas Product");
    return true;
}

//----------------------------------------------------------------------
bool Haas::getVendorString (char* text)
{
    strcpy (text, "Fugal Computing");
    return true;
}

//----------------------------------------------------------------------
// TODO
void Haas::process (float **inputs, float **outputs, long sampleFrames)
{
    float *in1  =  inputs[0];
    float *in2  =  inputs[1];
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    float fGain = 1.0;
    while (--sampleFrames >= 0)
    {
        (*out1++) += (*in1++) * fGain;	// accumulating
        (*out2++) += (*in2++) * fGain;
    }
}

//----------------------------------------------------------------------
// TODO
void Haas::processReplacing (float **inputs, float **outputs, long sampleFrames)
{
    float *in1  =  inputs[0];
    float *in2  =  inputs[1];
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    float fGain = 1.0;
    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * fGain;	// replacing
        (*out2++) = (*in2++) * fGain;
    }
}


//----------------------------------------------------------------------
// Prototype of the export function main
//----------------------------------------------------------------------
#if BEOS
#define main main_plugin
extern "C" __declspec(dllexport) AEffect *main_plugin(audioMasterCallback audioMaster);

#elif MACX
#define main main_macho
extern "C" AEffect *main_macho(audioMasterCallback audioMaster);

#else
AEffect *main(audioMasterCallback audioMaster);
#endif

//----------------------------------------------------------------------
AEffect *main(audioMasterCallback audioMaster)
{
    // Get VST Version
    if (!audioMaster(0, audioMasterVersion, 0, 0, 0, 0))
	return 0;  // old version

    // Create the AudioEffect
    Haas* effect = new Haas (audioMaster);
    if (!effect)
		return 0;

    return effect->getAeffect();
}

#if MAC
#pragma export off
#endif

//----------------------------------------------------------------------
#if WIN32
#include <windows.h>
void* hInstance;
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
    hInstance = hInst;
    return 1;
}
#endif
