#include "detune.h"
#include <math.h>

float linterpolate(float y0, float y1, float frac)
{
    return y0 + frac*(y1-y0);
}

float detune(detune_state *s, float x)
{
    float *a1, *a2, *b1, *b2, a, b;
    float c;
    float y;

    // advance
    *(s->dl.p) = x;
    (s->dl.p)++;
    wrap(DETUNE_RING_m-1, s->dl.w, &s->dl.p);
    if (s->delta == 1)
        s->offset = s->dl.p - s->dl.w;

    int i = floor(s->offset);
    float frac = s->offset - i;

    // get pointers to a and b, then linearly interpolate
    a1 = s->dl.w + i;
    a2 = a1+1;
    wrap(DETUNE_RING_m-1, s->dl.w, &a1);
    wrap(DETUNE_RING_m-1, s->dl.w, &a2);

    i = i + DETUNE_RING_m/2;
    b1 = s->ring + i;
    b2 = b1+1;
    wrap(DETUNE_RING_m-1, s->dl.w, &b1);
    wrap(DETUNE_RING_m-1, s->dl.w, &b2);

    a = linterpolate(*a1, *a2, frac);
    b = linterpolate(*b1, *b2, frac);

    // how much crossfade?
    int p = s->dl.p - s->dl.w;
    c = fabs(s->offset - p);
#if 0
    // This doesn't seem to work very well.
    if (c > DETUNE_RING_m/2)
        c -= DETUNE_RING_m/2;
    c /= DETUNE_RING_m;
#else
    c = 1; // always a. seems to work best. yes, could optimize away b
#endif

    // calculate y
    y = c*a + (1-c)*b;

    s->offset += s->delta;
    while (s->offset > DETUNE_RING_m-1)
        s->offset -= DETUNE_RING_m;

    return y;
}
