VST=../vstsdk2.4
PLUGIN=haas

CXXFLAGS=-I$(VST) -I$(VST)/public.sdk/source/vst2.x -DBUILDING_DLL -mno-cygwin
CFLAGS=$(CXXFLAGS)
LDFLAGS=
DLLWRAP=dllwrap --target=i386-mingw32 -mno-cygwin
DLL=$(PLUGIN).dll
HAAS=haas-vst.o dsp.o haas.o detune.o 

$(DLL): $(HAAS) audioeffect.o audioeffectx.o vstplugmain.o
	$(DLLWRAP) --driver-name c++ --def $(PLUGIN).def $^ -o $@

audioeffect.o: $(VST)/public.sdk/source/vst2.x/audioeffect.cpp
	$(CXX) $(CXXFLAGS) -c $<

audioeffectx.o: $(VST)/public.sdk/source/vst2.x/audioeffectx.cpp
	$(CXX) $(CXXFLAGS) -c $<

vstplugmain.o: $(VST)/public.sdk/source/vst2.x/vstplugmain.cpp
	$(CXX) $(CXXFLAGS) -c $<

haas-vst.o: ../haas-vst.cc
	$(CXX) $(CXXFLAGS) -c $<
haas.o: ../../common/haas.c
	$(CC) $(CFLAGS) -c $<
detune.o: ../../common/detune.c
	$(CC) $(CFLAGS) -c $<
dsp.o: ../../common/dsp.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(DLL) lib$(PLUGIN).*

.PHONY: clean
