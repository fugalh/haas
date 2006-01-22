#ifndef __HAAS_VST_H
#define __HAAS_VST_H

#include <audioeffectx.h>
#include <vstplugsmacho.h>
// SndObj headers choke on SGI being defined as 0, (from vstplugsmacho.h) they
// expect it to not be defined at all.
#undef SGI
#include <SndObj/AudioDefs.h>

//-------------------------------------------------------------------------------------------------------
class Haas : public AudioEffectX
{
    public:
	Haas(audioMasterCallback audioMaster);
	~Haas();

	// Processes
	virtual void process (float **inputs, float **outputs, 
		long sampleFrames);
	virtual void processReplacing (float **inputs, float **outputs, 
		long sampleFrames);

	// Program
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);

	// Parameters
	virtual void setParameter (long index, float value);
	virtual float getParameter (long index);
	virtual void getParameterLabel (long index, char *label);
	virtual void getParameterDisplay (long index, char *text);
	virtual void getParameterName (long index, char *text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual long getVendorVersion () { return 1000; }

	virtual VstPlugCategory getPlugCategory() { return kPlugCategEffect; }

    protected:
	float delay;
	float balance;
	float detune;
	float lpf;
	float lpf_freq;
	float predelay;

	SndObj *channel1, *procobj1;

	char programName[32];
};

#endif
