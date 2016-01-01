// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTexture.h
  \brief SlTexture class, RenderOptions enum, SlTextureInfo struct
*/

#ifndef SLTEXTURE_H
#define SLTEXTURE_H

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/*! \enum RenderOptions
  \brief specifies where, what, and how to render. 

  Specifies whether to use the SlTexture's source or destination SDL_Rect. 
  Also specifies use of colour mod and alpha mod.
*/
enum SlRenderOptions {
  SL_RENDER_DEFAULT         = (1u << 0),
  SL_RENDER_USE_DESTINATION = (1u << 1),
  SL_RENDER_USE_SOURCE      = (1u << 2),
  SL_RENDER_COLORMOD        = (1u << 3),
  SL_RENDER_FADE            = (1u << 4),
  SL_RENDER_ALPHAMOD        = (1u << 5),
};


/*! \struct SlTextureInfo

  Contains source and destination rectangles.
  In SlSpritesheet, the location of each sprite on the spritesheet is defined by the corresponding SlTextureInfo.
*/
struct SlTextureInfo
{
  std::string name;
  /*! When a texture is loaded from an image file, sourceRect is set to the width and height of the texture.
    It can be adjusted to clip certain sections of the texture.
  */
  SDL_Rect sourceRect = {0,0,0,0};
  /*! destinationRect specifies where and how large to render the texture. 
    Upon loading an image it is set to the same values as sourceRect.
  */
  SDL_Rect destinationRect = {0,0,0,0};
  /*! red, green, blue, alpha

    Used for SL_RENDER_COLORMOD (SDL_SetTextureColorMod) and SL_RENDER_ALPHAMOD (SDL_SetTextureAlphaMod).
    Also used as fill colour when creating texture from rectangle.
    Initialized to black and max alpha.
  */
  uint8_t color[4] = {0x00, 0x00, 0x00, 0xFF};
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
  /*! SlRenderOptions for the texture.
    Used when  rendered with render(SDL_Renderer *renderer), 
    overwritten when rendered with bool render(SDL_Renderer *renderer, uint32_t renderOptions);
  */
  uint32_t renderOptions = SL_RENDER_DEFAULT;
};


