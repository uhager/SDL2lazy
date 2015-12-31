## Makefile for SDL2lazy
## author: Ulrike Hager

CXXFLAGS += -Wall -fPIC -std=c++11 -I.
SDL_INCLUDES = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_image

CC = gcc

OBJS = SlTexture.o SlSpritesheet.o

all: $(OBJS) libSDL2lazy.so

%.o: %.cc
	$(CC) $(CXXFLAGS) $(SDL_INCLUDES) -o $@ -c $<

%.so: $(OBJS) 
	$(CC) -shared -o  $@ $(OBJS) $(SDL_LIBS)
clean:
	rm -f *.o *.so

doc:
	doxygen Doxyfile
