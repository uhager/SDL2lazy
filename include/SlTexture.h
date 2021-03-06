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



class SlSprite;
class SlFont;

/*! \class SlTexture

  Main class for textures.
*/
class SlTexture
{

 public:
  /*! Constructor with texture name.

    All textures have a name. Remember to create the actual texture!
  */
  SlTexture(const std::string& name);
  /*! deletes texture_
   */
  ~SlTexture();
  /*! Delete copy constructor. There's no reason to copy the texture, if the goal is to render different parts of it or with different options create a SlSprite.
   */
  SlTexture(const SlTexture&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlTexture &operator=(const SlTexture&) = delete;

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
  /*! createFromRectangle uses SDL_FillRect to create a texture based on the given geometry and the color defined in SlTextureInfo.
    \throws std::runtime_error if texture can't be created or rectangle can't be rendered.
   */
  SlTexture* createFromRectangle(SDL_Renderer* renderer, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha );
  /*! Create a new texture by rendering a sprite on this texture. Calls SlSprite::render() for the sprite.
    \throws std::runtime_error if texture can't be created or rendered.    
   */
  SlTexture* createFromSpriteOnTexture(SDL_Renderer *renderer, SlTexture* backgroundTexture, const std::shared_ptr<SlSprite> foregroundSprite);
  /*! Create a new texture from the message using the specified SlFont.
    \throws std::runtime_error if surface or texture can't be created.
 */
  SlTexture* createFromText(SDL_Renderer *renderer, const std::shared_ptr<SlFont> font, const std::string& message, int width);
  /*! Creates a new texture with dimensions w x h and fills it with tiles of SlSprite tile. \n
    The SlRenderSettings::destinationRect at position 0 in the sprite's lSprite::destinations_ is used to determine the width of the tile. \n 
    If the sprite's SlSprite::destinations_ is empty, addDefaultDestination() is called which sets the destinationRect to equal the sourceRect.
    \throws std::runtime_error if the texture can't be created or the step size for placing the tiles it <= 0.
  */
  SlTexture* createFromTile(SDL_Renderer *renderer, const std::shared_ptr<SlSprite> tile, int width, int height);
  /*! Returns the dimensions of the SDL_Texture.
   */
  void dimensions(int& width, int& height);
  /*! uses IMG_LoadTexture to get texture from png image file
    \throws std::runtime_error if object already has a texture or texture can't be loaded.    
   */
  SlTexture* loadFromFile(SDL_Renderer* renderer, const std::string& fileName);
  /*! Returns the name of the texture.
    Changing the name after creation is not allowed.
   */
  std::string name() const {return name_;}
  /*! Returns the SDL_texture. Changing the texture is not allowed.
   */
  SDL_Texture* texture() {return texture_;}
  
 protected:
  SlTexture();
  /*! The object's name. Cannot be changed.
   */
  std::string name_ = "unnamedTexture";
  /*! The actual SDL_Texture.
   */
  SDL_Texture *texture_;
};

#endif // SLTEXTURE_H
