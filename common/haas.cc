#include "haas.h"

Haas::Haas() : 
    delay(0), balance(0), detune(0), lpf(0), lpf_freq(9.5), predelay(0),
    ldelay(0, &lchan),			rdelay(0, &rchan),
    ldetune(0.1, &ldelay, (int)0),	rdetune(0.1, &rdelay, (int)0),
    llpf(85000, &ldetune),		rlpf(85000, &rdetune)
{
}
