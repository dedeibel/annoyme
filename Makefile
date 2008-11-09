
MAINFILES=Annoyme.o Event.o XevieInput.o StaticConfiguration.o AlsaOutput.o Sample.o SimpleWaveFileLoader.o SoundLoaderFactory.o
EXECUTABLES=annoyme
LDFLAGS=-lX11 -lXevie
SOURCES=$(wildcard *.cpp)
LIBS=-lasound
DEPS=$(patsubst %.o,%.d,$(MAINFILES))

CXXFLAGS=-Wall -I. -ggdb $(LIBS)

all: annoyme

annoyme: $(MAINFILES)

clean:
	rm -f *.o *.d
	rm -f $(EXECUTABLES)

deps: $(SOURCES)
	$(CXX) -MD -E $^ > /dev/null

-include $(DEPS)
