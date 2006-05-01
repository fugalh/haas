CFLAGS+=-g
haas.o: haas.c haas.h dsp.h

LDFLAGS+=-lsndfile
test: test.o haas.o dsp.o lpf.o

clean:
	rm -f *.o test

.PHONY: clean
