## Makefile for SDL2lazy
## author: Ulrike Hager

SRC = ./src
INC = ./include
LIB = ./lib
EXAMPLE = ./example

SDL_INCLUDES = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_image

CC = gcc
CXX = g++
CXXFLAGS += -g -O2 -Wall -fPIC -std=c++11 -I$(INC)

OBJS = $(SRC)/SlTexture.o $(SRC)/SlSpritesheet.o
EXAMPLE_OBJS = $(EXAMPLE)/lazy-test.o
ALL += lib/libSDL2lazy.so example/lazy-test

example: example/lazy-test
lib: lib/libSDL2lazy.so
all: $(ALL) 

%.o: %.cc
	$(CC) $(CXXFLAGS) $(SDL_INCLUDES) -o $@ -c $<

lib/libSDL2lazy.so: $(OBJS)
	mkdir -p lib/
	$(CC) -shared -o  $@ $(OBJS) $(SDL_LIBS)

example/lazy-test: lib/libSDL2lazy.so $(EXAMPLE_OBJS)
	$(CXX) $(CXXFLAGS) $(EXAMPLE_OBJS) $(SDL_LIBS) -L$(LIB) -lSDL2lazy -o $@

clean:
	rm -f *.o *.so $(ALL) $(OBJS) $(EXAMPLE_OBJS)
	-rm -rf lib/

dox:
	doxygen Doxyfile
