#ifndef DSP_H
#define DSP_H

// Z^-m
typedef struct {
    float *w,*p;
    int m;		// delay length (not order)
} delay_state;

float delay(delay_state *s, float x);
void cdelay(int D, float *w, float **p);
void wrap(int M, float *w, float **p);
void normalize(int n, float *in, float *out);

#endif
