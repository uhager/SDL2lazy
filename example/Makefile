## Makefile for lazy-test to test SDL2lazy functionality
## author: Ulrike Hager

OBJS = lazy-test.o

CXX = g++

CXXFLAGS = -Wall -std=c++11 $(shell sdl2-config --cflags) -I../ 

SDLLIBS = $(shell sdl2-config --libs) -lSDL2_image -L../ -lSDL2lazy

all : $(OBJS) lazy-test

lazy-test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(SDLLIBS) -o $@ 

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *.o lazy-test

