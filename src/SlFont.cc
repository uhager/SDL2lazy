

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
