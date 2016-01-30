// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSprite.cc

  SlSprite implementation
*/

#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "SlTexture.h"
#include "SlSprite.h"


SlSprite::SlSprite(const std::string& name, SlTexture* texture, int x, int y, int width, int height)
  : name_(name)
  , texture_(texture)
{
  if (width == 0 || height == 0) {   //! assumes whole texture to be used
    SDL_QueryTexture(texture_->texture(), nullptr, nullptr, &sourceRect_.w, &sourceRect_.h);
  }
  else {
    sourceRect_.x = x;
    sourceRect_.y = y;
    sourceRect_.w = width;
    sourceRect_.h = height;
  }
  addDefaultDestination();
#ifdef DEBUG
  std::cout << "[SlSprite::SlSprite] Created " << name_ << " from texture " << texture_->name() << " w = " << sourceRect_.w  << " h = " << sourceRect_.h << std::endl;
#endif
}



SlSprite::SlSprite()
{}



SlSprite::SlSprite(const std::string& name)
  : name_(name)
{
}



SlSprite::SlSprite(SlTexture* texture)
  : texture_(texture)
{
}



SlSprite::~SlSprite()
{
  texture_ = nullptr;
#ifdef DEBUG
  std::cout << "[SlSprite::~SlSprite] deleting sprite " << name_  << std::endl;
#endif
}



SlSprite*
SlSprite::addDefaultDestination()
{
  SlRenderSettings defSet;
  defSet.destinationRect = sourceRect_;
  defSet.destinationRect.x = defSet.destinationRect.y = 0;
  destinations_.push_back(defSet);

  return this;
}



SlSprite*
SlSprite::addDestination(int x, int y, uint32_t renderOptions)
{
  SlRenderSettings toAdd;
  toAdd.destinationRect = sourceRect_;
  toAdd.destinationRect.x = x;
  toAdd.destinationRect.y = y;
  destinations_.push_back(toAdd);

  return this;
}



SlSprite*
SlSprite::centerAt( unsigned int x, unsigned int y, unsigned int destination)
{
  if ( destination >= destinations_.size() )
    throw std::invalid_argument("Invalid sprite destination index.");
  
  SDL_Rect& dest = destinations_.at(destination).destinationRect ;
  dest.x = x - dest.w / 2 ;
  dest.y = y - dest.h / 2 ;

  return this;
}



SlSprite*
SlSprite::centerInSprite(const std::shared_ptr<SlSprite> otherSprite, unsigned int destinationThis, unsigned int destinationOther)
{
  if ( destinationThis >= destinations_.size() || destinationOther >= otherSprite->destinations_.size() ) 
    throw std::invalid_argument( "[SlSprite::centerInSprite] Couldn't center " + name_ + " in " + otherSprite->name_ + ": destination out of bounds." );
    
  SDL_Rect target = otherSprite->destinations_.at(destinationOther).destinationRect;
  SDL_Rect& dest = destinations_.at(destinationThis).destinationRect ;
  dest.x = target.x + ( target.w - dest.w ) / 2 ;
  dest.y = target.y + ( target.h - dest.h ) / 2 ;

  return this;
}



void
SlSprite::clearDestinations()
{
  destinations_.clear();
}



