// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpritesheet.h

  SlSpritesheet
*/
#ifndef SLSPRITESHEET_H
#define SLSPRITESHEET_H

#include <cstdint>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlTexture.h"

/*! \class SlSpritesheet

This class is used for textures that consist of parts which are rendered independently.
Sprite names and coordinates are read in from a configuration file.
Each sprite has its own SlTextureInfo. The sourceRect cannot be overwritten since the information cannot be restored.
 */

class SlSpritesheet : public SlTexture 
{
 public:
  /*! Constructor setting the Spritesheet's name.
    Like SlTextures, SlSpritesheets all have names.
   */
  SlSpritesheet(std::string name);
  //  ~SlSpritesheet();

  /*! Centers all sprites at some coordinate using their sourceRect.

    The given coordinates will be at the x- and y- midpoints of each sprite's destinationRect.
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.
  */
  bool centerAllSpritesAt(int x, int y);   
  /*! Centers all sprites in the specified rectangle.

    The center of each sprite's destinationRect will be at the center of the rectangle given by
    x, y, x+w, y+h.\n
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.
  */
  bool centerAllSpritesIn(int x, int y, int w, int h);   
  /*! Centers all sprites in the specified rectangle.

    The center of each sprite's destinationRect will be at the center of the given rectangle.
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.
  */
  bool centerAllSpritesIn(SDL_Rect rect);
  /*! Center sprite at the specified coordinates.

    The center the sprite's destinationRect will be placed at the specified coordinates.
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.
    Calls setCurrentSprite() setting the #currentSprite iterator to this sprite.
  */
  bool centerSpriteAt(std::string name, int x, int y);
  /*! Center sprite in the specified rectangle.

    The center the sprite's destinationRect will be at the center of the rectangle given by
    x, y, x+w, y+h.\n
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.\n
    Calls setCurrentSprite() setting the #currentSprite iterator to this sprite.
  */
  bool centerSpriteIn(std::string name, int x, int y, int w, int h);
  /*! Sets the currentSprite iterator to point at the specified sprite
    After calling setCurrentSprite you can call renderSprite(SDL_Renderer* renderer, uint32_t renderOptions) directly rather than
    renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions).
   */
  void setCurrentSprite(std::string name);
  /*! Finds sprite by its name and returns its position in #sprites.

    Does not affect currentSprite.
   */
  int findSprite(std::string name);
  /*! Reads a configuration file with names and coordinates that specify the sprites
    on the texture.
    Example:\n
    left    0       0       140     80\n
    right   0       80      140     160\n
    where "left" and "right" are the names of 2 sprites with their coordinates given by x1, y1, x2, y2 \n
    (nb: these are all coordinates, not width and height, the 2 sprites in the example have the same dimensions.)
   */
  bool readCoordSheet(std::string fileName);
  /* Render the current sprite with its SlTextureInfo settings

     Calls SlTexture::render(SDL_Renderer *renderer, SlTextureInfo textureInfo).
   */
  bool render(SDL_Renderer *renderer);
  /*! Renders all sprites as specified in their SlTextureInfo properties.
   */
  bool renderAll(SDL_Renderer *renderer);
  /*! Renders the specified sprite. 

    Calls setCurrentSprite(std::string name) and thus also sets the #currentSprite.
    Then calls renderSprite(SDL_Renderer* renderer, uint32_t renderOptions).
    Note about renderOptions: since it doesn't make sense to render a sprite with a different sourceRect, 
    SL_RENDER_USE_SOURCE is always used with the sprite's sourceRect.
   */
  bool renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions);
  /*! Renders the #currentSprite.

    Note about renderOptions: since it doesn't make sense to render a sprite with a different sourceRect, 
    SL_RENDER_USE_SOURCE is always used with the sprite's sourceRect.
   */
  bool renderSprite(SDL_Renderer* renderer, uint32_t renderOptions);
  /*! Sets the coordinates for plotting the specified sprite.
   */
  bool setDestinationOrigin(std::string spriteName, int x, int y);
  /*! Sets the target dimensions for plotting the specified sprite.
  */
  bool setDestinationDimension(std::string spriteName, int w, int h);
  /*! Sets the destination rectangle for plotting the specified sprite.
   */
  bool setDestination(std::string spriteName, SDL_Rect destRect);
  /*! Sets the render options for the named sprite.

    SL_RENDER_USE_SOURCE will remain set.
   */
  void setSpriteRenderOption(std::string name, uint32_t renderOptions);
  /*! Sets the render options for currentSprite.

    SL_RENDER_USE_SOURCE will remain set.
   */
  void setSpriteRenderOption(uint32_t renderOptions);
    

 private:
  SlSpritesheet();
  /*! Holds a SlTextureInfo for each sprite
   */
  std::vector<SlTextureInfo> sprites;
  /*! Points to the sprite set by setCurrentSprite().
   */
  std::vector<SlTextureInfo>::iterator currentSprite = sprites.begin();
  /*! Reads coordinates from line and adds the new sprite to #sprites.
   */
  bool readSourceRect(std::string line);
  /*! Sets the render options for the sprite at position i in #sprites.

    SL_RENDER_USE_SOURCE will remain set.
   */
  void setSpriteRenderOption(unsigned int i, uint32_t renderOptions);
};


#endif // SLSPRITESHEET_H

