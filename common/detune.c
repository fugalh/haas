#include "haas.h"
#include <math.h>

static float lpf_coeffs[];

void detune(float inl, float inr, float *outl, float *outr)
{
    delay_state *lpf1_dl, *lpf2_dl;    double a,b;
    int i;

    // short circuit
    *outl = inl;
    *outr = inr;
    return;

    /* left */
    lpf1_dl = &left.detune.lpf1_dl;
    lpf2_dl = &left.detune.lpf2_dl;
    a = b = inl;

    // first stage (ring with quad1)
    a *= sin(left.detune.quad1.w * left.detune.quad1.t);
    b *= cos(left.detune.quad1.w * left.detune.quad1.t++);

    // second stage (lowpass filters)
    *lpf1_dl->p = a;
    *lpf2_dl->p = b;
    // y(n) = sum(x(k)*h(n-k))
    a = b = 0;
    for (i=0; i < LPF_m; i++)
    {
	float x,h;
	float *w,*p;
	w = lpf1_dl->w;
	p = lpf1_dl->p;
	x = *(w + (p-w+i)%LPF_m);
	h = lpf_coeffs[i];
	a += x*h;

	w = lpf2_dl->w;
	p = lpf2_dl->p;
	x = *(w + (p-w+i)%LPF_m);
	h = lpf_coeffs[i];
	b += x*h;
    }
    lpf1_dl->p++;
    lpf2_dl->p++;
    wrap(LPF_m-1, lpf1_dl->w, &lpf1_dl->p);
    wrap(LPF_m-1, lpf2_dl->w, &lpf2_dl->p);


    // third stage (ring with quad2)
    a *= sin(left.detune.quad2.w * left.detune.quad2.t);
    b *= cos(left.detune.quad2.w * left.detune.quad2.t++);

    // final sum
    *outl = a+b;



    /* right */
    lpf1_dl = &right.detune.lpf1_dl;
    lpf2_dl = &right.detune.lpf2_dl;
    a = b = inr;

    // first stage (ring with quad1)
    a *= sin(right.detune.quad1.w * right.detune.quad1.t);
    b *= cos(right.detune.quad1.w * right.detune.quad1.t++);

    // second stage (lowpass filters)
    *lpf1_dl->p = a;
    *lpf2_dl->p = b;
    // y(n) = sum(x(k)*h(n-k))
    a = b = 0;
    for (i=0; i < LPF_m; i++)
    {
	float x,h;
	float *w,*p;
	w = lpf1_dl->w;
	p = lpf1_dl->p;
	x = *(w + (p-w+i)%LPF_m);
	h = lpf_coeffs[i];
	a += x*h;

	w = lpf2_dl->w;
	p = lpf2_dl->p;
	x = *(w + (p-w+i)%LPF_m);
	h = lpf_coeffs[i];
	b += x*h;
    }
    lpf1_dl->p++;
    lpf2_dl->p++;
    wrap(LPF_m-1, lpf1_dl->w, &lpf1_dl->p);
    wrap(LPF_m-1, lpf2_dl->w, &lpf2_dl->p);


    // third stage (ring with quad2)
    a *= sin(right.detune.quad2.w * right.detune.quad2.t);
    b *= cos(right.detune.quad2.w * right.detune.quad2.t++);

    // final sum
    *outr = a+b;
}

/* octave: fir1(31, ((48000/2)-1)/48000) */
static float lpf_coeffs[LPF_m] = {
   -8.1635e-05,
    9.1246e-05,
   -1.1968e-04,
    1.6578e-04,
   -2.2765e-04,
    3.0277e-04,
   -3.8806e-04,
    4.8002e-04,
   -5.7490e-04,
    6.6880e-04,
   -7.5789e-04,
    8.3851e-04,
   -9.0737e-04,
    9.6164e-04,
   -9.9911e-04,
    1.0182e-03,
    9.9899e-01,
    9.9911e-04,
   -9.6165e-04,
    9.0737e-04,
   -8.3852e-04,
    7.5790e-04,
   -6.6881e-04,
    5.7491e-04,
   -4.8003e-04,
    3.8806e-04,
   -3.0278e-04,
    2.2766e-04,
   -1.6578e-04,
    1.1968e-04,
   -9.1248e-05,
    8.1637e-05
};

