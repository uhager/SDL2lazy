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



/*! \class SlFont for creating a texture from text.
 */
class SlFont
{
 public:
  /*! Constructor, makes sure each SlFont has a name.
   */
  SlFont(std::string name);
  /*!Default destructor. Calls TTF_CloseFont. 
   */ 
  ~SlFont();

  /*! The colour the font will be rendered when creating the texture. (This is not the same as the color mod and alpha mod that may be applied when the resulting SlSprite is rendered.)
   */
  short color[4] = {0, 0, 0, 255};
  /*! Direct access to the TTF_Font.
   */
  TTF_Font* font() {return font_;}
  /*! Loads font from fontfile. Hangs on to the font until destructor is called.\n
    \throws std::runtime_error if font can't be loaded.
   */ 
  void loadFont(std::string fontfile, int fontsize);
  /*! The object name cannot be changed after instantiation.
   */
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
  /*! The actual font. To improve perfomance, the object will keep the font until the destructor is called.
   */
  TTF_Font* font_;
  /*! The object name cannot be changed after instantiation.
   */
  std::string name_;

};


#endif  /* SLFONT_H */


