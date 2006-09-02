#ifndef HAAS_VST_H
#define HAAS_VST_H

#define _CRT_SECURE_NO_DEPRECATE

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "../common/haas.h"

//-------------------------------------------------------------------------------------------------------
class Haas : public AudioEffectX
{
public:
	Haas (audioMasterCallback audioMaster);
	~Haas ();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual void setSampleRate (float sampleRate);


	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	haas_parameters params;
	char programName[kVstMaxProgNameLen+1];
};

#endif
