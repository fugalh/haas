#ifndef HAAS_H
#define HAAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dsp.h"

/* structs */
    
typedef struct {
    float predelay, delay, pan, detune, lpf, lpf_cutoff;
} haas_parameters;

typedef struct {
    float w;	// omega = 2*pi*f
    int t;
} quad_state;

typedef struct {
    quad_state quad1, quad2;
    delay_state lpf1_dl, lpf2_dl;
    int t;
} detune_state;

// one-pole IIR lowpass filter
// a = 1-b
// b = exp(-2*pi*fc/fs)
typedef struct
{
    float z;	// Z^-1
    float a,b;
    float gain;	// crossfade gain
} onepole_state;

// as in left or right channel
typedef struct {
    delay_state delay;	// predelay + haas delay
    float attenuation;	// <= 1
    detune_state detune;
    onepole_state lpf;
} channel;

/* data */
#define LPF_m 32
extern channel left, right;

/* functions */

void haas_init(int samplerate);
void haas_config(haas_parameters, int samplerate);
void haas_run(float *in_left, float *in_right, 
	float *out_left, float *out_right,
	int num_frames);

extern int fs;
#ifdef __cplusplus
}
#endif
#endif