/*! \class SlTexture

  Main class for textures.
*/
class SlTexture
{
 public:
  /*! Constructor with texture name.

    All textures have a name. This is currently only really implemented for spritesheets, where individual
    sprites are identified by their name.
  */
  SlTexture(std::string name);
  /*! deletes texture_
   */
  virtual ~SlTexture();
  /*! Delete copy constructor. There's no reason to copy the texture, if the goal is to render different parts of it or with different options use SlSpritesheet.
   */
  SlTexture(const SlTexture&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlTexture &operator=(const SlTexture&) = delete;

  
  /*! createFromRectangle uses SDL_FillRect to create a texture based on the given geometry and the color defined in SlTextureInfo.
    When no geometry is specified in the function call, SlTextureInfo::sourceRect is used.
    Colour has to be set before calling this, or the texture will be the default colour (currently black).
  */
  bool createFromRectangle(SDL_Renderer* renderer);
  /*! Sets textureInfo_.sourceRect to rectangle and calls createFromRectangle(SDL_Renderer* renderer)
   */
  bool createFromRectangle(SDL_Renderer* renderer, SDL_Rect rectangle);
  /*! Sets textureInfo_.sourceRect width and height to w, h, and calls createFromRectangle(SDL_Renderer* renderer)
   */
   bool createFromRectangle(SDL_Renderer* renderer, int w, int h);
  /*! Returns the origin of the texture in the window, i.e. destinationRect x,y coordinates.
   */
  bool currentLocation(int& x, int &y);  
  /*! Returns the origin and dimension of the texture in the window, i.e. the destinationRect.
   */
  SDL_Rect destination();
  /*! Returns the dimension of the texture in the window, i.e. destinationRect width, height.
   */
  bool destinationDimension(int& width, int& height);
  /*! Returns the dimension of the texture, i.e. sourceRect width, height.
   */
  bool dimension(int& width, int& height);
  /*! grand plans, not currently implemented
   */
  bool initializeFromConfigFile(SDL_Renderer* renderer, std::string fileName);
  /*! uses IMG_LoadTexture to get texture from png image file
   */
  bool loadFromFile(SDL_Renderer* renderer, std::string fileName);
  /*! Returns the name of the SlTexture
   */
  std::string name();
  /*! Restores sourceRect to texture size.

    If textureInfo_.sourceRect was changed, this function calls SDL_QueryTexture to restore it to the size of the texture. 
   */
  bool recoverOriginalSize();

  /*! Renders texture_ using SDL_RenderCopy
   
    Uses textureInfo_.renderOptions to specify where and what to plot.
    Currently implemented:\n
    sourceRect to clip part of the texture\n
    destinationRect to specify where and how big to render\n
    colour mod (SDL_SetTextureColorMod)\n
    alpha mod (SDL_SetTextureAlphaMod)\n
  */
  virtual bool render(SDL_Renderer *renderer);

  /*! Renders texture_.
   
    Replaces textureInfo_.renderOptions with renderOptions.
    renderOptions use enum RenderOptions to specify where and what to plot.
    Currently implemented:\n
    sourceRect to clip part of the texture\n
    destinationRect to specify where and how big to render\n
    colour mod (SDL_SetTextureColorMod)\n
    alpha mod (SDL_SetTextureAlphaMod)\n
  */
  bool renderWithOptions(SDL_Renderer *renderer, uint32_t renderOptions);

  /*! Renders SlTexture toAdd on top of texture_ , using toAdd->textureInfo_.renderOptions

    Used to combine textures.
    renderOptions modify toAdd when rendered on top of texture_.
  */
  bool renderTextureOnTexture(SDL_Renderer *renderer, SlTexture *toAdd);

  /*! Renders SlTexture toAdd on top of texture_ , renderOptions apply to toAdd

    Used to combine textures.
    renderOptions modify toAdd when rendered on top of texture_.
  */
  bool renderTextureOnTexture(SDL_Renderer *renderer, SlTexture *toAdd, uint32_t renderOptions);

  /*! color is use when using color mod to render, and when creating a texture from a rectangle.
   */
  bool setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
  /*! Changes destinationRect

    Sets where and how large the texture will be rendered.
  */
  bool setDestination(SDL_Rect dstRect);
  /*! Changes destinationRect

    Sets where the texture will be rendered.
  */
  bool setDestinationOrigin(int x, int y);
  /*! Changes destinationRect

    Sets how large the texture will be rendered.
  */
  bool setDestinationDimension(int width, int height);
  /*! Sets SlRenderOptions in textureInfo_
   */
  void setRenderOptions(uint32_t renderOptions);
  /*!Creates a new texture with dimensions w x h and fills it with tiles of the current texture. 
    The current texture is not stretched, but rendered as often as needed to fill the new texture.
    The current texture's sourceRect is used to determine what part is used for the tiles 
    so that it is possible to use only part of the old texture for the new texture. 
    The original texture is then deleted, and the sourceRect is adjusted to the new texture's dimensions.
   */
  bool tileTexture(SDL_Renderer *renderer, int width, int height);
  
 protected:
  SlTexture();
  /*! The actual SDL_Texture.
   */
  SDL_Texture *texture_;
  /*! Contains the SlTextureInfo for the texture.
    Used when rendering.
   */
  SlTextureInfo textureInfo_;
  void initialize();

  /*! Renders texture using settings in textureInfo. 

    This is where the actual rendering happens.
    textureInfo is passed as a reference to allow toogling of alphaModIsSet and colorModIsSet.
  */
  bool renderThis(SDL_Renderer *renderer, SlTextureInfo& textureInfo);

};

#endif // SLTEXTURE_H
