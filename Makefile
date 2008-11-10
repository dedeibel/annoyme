
MAINFILES=Annoyme.o Event.o XevieInput.o InputEventReaderFactory.o StaticConfiguration.o AlsaOutput.o SoundOutputFactory.o Sample.o SimpleWaveFileLoader.o SoundLoaderFactory.o
EXECUTABLES=annoyme
LIBS=-lasound -lX11 -lXevie
LDFLAGS=$(LIBS)
SOURCES=$(wildcard *.cpp)
DEPS=$(patsubst %.o,%.d,$(MAINFILES))

CXXFLAGS=-Wall -I. -ggdb

all: annoyme

annoyme: $(MAINFILES)

clean:
	rm -f *.o *.d
	rm -f $(EXECUTABLES)

deps: $(SOURCES)
	$(CXX) -MD -E $^ > /dev/null

-include $(DEPS)
