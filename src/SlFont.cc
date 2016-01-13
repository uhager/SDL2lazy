// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlFont.cc

  SlFont implementation
*/

#include <iostream>

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


bool
SlFont::loadFont(std::string fontfile, int fontsize)
{
  bool hasfont = false;
  font_ = TTF_OpenFont(fontfile.c_str(), fontsize);
  if ( !font_ ) {
#ifdef DEBUG
    std::cerr << "[SlTextureManager::setFont] Error: couldn't open font from file" << fontfile << std::endl;
#endif
    return hasfont;
  }
  return true;
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

