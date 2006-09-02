CFLAGS+=-g
all: haas.o dsp.o detune.o
haas.o: haas.c haas.h dsp.h detune.h

LDFLAGS+=-lsndfile
test: test.o haas.o dsp.o detune.o

clean:
	rm -f *.o test

.PHONY: clean
