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

  short color[4] = {0, 0, 0, 255};
  std::string message;


  TTF_Font* font() {return font_;}
  /*! Loads font from fontfile. Hangs on to the font until destructor is called.\n
    \throws std::runtime_error if font can't be loaded.
   */ 
  void loadFont(std::string fontfile, int fontsize);
  std::string name(){ return name_;}
  /*! Returns the colour as a SDL_Color struct
   */
  SDL_Color sdlcolor();
  /*! Set alpha, leaving colours unchanged.
   */
  void setAlpha(uint8_t alpha);
  /*! Set colours and alpha directly without needing to create an array.
   */
  void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
  /*! Set colours directly without needing to create an array. Leaves alpha unchanged.
   */
  void setColor(uint8_t red, uint8_t green, uint8_t blue);

 private:
  TTF_Font* font_;
  std::string name_;

};


#endif  /* SLFONT_H */
