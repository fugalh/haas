#include "dsp.h"
#include "haas.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14
#endif

/* data */

// lowpass filter coefficients, for the lpf in the detuner
channel left, right;

// sample rate
int fs;

// delay lines
#define MAX_DELAY (int)((45+100)*1.e-3*48000)
float left_dl_ary[MAX_DELAY+1];
float right_dl_ary[MAX_DELAY+1];

// detuner lpf delay lines
float detune_llpf1_ary[LPF_m];
float detune_llpf2_ary[LPF_m];
float detune_rlpf1_ary[LPF_m];
float detune_rlpf2_ary[LPF_m];


/* functions */

float onepole(onepole_state *s, float x);
void  pan(float inl, float inr, float *outl, float *outr);


/* implementation */

// run at the beginning or whenever samplerate changes
void haas_init(int samplerate)
{
    fs = samplerate;
    delay_state *dl;

    // delay lines
    left.delay.p = left.delay.w = left_dl_ary;
    right.delay.p = right.delay.w = right_dl_ary;

    // detuner
    detune_state *ds = &left.detune;
    dl = &ds->dl;
    dl->w = ds->ring;
    dl->p = dl->w;
    dl->m = DETUNE_RING_m-1;
    ds->offset = 0;

    ds = &right.detune;
    dl = &ds->dl;
    dl->w = ds->ring;
    dl->p = dl->w;
    dl->m = DETUNE_RING_m-1;
    ds->offset = 0;
}

// run when parameters change
void haas_config(haas_parameters p, int samplerate)
{
    if (fs != samplerate)
	haas_init(samplerate);

    // delay
    left.delay.m = (p.predelay * 1.0e-3)*fs;
    if (p.delay < 0)
	left.delay.m += (-p.delay * 1.0e-3)*fs;

    right.delay.m = (p.predelay * 1.0e-3)*fs;
    if (p.delay > 0)
	right.delay.m += (p.delay * 1.0e-3)*fs;
    

    // lpf
    left.lpf.gain = 0;
    if (p.lpf < 0)
	left.lpf.gain = -p.lpf;
    left.lpf.a = exp(-2.0 * M_PI * p.lpf_cutoff/fs);
    left.lpf.b = 1.0 - left.lpf.a;

    right.lpf.gain = 0;
    if (p.lpf > 0)
	right.lpf.gain = p.lpf;
    right.lpf.b = left.lpf.b;
    right.lpf.a = left.lpf.a;


    // pan
    left.attenuation = 0;
    if (p.pan > 0)
	left.attenuation = p.pan;	// attenuate the other side

    right.attenuation = 0;
    if (p.pan < 0)
	right.attenuation = -p.pan;

    // detune
    left.detune.delta = 1;
    if (p.detune < 0)
        left.detune.delta = 1/pow(2, -p.detune/1200);

    right.detune.delta = 1;
    if (p.detune > 0)
        right.detune.delta = 1/pow(2, p.detune/1200);

}

// the heart
void haas_run(float *inl, float *inr, 
	float *outl, float *outr,
	int frames)
{
    int i;
    float l,r;

    for (i=0; i<frames; i++)
    {
	l = inl[i];
	r = inr[i];

	l = delay(&left.delay, l);
	r = delay(&right.delay, r);

	l = detune(&left.detune, l);
	r = detune(&right.detune, r);

	l = onepole(&left.lpf, l);
	r = onepole(&right.lpf, r);

	pan(l, r, outl+i, outr+i);
    }
}

float onepole(onepole_state *lpf, float x)
{
    double d;

    d = lpf->b * (x + lpf->z * lpf->a);		// lowpass
    d = d * lpf->gain + x * (1 - lpf->gain);	// crossfade
    lpf->z = x;
    return d;
}

void pan(float inl, float inr, float *outl, float *outr)
{
    *outl = inl * (1.0-left.attenuation);
    *outr = inr * (1.0-right.attenuation);
}
