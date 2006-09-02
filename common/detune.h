#ifndef DETUNE_H
#define DETUNE_H

typedef struct {
    float *w;
    float offset;
    float ring[96000/2]; // XXX This probably doesn't have to be so large
} detune_state;

float detune(detune_state *s, float cents);

#endif
