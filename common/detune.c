#include "detune.h"
#include <math.h>

float detune(detune_state *s, float x)
{ 
    double a,b;
    float w,t;

#if 0
    // short circuit
    *outl = inl;
    *outr = inr;
    return;
#endif

    // init
    a = b = x;

    // first stage (ring with quad1)
    w = s->quad1.w;
    t = (float)s->quad1.t++/fs;
    a *= sin(w*t);
    b *= cos(w*t);

    // second stage (lowpass filters)
    a = fir_lpf(&s->lpf1_dl, a);
    b = fir_lpf(&s->lpf2_dl, b);

    // third stage (ring with quad2)
    w = s->quad2.w;
    t = (float)s->quad2.t++/fs;
    a *= sin(w*t);
    b *= cos(w*t);

    // final sum
    return a+b;
}

#include "lpf.c"

// sum_{k=0}^{M}(b_k*z^-k)
double fir_lpf(delay_state *dl, float x)
{
    double b,h;
    int k;
    float *p, *w;
    double y;

    // fir
    w = dl->w;
    p = dl->p;
    *p = x;
    y = 0;
    for (k=0; k<LPF_m; k++)
    {
	h = *(w+(p-w+k)%LPF_m);
	b = lpf_coeffs[k];
	y += b*h;
    }

    // update
    cdelay(LPF_m-1, w, &dl->p);
    
    return y;
}

