
AUDIOLIBS=alsa
#AUDIOLIBS=alsa ao

EXECUTABLES=annoyme
LIBS=-lX11 -lXevie
SOURCES=$(wildcard *.cpp)

ADD_AUDIO_LIBS=
ADD_AUDIO_FILES=
ADD_AUDIO_DEFINES=

ifneq (,$(findstring alsa,$(AUDIOLIBS)))
  ADD_AUDIO_LIBS+= -lasound
	ADD_AUDIO_FILES+= AlsaOutput.o
	ADD_AUDIO_DEFINES+= -DWITH_ALSA
endif
ifneq (,$(findstring ao,$(AUDIOLIBS)))
  ADD_AUDIO_LIBS+= -lao -ldl
	ADD_AUDIO_FILES+= AOutput.o
	ADD_AUDIO_DEFINES+= -DWITH_AO
endif

MAINFILES=Annoyme.o Event.o Sample.o$(ADD_AUDIO_FILES) XevieInput.o InputEventReaderFactory.o StaticConfiguration.o SoundOutputFactory.o SimpleWaveFileLoader.o SoundLoaderFactory.o
DEPS=$(patsubst %.o,%.d,$(MAINFILES))

LDFLAGS=$(LIBS)$(ADD_AUDIO_LIBS)
CXXFLAGS=-Wall -I. -ggdb$(ADD_AUDIO_DEFINES)


all: annoyme

annoyme: $(MAINFILES)

clean:
	rm -f *.o *.d
	rm -f $(EXECUTABLES)

deps: $(SOURCES)
	$(CXX) $(CXXFLAGS) -MD -E $^ > /dev/null

-include $(DEPS)
