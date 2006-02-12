#include "haas-vst.h"

//----------------------------------------------------------------------
HaasVST::HaasVST(audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, 1, 6)  // 1 program, 6 params
{
    setNumInputs(2);     // stereo in
    setNumOutputs(2);    // stereo out
    setUniqueID('Haas'); // identify
    canMono();           // makes sense to feed both inputs with the same signal
    canProcessReplacing(); // supports both accumulating and replacing output
    strcpy(programName, "Default"); // default program name
}

//----------------------------------------------------------------------
HaasVST::~HaasVST()
{
    // noop
}

//----------------------------------------------------------------------
void HaasVST::setProgramName(char *name)
{
    strcpy (programName, name);
}

//----------------------------------------------------------------------
void HaasVST::getProgramName(char *name)
{
    strcpy (name, programName);
}

//----------------------------------------------------------------------
void HaasVST::setParameter(long index, float value)
{
    switch(index)
    {
        case 0: haas.delay = (value - .5) * 45.0;   break;
        case 1: haas.balance = (value - .5) * 2.0;  break;
        case 2: haas.detune = (value - .5) * 15.0;  break;
        case 3: haas.lpf = (value - .5) * 2.0;      break;
        case 4: haas.lpf_freq = value * 17.0 + 1.0; break;
        case 5: haas.predelay = value * 100.0;      break;
    }
}

//----------------------------------------------------------------------
float HaasVST::getParameter(long index)
{
    switch(index)
    {
        case 0:
            return .5 + haas.delay / 45.0;
        case 1:
            return .5 + haas.balance / 2.0;
        case 2:
            return .5 + haas.detune / 15.0;
        case 3:
            return .5 + haas.lpf / 2.0;
        case 4:
            return (haas.lpf_freq - 1.0) / 17.0;
        case 5:
            return haas.predelay / 100.0;
    }
    return 0.f; // bogus
}

//----------------------------------------------------------------------
void HaasVST::getParameterName(long index, char *label)
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
void HaasVST::getParameterDisplay(long index, char *text)
{
    switch (index)
    {
        case 0: // delay
            dB2string((2.0 * haas.delay - 1.0) * 45.0, text);
            break;
        case 1: // balance
            dB2string(2.0 * haas.balance - 1.0, text);
            break;
        case 2: // detune
            dB2string(2.0 * haas.detune - 1.0, text);
            break;
        case 3: // lpf
            dB2string(2.0 * haas.detune - 1.0, text);
            break;
        case 4: // lpf_freq
            dB2string(1.0 + (haas.lpf_freq * 17.0), text);
            break;
        case 5: // predelay
            dB2string(haas.predelay * 100.0, text);
            break;
    }
}

//----------------------------------------------------------------------
void HaasVST::getParameterLabel(long index, char *label)
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
bool HaasVST::getEffectName(char* name)
{
    strcpy (name, "Haas Effect");
    return true;
}

//----------------------------------------------------------------------
bool HaasVST::getProductString (char* text)
{
    strcpy (text, "Haas Product");
    return true;
}

//----------------------------------------------------------------------
bool HaasVST::getVendorString (char* text)
{
    strcpy (text, "Hans Fugal");
    return true;
}

//----------------------------------------------------------------------
void HaasVST::process (float **inputs, float **outputs, long sampleFrames)
{
    haas.run_adding(inputs, outputs, sampleFrames);
}

//----------------------------------------------------------------------
void HaasVST::processReplacing (float **inputs, float **outputs, long sampleFrames)
{
    haas.run(inputs, outputs, sampleFrames);
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
    HaasVST* effect = new HaasVST(audioMaster);
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

// vim: expandtab
