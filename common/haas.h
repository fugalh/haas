#ifndef HAAS_H
#define HAAS_H

#include <SndObj/AudioDefs.h>

class Haas
{
public:
    /// Stereo input, stereo output.
    void run(float **in, float **out, unsigned long count);
    /// Stereo input, stereo output.
    void run_adding(float **in, float **out, unsigned long count);
    void set_run_adding_gain(float);

    float delay(void);		// ms
    float balance(void);
    float detune(void);		// cents
    float lpf(void);
    float lpf_freq(void);	// KHz
    float predelay(void);	// ms

    // -45.0 to 45.0 ms
    void set_delay(float);
    // -1.0 to 1.0
    void set_balance(float);
    // -15.0 to 15.0 cents
    void set_detune(float);
    // -1.0 to 1.0
    void set_lpf(float);
    // 1.0 to 18.0 KHz
    void set_lpf_freq(float);
    // 0.0 to 100.0 ms
    void set_predelay(float);
};

#endif
