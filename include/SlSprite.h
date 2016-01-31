// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSprite.h
  \brief SlSprite class, RenderOptions enum, SlRenderSettings struct

A SlSprite is any object that will be rendered from a texture. It contains a pointer to a SlTexture and has to be created after the SlTexture.\n
The SlSprite has a source rectangle which defines which part of the texture is rendered and cannnot be changed. It can have several destination rectangles with different render options to render it multiple times on screen.
*/

#ifndef SLSPRITE_H
#define SLSPRITE_H


#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlRenderOptions.h"
#include "SlTexture.h"



/*! \struct SlRenderSettings

  Contains the destination rectangle, the color (for SDL_SetColorMod), and the render options.
*/
struct SlRenderSettings
{
  /*! Holds the location where the sprite will be rendered in the window.
   */
  SDL_Rect destinationRect = {0,0,0,0};
  /*! red, green, blue, alpha

    Used for SL_RENDER_COLORMOD (SDL_SetTextureColorMod) and SL_RENDER_ALPHAMOD (SDL_SetTextureAlphaMod).
    Also used as fill colour when creating texture from rectangle.
    Initialized to black and max alpha.
  */
  uint8_t color[4] = {0x00, 0x00, 0x00, 0xFF};
  /*! SlRenderOptions for the texture.
  */
  uint32_t renderOptions = SL_RENDER_DEFAULT;
  /*! Angle by which the sprite will be rotated.
   */
  double angle = 0;
};



/*! \class SlSprite
Defines a part of a texture to be rendered with given settings.\n
A SlSprite is any object that will be rendered from a texture. It contains a pointer to a SlTexture and has to be created after the SlTexture.\n
The SlSprite has a source rectangle which defines which part of the texture is rendered and cannnot be changed. It can have several destination rectangles with different render options to render it multiple times on screen.
 */
class SlSprite
{
 public:
  /*! This is the constructor that should be used for creating SlSprites. 

    All SlSprites must have a name and a texture. \n
    If either width or height are 0, the texture's width and height will be used.\n
    The default values will use the whole texture as a sprite.
    Calls addDefaultDestination(), so if you don't want to use that, either delete or adjust that destination.
   */
  SlSprite(const std::string& name, SlTexture* texture, int x = 0, int y = 0, int width = 0, int height = 0);
  
  ~SlSprite();
  /*! Delete copy constructor. There's no reason to copy the sprite, if the goal is to render different parts of it or with different options create another sprite.
   */
  SlSprite(const SlSprite&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlSprite &operator=(const SlSprite&) = delete;

  /*! Adds an entry to #destinations_ with default settings.
    Default destination is x = 0, y = 0, and destination dimensions equal to source dimensions;
   */
  SlSprite* addDefaultDestination();
  /*! Adds an entry to #destinations_. The destination dimensions are set to the source dimensions, color is default.
   */
  SlSprite* addDestination(int x, int y, uint32_t renderOptions = SL_RENDER_DEFAULT);
  /*! Centers the destination of this sprite in the destinationRect of the other sprite.
   */
  SlSprite* centerInSprite(const std::shared_ptr<SlSprite> otherSprite, unsigned int destinationThis = 0, unsigned int destinationOther = 0);
  /*! Centers the destination of this sprite at the specified coordinates.
   */
  SlSprite* centerAt( unsigned int x, unsigned int y, unsigned int destination = 0);
  /*! Removes all entries from #destinations_. Sprite will not be rendered.
   */
  void clearDestinations();
  /*! Returns the origin and dimension of the texture in the window, i.e. the destinationRect for #destinations_ at position i.
   */
  SDL_Rect destination(unsigned int i = 0);
  /*! Returns the dimension of the texture in the window, i.e. destinationRect width, height for #destinations_ at position i.
   */
  void destinationDimension(int& width, int& height, unsigned int i = 0);
  /*! Returns the origin of the texture in the window, i.e. destinationRect x,y coordinates for #destinations_ at position i.
   */
  void destinationOrigin(int& x, int& y, unsigned int i = 0);
  /*! Checks if any destinations are defined, i.e. if #destinations_ has size > 0;
   */
  bool hasDestination();
  /*! Checks whether the given coordinates are inside the specified destination for this sprite.
   */
  bool is_inside(const int& x, const int& y, const unsigned int& dest = 0);
  /*! Moves the sprite by the amounts given by x and y, i.e. x and y are deltas not absolutes.
  */
  void moveDestinationOriginBy(int x, int y, unsigned int i = 0);
  /*! Allows reading the name, but not changing it.
   */
  std::string name() const {return name_;}
  /*! Renders all copies of the sprite given in #destinations_.
   */
  void render(SDL_Renderer* renderer);
  /*! Renders the copy of the sprite at position i in render settings.\n
    \throws std::runtime_error if invalid destination or unable to render.
   */
  void render(SDL_Renderer* renderer, unsigned int i);

  /*! Sets angle for position i of #destinations_.
    The angle in degrees that indicates the rotation that will be applied when that sprite destination is rendered. Rotation will be around object centre.
   */
  void setAngle(double angle, unsigned int i = 0);
  /*! Sets color for position i of #destinations_.
    Color is use when using color mod to render, and when creating a texture from a rectangle.
   */
  void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF, unsigned int i = 0);
  /*! Changes destinationRect for position i of #destinations_.

    Sets where and how large the texture will be rendered.
  */
  void setDestination(const SDL_Rect& dstRect, unsigned int i = 0);
  /*! Changes destinationRect for position i of #destinations_.

    Sets where and how large the texture will be rendered.
  */
  void setDestination(int x, int y, int w, int h, unsigned int i = 0);
 /*! Changes destinationRect for position i of #destinations_.

    Sets how large the texture will be rendered.
  */
  void setDestinationDimension(int width, int height, unsigned int i = 0);
  /*! Changes destinationRect for position i of #destinations_ .
    Sets where the sprite will be rendered.
  */
  void setDestinationOrigin(int x, int y, unsigned int i = 0);
  /*! Sets SlRenderOptions for position i of #destinations_.
    \retval false if i > #destinations_ size.
   */
  void setRenderOptions(uint32_t renderOptions, unsigned int i = 0);
  /*! Number of defined destinations. Used by the SlManager to check if requested destinations are valid.
   */
  unsigned int size() {return destinations_.size();}
  /*! Access to the name of the underlying SlTexture.
   */
  std::string textureName() const {return texture_->name();}

  
 protected:
  /*! Default constructor
   */
  SlSprite();
  /*! Creates sprite with name.
    Remember to add a texture!
   */
  SlSprite(const std::string& name);
  /*! Creates sprite with with this texture.
   */
  SlSprite(SlTexture* texture);
    /*! The name of a sprite cannot be changed after it is created
   */
  std::string name_ = "unnamedSprite";
  /*! When a texture is loaded from an image file, sourceRect is set to the width and height of the texture.
    This is set when the sprite is created.
  */
  SDL_Rect sourceRect_ = {0,0,0,0};
  /*! Contains the actual SDL_Texture.
   */
  SlTexture* texture_;
  /*! Settings for where and how to render the sprite. Multiple copies of the sprite can be rendered with different settings.
  */
  std::vector<SlRenderSettings> destinations_;

};


#endif // SLSPRITE_H
