#ifndef DSP_H
#define DSP_H

// Z^-m
typedef struct {
    float *w,*p;
    int m;		// delay length (not order)
} delay_state;

void cdelay(int D, float *w, float **p);
void wrap(int M, float *w, float **p);

#endif
