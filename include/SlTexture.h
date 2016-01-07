// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTexture.h
  \brief SlTexture class for creating SDL_Textures. 
*/

#ifndef SLTEXTURE_H
#define SLTEXTURE_H

#include <cstdint>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlSprite.h"

class SlSprite;

/*! \class SlTexture

  Main class for textures.
*/
class SlTexture
{
  /*! SlSprite is friend so SlSprites can set the alpha mod and color mod of the texture.
   */
  friend class SlSprite;
  friend class SlManager;
 public:
  /*! Constructor with texture name.

    All textures have a name. Remember to create the actual texture!
  */
  SlTexture(std::string name);
  /*! deletes texture_
   */
  ~SlTexture();
  /*! Delete copy constructor. There's no reason to copy the texture, if the goal is to render different parts of it or with different options create a SlSprite.
   */
  SlTexture(const SlTexture&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlTexture &operator=(const SlTexture&) = delete;

  /*! Doesn't do anything yet.
   */
  int createFromConfigFile(SDL_Renderer* renderer, std::string fileName);
  /*! createFromRectangle uses SDL_FillRect to create a texture based on the given geometry and the color defined in SlTextureInfo.
   */
  int createFromRectangle(SDL_Renderer* renderer, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha );
  /*! Create a new texture by rendering a sprite on this texture.
   */
  int createFromSpriteOnTexture(SDL_Renderer *renderer, SlTexture* backgroundTexture, std::shared_ptr<SlSprite> foregroundSprite);
  /*! Creates a new texture with dimensions w x h and fills it with tiles of SlSprite tile. 

    The SlRenderSettings::destinationRect at position 0 in the sprite's lSprite::destinations_ is used to determine the width of the tile. 
    If the sprite's SlSprite::destinations_ is empty, addDefaultDestination() is called which sets the destinationRect to equal the sourceRect.
  */
  int createFromTile(SDL_Renderer *renderer, std::shared_ptr<SlSprite> tile, int width, int height);
  /*! Returns the dimensions of the SDL_Texture.
   */
  bool dimensions(int& width, int& height);
  /*! uses IMG_LoadTexture to get texture from png image file
   */
  bool loadFromFile(SDL_Renderer* renderer, std::string fileName);

  
 protected:
  SlTexture();
  /*! The object's name. Cannot be changed.
   */
  std::string name_ = "unnamedTexture";
  /*! The actual SDL_Texture.
   */
  SDL_Texture *texture_;
  /*! alphaModIsSet stores whether SDL_SetTextureAlphaMod has been applied. 
    If true but the RenderOptions for the next render call don't request the mod
    anymore, the texture is reset to default.
  */
  bool alphaModIsSet = false;
  /*! colorModIsSet stores whether SDL_SetTextureColorMod has been applied. 
    If true but the RenderOptions for the next render call don't request the mod
    anymore, the texture is reset to default.
  */
  bool colorModIsSet = false;
};

#endif // SLTEXTURE_H