SDL_Rect
SlSprite::destination(unsigned int i)
{
  SDL_Rect rect;
  if (i >= destinations_.size() )
    throw std::invalid_argument("[SlSprite::destination] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  rect = destinations_.at(i).destinationRect;
  return rect;
}



void
SlSprite::destinationDimension(int& width, int& height, unsigned int i)
{
  if (i >= destinations_.size() )
    throw std::invalid_argument("[SlSprite::destinationDimension] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  width = destinations_.at(i).destinationRect.w;  
  height = destinations_.at(i).destinationRect.h;  
}



void
SlSprite::destinationOrigin(int& x, int& y, unsigned int i)
{
  if (i >= destinations_.size() )
    throw std::invalid_argument("[SlSprite::destinationOrigin] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );

  x = destinations_.at(i).destinationRect.x;  
  y = destinations_.at(i).destinationRect.y;  
}



bool
SlSprite::hasDestination()
{
  return !destinations_.empty();
}
  


bool
SlSprite::is_inside(const int& x, const int& y, const unsigned int& dest)
{
  bool result = true;
  SDL_Rect& destRect = destinations_.at(dest).destinationRect;
  if ( x < destRect.x || x > destRect.x + destRect.w ||
       y < destRect.y || y > destRect.y + destRect.h )
    result = false;
  return result;
}



void
SlSprite::render(SDL_Renderer* renderer)
{
#ifdef DEBUG
  if (destinations_.size() > 1) std::cout << "[SlSprite::render] " << name_ << ": rendering " << destinations_.size() << " destinations" << std::endl;
#endif

  for (unsigned int i = 0; i < destinations_.size() ; ++i){
    render(renderer, i);
  }
}



void
SlSprite::render(SDL_Renderer* renderer, unsigned int i)
{
  if (i >= destinations_.size() )
    throw std::runtime_error("Invalid render destination for " + name_ );

  SlRenderSettings& dest = destinations_.at(i);
    
  int modColor = (dest.renderOptions & SL_RENDER_COLORMOD);
  if ((modColor == SL_RENDER_COLORMOD) && !texture_->colorModIsSet) {
    SDL_SetTextureColorMod(texture_->texture(), dest.color[0], dest.color[1], dest.color[2] );
    texture_->colorModIsSet = true;
  }
  if (texture_->colorModIsSet && (modColor == 0)) {
    SDL_SetTextureColorMod(texture_->texture(), 0xFF, 0xFF, 0xFF);
    texture_->colorModIsSet = false;
  }
  
  int modAlpha = (dest.renderOptions & SL_RENDER_ALPHAMOD);
  if ((modAlpha == SL_RENDER_ALPHAMOD) && !texture_->alphaModIsSet) {
    SDL_SetTextureBlendMode( texture_->texture(), SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod(texture_->texture(), dest.color[3] );
    texture_->alphaModIsSet = true;
  }
  if (texture_->alphaModIsSet && (modAlpha == 0)){
    SDL_SetTextureBlendMode( texture_->texture(), SDL_BLENDMODE_NONE );
    //SDL_SetTextureAlphaMod(texture_, 0xFF );
    texture_->alphaModIsSet = false;
  }

  int hasRendered = SDL_RenderCopy(renderer, texture_->texture(), &sourceRect_, & dest.destinationRect);
  if (hasRendered != 0) {
    throw std::runtime_error("Error rendering " + name_ + ": " + std::string( SDL_GetError() ) );
  }
}



void
SlSprite::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int i)
{
  if (i >= destinations_.size() )
    throw std::invalid_argument("[SlSprite::setColor] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );

  auto p = destinations_.at(i).color;
  *p = red; ++p;
  *p = green; ++p;
  *p = blue; ++p;
  *p = alpha; 
}



void
SlSprite::setDestination(const SDL_Rect& dstRect, unsigned int i)
{

  if (i >= destinations_.size()) 
    throw std::invalid_argument("[SlSprite::setDestination] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  destinations_.at(i).destinationRect = dstRect;
}



void
SlSprite::setDestination(int x, int y, int w, int h, unsigned int i)
{
  if (i >= destinations_.size())
    throw std::invalid_argument("[SlSprite::setDestination] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
    
  SDL_Rect dstRect = {x, y, w, h};
  setDestination(dstRect, i);
}



void
SlSprite::setDestinationDimension(int width, int height, unsigned int i)
{
  if (i >= destinations_.size()) 
    throw std::invalid_argument("[SlSprite::setDestinationDimension] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  destinations_.at(i).destinationRect.w = width;
  destinations_.at(i).destinationRect.h = height;
}



void
SlSprite::setDestinationOrigin(int x, int y, unsigned int i)
{
  if (i >= destinations_.size()) 
    throw std::invalid_argument("[SlSprite::setDestinationOrigin] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  SDL_Rect& dest = destinations_.at(i).destinationRect;
  dest.x = x;
  dest.y = y;
}



void
SlSprite::setRenderOptions(uint32_t renderOptions, unsigned int i)
{
  if (i >= destinations_.size()) 
    throw std::invalid_argument("[SlSprite::setRenderOptions] attempting to access destination " + std::to_string(i) + " out of " + std::to_string( destinations_.size() ) );
  destinations_.at(i).renderOptions = renderOptions;
}


  

