// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlFont.cc

  SlFont implementation
*/

#include <iostream>
#include <stdexcept>

#include "SlFont.h"


SlFont::SlFont(std::string name)
  : name_(name)
{
  font_ = nullptr;
}

SlFont::~SlFont()
{
  if (font_) TTF_CloseFont(font_);
}


void
SlFont::loadFont(std::string fontfile, int fontsize)
{
  font_ = TTF_OpenFont(fontfile.c_str(), fontsize);
  if ( !font_ )
    throw std::runtime_error( "TTF_OpenFont: " + std::string( TTF_GetError() ) );
}



SDL_Color 
SlFont::sdlcolor()
{
  SDL_Color scol = {static_cast<uint8_t>(color[0]), static_cast<uint8_t>(color[1]), static_cast<uint8_t>(color[2]), static_cast<uint8_t>(color[3])};
  return scol;
}



void 
SlFont::setAlpha(uint8_t alpha)
{
  color[3] = alpha;
}



void 
SlFont::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;
}



void 
SlFont::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
  color[0] = red;
  color[1] = green;
  color[2] = blue;
}

