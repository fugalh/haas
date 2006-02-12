#include "haas.h"

float Haas::cent = pow(2, 1.0/1200);

Haas::Haas() : 
    _delay(0), _balance(0), _detune(0), _lpf(0), _lpf_freq(9.5), _predelay(0),
    lpredelay(0, &lchan),		rpredelay(0, &rchan),
    ldelay(0, &lpredelay),		rdelay(0, &rpredelay),
    ldetune(0.1, &ldelay, (int)0),	rdetune(0.1, &rdelay, (int)0),
    llpf(95000, &ldetune),		rlpf(95000, &rdetune),
    rag(1.0)
{
}

Haas::~Haas()
{
}

void Haas::run(float **in, float **out, unsigned long count)
{
    // set vector sizes
    if (lchan.GetVectorSize() != count)
    {
	lchan.SetVectorSize(count);
	lpredelay.SetVectorSize(count);
	ldelay.SetVectorSize(count);
	ldetune.SetVectorSize(count);
	llpf.SetVectorSize(count);

	rchan.SetVectorSize(count);
	rpredelay.SetVectorSize(count);
	rdelay.SetVectorSize(count);
	rdetune.SetVectorSize(count);
	rlpf.SetVectorSize(count);
    }

    // push in samples
    lchan.PushIn(in[0], count);
    rchan.PushIn(in[1], count);

    // process
    lchan.DoProcess();
    if (_balance > 0)
	lchan *= 1 - _balance;
    lpredelay.DoProcess();
    ldelay.DoProcess();
    ldetune.DoProcess();
    if (_lpf > 0)
    {
	llpf.DoProcess();
	llpf *= _lpf;
	ldetune *= 1.0 - _lpf;
	llpf += ldetune;
	llpf.PopOut(out[0], count);
    } else
	ldetune.PopOut(out[0], count);

    rchan.DoProcess();
    if (_balance < 0)
	rchan *= 1 + _balance;
    rpredelay.DoProcess();
    rdelay.DoProcess();
    rdetune.DoProcess();
    if (_lpf < 0)
    {
	rlpf.DoProcess();
	rlpf *= -_lpf;
	rdetune *= 1.0 + _lpf;
	rlpf += rdetune;
	rlpf.PopOut(out[1], count);
    } else
	rdetune.PopOut(out[1], count);
}

void Haas::run_adding(float **in, float **out, unsigned long count)
{
    float *out2[2];
    int i,j;
    out2[0] = (float*)alloca(sizeof(float) * count);
    out2[1] = (float*)alloca(sizeof(float) * count);

    run(in, out2, count);

    for (i=0; i<2; i++)
	for (j=0; j<count; j++)
	    out[i][j] += rag * out2[i][j];
}

void Haas::set_run_adding_gain(float gain)
{
    rag = gain;
}

float Haas::delay(void)		{ return _delay; }
float Haas::balance(void)	{ return _balance; }
float Haas::detune(void) 	{ return _detune; }
float Haas::lpf(void)		{ return _lpf; }
float Haas::lpf_freq(void)	{ return _lpf_freq; }
float Haas::predelay(void)	{ return _predelay; }

void Haas::set_delay(float v) 
{
    _delay = v;
    if (_delay < 0)
    {
	rdelay.SetDelayTime(-_delay * 1000);
	ldelay.SetDelayTime(0);
    } else {
	ldelay.SetDelayTime(_delay * 1000);
	rdelay.SetDelayTime(0);
    }
}

void Haas::set_balance(float v) 
{
    _balance = v;
}

void Haas::set_detune(float v) 
{
    _detune = v;
    if (_detune < 0)
    {
	rdetune.SetPitch(1.0f + _detune * cent);
	ldetune.SetPitch(1.0f);
    } else {
	ldetune.SetPitch(1.0f - _detune * cent);
	rdetune.SetPitch(1.0f);
    }
}

void Haas::set_lpf(float v) 
{
    _lpf = v;
}

void Haas::set_lpf_freq(float v) 
{
    _lpf_freq = v;
    llpf.SetFreq(_lpf_freq * 1000);
    rlpf.SetFreq(_lpf_freq * 1000);
}

void Haas::set_predelay(float v) 
{
    _predelay = v;
    lpredelay.SetDelayTime(_predelay);
    rpredelay.SetDelayTime(_predelay);
}

