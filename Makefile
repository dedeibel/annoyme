
MAINFILES=Annoyme.o XevieInput.o StaticConfiguration.o AlsaOutput.o Sample.o SimpleWaveFileLoader.o
EXECUTABLES=annoyme

SOURCES=$(wildcard *.cpp)
DEPS=$(patsubst %.o,%.d,$(MAINFILES))

CXXFLAGS=-I.

all: annoyme

annoyme: $(MAINFILES)

clean:
	rm -f *.o *.d
	rm -f $(EXECUTABLES)

deps: $(SOURCES)
	$(CXX) -MD -E $^ > /dev/null

-include $(DEPS)
