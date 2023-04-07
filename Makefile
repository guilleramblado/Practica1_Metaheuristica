CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=FUENTES/programa.cpp
INCLUDES=includes/programa.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=BIN/ejecutable

all: $(SOURCES) $(INCLUDES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

