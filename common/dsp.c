#include "dsp.h"

void cdelay(int D, float *w, float **p)
{
	(*p)--;			/* move pointer and... */
	wrap(D, w, p);		/* ...wrap modulo-(D+1) */
}

void wrap(int M, float *w, float **p)
{
    if (*p < w || *p > (w + M))
	*p = w + (*p - w) % (M + 1);

    if (*p - w < 0)
	*p += M+1;	// stupid C modulus operator
}
