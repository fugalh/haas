#include "dsp.h"
#include "haas.h"
#include <math.h>

/* data */
// lowpass filter coefficients, for the lpf in the detuner
channel left, right;
int fs;		// sample rate
#define MAX_DELAY (int)((45+100)*1.e-3*48000)
float left_dl_ary[MAX_DELAY+1];
float right_dl_ary[MAX_DELAY+1];
float detune_llpf1_ary[LPF_m];
float detune_llpf2_ary[LPF_m];
float detune_rlpf1_ary[LPF_m];
float detune_rlpf2_ary[LPF_m];

/* functions */
void delay(delay_state *ldl, delay_state *rdl, 
	float inl, float inr, float *outl, float *outr);
void onepole(float inl, float inr, float *outl, float *outr);
void detune(float inl, float inr, float *outl, float *outr);
void pan(float inl, float inr, float *outl, float *outr);

/* implementation */
void haas_config(haas_parameters p, int samplerate)
{
    fs = samplerate;

    /* left */
    // delay
    left.delay.w = left_dl_ary;
    left.delay.m = (p.predelay * 1.0e-3)*fs;
    if (p.delay < 0)
	left.delay.m += (-p.delay * 1.0e-3)*fs;
    wrap(MAX_DELAY, left.delay.w, &left.delay.p); // in case p is 0

    
    // pan
    if (p.pan < 0)
	right.attenuation = -p.pan;
    else
	right.attenuation = 0;

    // detune
    left.detune.quad2.w = 2*M_PI*fs/4.0;
    if (p.detune < 0)
	left.detune.quad1.w = 2*M_PI*(fs/4.0 - 440.0*(pow(2,-p.detune/1200)));
    else
	left.detune.quad1 = left.detune.quad2;
    if (left.detune.lpf1_dl.w == 0)	// init detune lowpass filters
    {
	left.detune.lpf1_dl.w = detune_llpf1_ary;
	left.detune.lpf2_dl.w = detune_llpf2_ary;
	wrap(LPF_m-1, left.detune.lpf1_dl.w, &left.detune.lpf1_dl.p);
	wrap(LPF_m-1, left.detune.lpf2_dl.w, &left.detune.lpf2_dl.p);
	left.detune.lpf1_dl.m = LPF_m;
	left.detune.lpf2_dl.m = LPF_m;

	right.detune.lpf1_dl.w = detune_rlpf1_ary;
	right.detune.lpf2_dl.w = detune_rlpf2_ary;
	wrap(LPF_m-1, right.detune.lpf1_dl.w, &right.detune.lpf1_dl.p);
	wrap(LPF_m-1, right.detune.lpf2_dl.w, &right.detune.lpf2_dl.p);
	right.detune.lpf1_dl.m = LPF_m;
	right.detune.lpf2_dl.m = LPF_m;
    }

    // lpf
    if (p.lpf < 0)
	left.lpf.gain = -p.lpf;
    else
	left.lpf.gain = 0;
    left.lpf.b = exp(-2.0 * M_PI * p.lpf_cutoff/fs);
    left.lpf.a = 1.0 - left.lpf.b;

    /* right */
    // delay
    right.delay.w = right_dl_ary;
    right.delay.m = (p.predelay * 1.0e-3)*fs;
    if (p.delay > 0)
	right.delay.m += (p.delay * 1.0e-3)*fs;
    wrap(MAX_DELAY, right.delay.w, &right.delay.p); // in case p is 0
    
    // pan
    if (p.pan > 0)
	left.attenuation = p.pan;	// attenuate the other side
    else
	left.attenuation = 0;

    // detune
    right.detune.quad2.w = 2*M_PI*fs/4.0;
    if (p.detune > 0)
	right.detune.quad1.w = 2*M_PI*(fs/4.0 - 440.0*(pow(2,p.detune/1200)));
    else
	right.detune.quad1.w = right.detune.quad2.w;

    // lpf
    if (p.lpf > 0)
	right.lpf.gain = p.lpf;
    else
	right.lpf.gain = 0;
    right.lpf.a = left.lpf.a;
    right.lpf.b = left.lpf.b;
}

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
	delay(&left.delay, &right.delay, l, r, &l, &r);
	onepole(l, r, &l, &r);
	detune(l, r, &l, &r);
	pan(l, r, outl+i, outr+i);
    }
}

void delay(delay_state *ldl, delay_state *rdl, float inl, float inr, float *outl, float *outr)
{
    float *oldest;
    // left
    *ldl->p = inl;
    oldest = ldl->p + ldl->m;
    wrap(MAX_DELAY, ldl->w, &oldest);
    cdelay(MAX_DELAY, ldl->w, &ldl->p);
    *outl = *oldest;

    // right
    *rdl->p = inr;
    oldest = rdl->p + rdl->m;
    wrap(MAX_DELAY, rdl->w, &oldest);
    cdelay(MAX_DELAY, rdl->w, &rdl->p);
    *outr = *oldest;
}

void onepole(float inl, float inr, float *outl, float *outr)
{
    onepole_state *lpf;
    double d;

    lpf = &left.lpf;
    d = lpf->a * (inl + lpf->z * lpf->b);		// lowpass
    *outl = d * lpf->gain + inl * (1-lpf->gain);	// crossfade
    lpf->z = inl;

    lpf = &right.lpf;
    d = lpf->a * (inr + lpf->z * lpf->b);		// lowpass
    *outr = d * lpf->gain + inr * (1-lpf->gain);	// crossfade
    lpf->z = inr;
}

void pan(float inl, float inr, float *outl, float *outr)
{
    *outl = inl * (1.0-left.attenuation);
    *outr = inr * (1.0-right.attenuation);
}
