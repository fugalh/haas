VST_INCLUDE:=-I$(HOME)/src/VST/source/common

all:
	make -C common
	make -C ladspa

clean:
	make -C common clean
	make -C ladspa clean

.PSUEDO: all clean
