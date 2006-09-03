#include "haas-vst.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Haas (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Haas::Haas (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 6)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Haas');	// identify
	canProcessReplacing ();	// supports replacing output

	params.predelay = 0;
	params.delay = 0;
	params.pan = 0;
	params.detune = 0;
	params.lpf = 0;
	params.lpf_cutoff = 18000;
        haas_config(params, 48000);
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
Haas::~Haas ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Haas::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Haas::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
enum {PREDELAY, DELAY, PAN, DETUNE, LPF, LPF_CUTOFF};
void Haas::setParameter (VstInt32 index, float value)
{
    float value2 = (value - 0.5) * 2;
    switch (index)
    {
	case PREDELAY:
	    params.predelay = value * 100; // ms
	    break;
	case DELAY:
	    params.delay = -value2 * 45; // ms
	    break;
	case PAN:
	    params.pan = value2;
	    break;
	case DETUNE:
	    params.detune = value2 * 100;
	    break;
	case LPF:
	    params.lpf = -value2;
	    break;
	case LPF_CUTOFF:
	    params.lpf_cutoff = pow(18000, (1.0-value)); // Hz
	    break;
    }
    haas_config(params, (int)sampleRate);
}

//-----------------------------------------------------------------------------------------
void Haas::setSampleRate (float sampleRate)
{
    AudioEffectX::setSampleRate(sampleRate);
    haas_config(params,(int)sampleRate);
}

//-----------------------------------------------------------------------------------------
float Haas::getParameter (VstInt32 index)
{
    float value;
    switch (index)
    {
	case PREDELAY:
	    value = params.predelay / 100; // ms
	    break;
	case DELAY:
	    value = 0.5 - params.delay / (2*45); // ms
	    break;
	case PAN:
	    value = 0.5 + params.pan / 2;
	    break;
	case DETUNE:
	    value = 0.5 + params.detune / (2*100);
	    break;
	case LPF:
	    value = 0.5 - params.lpf / 2 ;
	    break;
	case LPF_CUTOFF:
	    value = 1.0 - log(params.lpf_cutoff)/log(18000); // Hz
	    break;
    }
    return value;
}

//-----------------------------------------------------------------------------------------
void Haas::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
	case PREDELAY:
	    vst_strncpy (label, "Predelay", kVstMaxParamStrLen);
	    break;
	case DELAY:
	    vst_strncpy (label, "Delay", kVstMaxParamStrLen);
	    break;
	case PAN:
	    vst_strncpy (label, "Pan", kVstMaxParamStrLen);
	    break;
	case DETUNE:
	    vst_strncpy (label, "Detune", kVstMaxParamStrLen);
	    break;
	case LPF:
	    vst_strncpy (label, "LPF xfade", kVstMaxParamStrLen);
	    break;
	case LPF_CUTOFF:
	    vst_strncpy (label, "LPF cut", kVstMaxParamStrLen);
	    break;
    }
}

//-----------------------------------------------------------------------------------------
void Haas::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
	case PREDELAY:
	    float2string (params.predelay, text, kVstMaxParamStrLen);
	    break;
	case DELAY:
	    float2string (fabs(params.delay), text, kVstMaxParamStrLen);
	    break;
	case PAN:
	    float2string (fabs(params.pan), text, kVstMaxParamStrLen);
	    break;
	case DETUNE:
	    float2string (fabs(params.detune), text, kVstMaxParamStrLen);
	    break;
	case LPF:
	    float2string (fabs(params.lpf), text, kVstMaxParamStrLen);
	    break;
	case LPF_CUTOFF:
	    float2string (params.lpf_cutoff, text, kVstMaxParamStrLen);
	    break;
    }
}

//-----------------------------------------------------------------------------------------
void Haas::getParameterLabel (VstInt32 index, char* label)
{
    switch (index)
    {
	case PREDELAY:
	    vst_strncpy (label, "ms", kVstMaxParamStrLen);
	    break;
	case DELAY:
	    vst_strncpy (label, "ms", kVstMaxParamStrLen);
	    break;
	case PAN:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
	    if (params.pan < 0)
		vst_strncpy (label, "L", kVstMaxParamStrLen);
	    if (params.pan > 0)
		vst_strncpy (label, "R", kVstMaxParamStrLen);
	    break;
	case DETUNE:
	    vst_strncpy (label, "cents", kVstMaxParamStrLen);
	    break;
	case LPF:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
	    if (params.lpf > 0)
		vst_strncpy (label, "L", kVstMaxParamStrLen);
	    if (params.lpf < 0)
		vst_strncpy (label, "R", kVstMaxParamStrLen);
	    break;
	case LPF_CUTOFF:
	    vst_strncpy (label, "Hz", kVstMaxParamStrLen);
	    break;
    }
}

//------------------------------------------------------------------------
bool Haas::getEffectName (char* name)
{
	vst_strncpy (name, "Haas", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Haas::getProductString (char* text)
{
	vst_strncpy (text, "Haas", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Haas::getVendorString (char* text)
{
	vst_strncpy (text, "Hans Fugal", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Haas::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void Haas::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    haas_run(inputs[0], inputs[1], outputs[0], outputs[1], sampleFrames);
}
