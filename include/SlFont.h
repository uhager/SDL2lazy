// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlFont.h
  \brief SlFont class holds font information.
*/

#ifndef SLFONT_H
#define SLFONT_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class SlFont
{
 public:
  SlFont(std::string name);
  ~SlFont();

  uint8_t color[4] = {0x00, 0x00, 0x00, 0xFF};
  
  bool loadFont(std::string fontfile, int fontsize);
  std::string name(){ return name_;}

 private:
  TTF_Font* font_;
  std::string name_;

};


#endif  /* SLFONT_H */
