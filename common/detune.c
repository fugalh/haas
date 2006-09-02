#include "detune.h"
#include <math.h>

float linterpolate(float y0, float y1, float frac)
{
    return y0 + frac*(y1-y0);
}

float detune(detune_state *s, float x)
{
    float *a1, *a2, *b1, *b2, a, b;
    int i = floor(s->offset);
    float frac = s->offset - i;
    int c;
    float y;

    return x;

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
    c = abs(a1 - s->dl.p);
    if (c > DETUNE_RING_m/2)
        c = abs(c - DETUNE_RING_m);
    c /= DETUNE_RING_m;

    // calculate y
    y = c*a + (1-c)*b;

    // advance
    delay(&s->dl, x);
    s->offset += s->delta;
    if (s->offset > DETUNE_RING_m)
        s->offset -= DETUNE_RING_m;

    return y;
}
