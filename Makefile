CC=g++
CFLAGS=-c -Wall -O2
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(addprefix build/,$(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=build/ArithmeticEncoder

all: dir $(SOURCES) $(EXECUTABLE)

dir:
	mkdir -p build

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build

tests: all
	cp build/ArithmeticEncoder tests/ && cd tests/ && ./test-all
	
