#ifndef HAAS_H
#define HAAS_H

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct {
    float predelay, delay, pan, detune, lpf, lpf_cutoff;
} haas_parameters;

void haas_config(haas_parameters, int samplerate);
void haas_run(float *in_left, float *in_right, 
	float *out_left, float *out_right,
	int num_frames);

#ifdef __cplusplus
}
#endif
#endif
