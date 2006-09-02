#ifndef DETUNE_H
#define DETUNE_H

#include "dsp.h"

// XXX is this a good size?
#define DETUNE_RING_m 8

typedef struct {
    delay_state dl;
    float offset;
    float delta;
    float ring[DETUNE_RING_m];
} detune_state;

float detune(detune_state *s, float x);

#endif
