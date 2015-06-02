CC = g++
#CFLAGS = -Wall -ggdb -std=c++11 -O2
#CFLAGS = -O3 -std=c++11
CFLAGS = -std=c++11

SOURCES = run.cpp engine.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp


clean:
	rm *.o run
