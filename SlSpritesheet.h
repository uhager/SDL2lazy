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
  ~SlSpritesheet();

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
  */
  bool centerSpriteAt(std::string name, int x, int y);
  /*! Center sprite in the specified rectangle.

    The center the sprite's destinationRect will be at the center of the rectangle given by
    x, y, x+w, y+h.\n
    This does not change the destinationRect width or height, only the xy origin will be adjusted.
    If the width or height are changed after calling this function, the sprite will no longer be centered.\n
    Calls findSprite(std::string name) and thus also sets the #currentSprite .
  */
  bool centerSpriteIn(std::string name, int x, int y, int w, int h);
  /*! Finds sprite by its name.
    This also sets currentSprite to this sprite. 
    After calling findSprite you can call renderSprite(SDL_Renderer* renderer, uint32_t renderOptions) directly rather than
    renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions).
    Calls findSprite(std::string name) and thus also sets the #currentSprite .
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
  /*! Renders the specified sprite. 

    Calls findSprite(std::string name) and thus also sets the #currentSprite.
    Then calls renderSprite(SDL_Renderer* renderer, uint32_t renderOptions).
    Note about renderOptions: since it doesn't make sense to render a sprite with a different sourceRect, 
    SL_RENDER_USE_SOURCE is always used with the sprite's sourceRect.
    If SL_RENDER_USE_DESTINATION is not set, the sprite's destination rectangle is used, otherwise the SlTexture::textureInfo_'s destinationRect is used.
   */
  bool renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions);
  /*! Renders the #currentSprite.

    Note about renderOptions: since it doesn't make sense to render a sprite with a different sourceRect, 
    SL_RENDER_USE_SOURCE is always used with the sprite's sourceRect.
    If SL_RENDER_USE_DESTINATION is not set, the sprite's destination rectangle is used, otherwise the SlTexture::textureInfo_'s destinationRect is used.
    TL;DR: Use SL_RENDER_DEFAULT to render a sprite to its current destination.
   */
  bool renderSprite(SDL_Renderer* renderer, uint32_t renderOptions);
  /*! Sets the coordinates for plotting the specified sprite.

    Calls findSprite(std::string name) and thus also sets the #currentSprite .
   */
  bool setDestinationOrigin(std::string spriteName, int x, int y);
  /*! Sets the target dimensions for plotting the specified sprite.

    Calls findSprite(std::string name) and thus also sets the #currentSprite .
   */
  bool setDestinationDimension(std::string spriteName, int w, int h);
  /*! Sets the destination rectangle for plotting the specified sprite.

    Calls findSprite(std::string name) and thus also sets the #currentSprite .
   */
  bool setDestination(std::string spriteName, SDL_Rect destRect);


 private:
  SlSpritesheet();
  int currentSprite;
  std::vector<SlTextureInfo> sprites;
  bool readSourceRect(std::string line);
};


#endif // SLSPRITESHEET_H

