#include "haas.h"
#include <string.h>
#include <sndfile.h>

#define SR 48000
void dump(float *l, float *r, SNDFILE *sf, int n);
int main(void)
{
    float impulse_l[SR];
    float output_l[SR];
    float impulse_r[SR];
    float output_r[SR];
    haas_parameters p;
    SNDFILE *sf;
    SF_INFO info;

    // set up impulse
    memset(impulse_l,0,sizeof(impulse_l));
    impulse_l[0] = 1.0;
    memset(impulse_r,0,sizeof(impulse_r));
    impulse_r[0] = 1.0;

    // soundfile
    info.samplerate = SR;
    info.channels = 2;
    info.format = SF_FORMAT_WAV|SF_FORMAT_FLOAT;
    sf = sf_open_fd(fileno(stdout), SFM_WRITE, &info, 0);

    // real wire
    dump(impulse_l, impulse_r, sf, SR);

    // wire
    p.predelay = 0;
    p.delay = 0;
    p.pan = 0; 
    p.detune = 0;
    p.lpf = 0;
    p.lpf_cutoff = 18000;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // predelay
    p.predelay = 100;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // delay left 45ms
    p.predelay = 0;
    p.delay = -45;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // delay right 45ms
    p.predelay = 0;
    p.delay = 45;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // pan left 1/2
    p.delay = 0;
    p.pan = -.5;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // pan right 1/2
    p.pan = .5;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // detune left 15 cents
    p.pan = 0;
    p.detune = -15;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // detune right 15 cents
    p.detune = 15;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // lpf full right at 880
    p.detune = 0;
    p.lpf = 1.0;
    p.lpf_cutoff = 880;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    // lpf full right at 880
    p.lpf = -1.0;
    haas_config(p,SR);
    haas_run(impulse_l, impulse_r, output_l, output_r, SR);
    dump(output_l, output_r, sf, SR);

    sf_close(sf);
    return 0;
}

void dump(float *l, float *r, SNDFILE *sf, int n)
{
    int i;
    float frame[2];

    for (i=0; i<n; i++)
    {
	frame[0] = l[i];
	frame[1] = r[i];
	sf_writef_float(sf,frame,1);
    }
}
