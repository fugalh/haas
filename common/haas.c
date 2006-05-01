#include "dsp.h"
#include "haas.h"
#include <math.h>

/* structs */

typedef struct {
    int quad1, quad2;	// Hz
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
// lowpass filter coefficients, for the lpf in the detuner
extern float lpf_coeffs[];
channel left, right;
int fs;		// sample rate
float left_dl_ary[(int)((45+100)*1.e-3*48000)];
float right_dl_ary[sizeof(left_dl_ary)];

/* functions */
void delay(delay_state *ldl, delay_state *rdl, 
	float inl, float inr, float *outl, float *outr);
void onepole(float inl, float inr, float *outl, float *outr);
void detune(float inl, float inr, float *outl, float *outr);
void pan(float inl, float inr, float *outl, float *outr);

/* implementation */
void config(parameters p, int samplerate)
{
    fs = samplerate;

    /* left */
    // delay
    left.delay.m = (p.predelay * 1.0e-3)*fs;
    left.delay.w = left_dl_ary;
    wrap(left.delay.m, left.delay.w, &left.delay.p);
    if (p.delay < 0)
	left.delay.m += (-p.delay * 1.0e-3)*fs;
    
    // pan
    if (p.pan < 0)
	left.attenuation = -p.pan;
    else
	left.attenuation = 0;

    // detune
    left.detune.quad2 = fs/4.0;
    if (p.detune < 0)
	left.detune.quad1 = fs/4.0 - 440.0*(pow(2,-p.detune/1200));
    else
	left.detune.quad1 = fs/4.0;

    // lpf
    left.lpf.gain = p.lpf;
    left.lpf.b = exp(-2.0 * M_PI * p.lpf_cutoff/fs);
    left.lpf.a = 1.0 - left.lpf.b;

    /* right */
    // delay
    right.delay.m = (p.predelay * 1.0e-3)*fs;
    right.delay.w = right_dl_ary;
    wrap(right.delay.m, right.delay.w, &right.delay.p);
    if (p.delay > 0)
	right.delay.m += (p.delay * 1.0e-3)*fs;
    
    // pan
    if (p.pan > 0)
	right.attenuation = p.pan;
    else
	right.attenuation = 0;

    // detune
    right.detune.quad2 = fs/4.0;
    if (p.detune > 0)
	right.detune.quad1 = fs/4.0 - 440.0*(pow(2,p.detune/1200));
    else
	right.detune.quad1 = fs/4.0;

    // lpf
    right.lpf.a = left.lpf.a;
    right.lpf.b = left.lpf.b;
    right.lpf.gain = left.lpf.gain;
}

void run(float *inl, float *inr, 
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
    if (ldl->m > 0)
    {
	*outl = *ldl->p;
	*ldl->p = inl;
	cdelay(ldl->m - 1, ldl->w, &ldl->p);
    }
    else
	*outl = inl;
    if (rdl->m > 0)
    {
	*outr = *rdl->p;
	*rdl->p = inr;
	cdelay(rdl->m - 1, rdl->w, &rdl->p);
    }
    else
	*outr = inr;
}

void onepole(float inl, float inr, float *outl, float *outr)
{
    onepole_state *lpf;

    lpf = &left.lpf;
    *outl = lpf->a * (inl + lpf->z * lpf->b);		// lowpass
    *outl = *outl * lpf->gain + inl * (1-lpf->gain);	// crossfade
    lpf->z = inl;

    lpf = &right.lpf;
    *outr = lpf->a * (inr + lpf->z * lpf->b);		// lowpass
    *outr = *outr * lpf->gain + inr * (1-lpf->gain);	// crossfade
    lpf->z = inr;
}

void detune(float inl, float inr, float *outl, float *outr)
{
    *outl = inl;	// glorified wire. TODO
    *outr = inr;
}

void pan(float inl, float inr, float *outl, float *outr)
{
    *outl = inl * (1.0-left.attenuation);
    *outr = inr * (1.0-right.attenuation);
}
