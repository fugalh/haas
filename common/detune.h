#ifndef DETUNE_H
#define DETUNE_H

#include "haas.h"

static float lpf_coeffs[];
double fir_lpf(delay_state *dl, float x);

#endif
