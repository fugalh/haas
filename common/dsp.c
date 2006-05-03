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
void normalize(int n, float *in, float *out)
{
    int i;
    float max = 0, g;
    for (i=0; i<n; i++)
	if (in[i] > max)
	    max = in[i];

    if (max > 1.0)
	g = 1.0/max;
    else
	g = 1.0;

    for (i=0; i<n; i++)
	*out = *in * g;
}
	
