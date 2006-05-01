#ifndef HAAS_H
#define HAAS_H

typedef struct {
    float predelay, delay, pan, detune, lpf, lpf_cutoff;
} parameters;

void config(parameters, int samplerate);
void run(float *in_left, float *in_right, 
	float *out_left, float *out_right,
	int num_frames);

extern int fs;

#endif
