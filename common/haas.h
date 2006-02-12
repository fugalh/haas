#ifndef HAAS_H
#define HAAS_H

#include <SndObj/AudioDefs.h>

class Haas
{
public:
    Haas();
    ~Haas();

    /// Stereo input, stereo output.
    void run(float **in, float **out, unsigned long count);
    /// Stereo input, stereo output.
    void run_adding(float **in, float **out, unsigned long count);
    void set_run_adding_gain(float);

    float delay;	// -45.0 to 45.0 ms
    float balance;	// -1.0 to 1.0
    float detune;	// -15.0 to 15.0 cents
    float lpf;		// -1.0 to 1.0
    float lpf_freq;	// 1.0 to 18.0 KHz
    float predelay;	// 0.0 to 100.0 ms

protected:
    SndObj lchan, rchan;
    DelayLine ldelay, rdelay, lpredelay, rpredelay;
    Pitch ldetune, rdetune;
    LoPass llpf, rlpf;
};

#endif
