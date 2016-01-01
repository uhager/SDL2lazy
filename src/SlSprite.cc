// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSprite.cc

  SlSprite implementation
*/

#include <iostream>
#include <string>

#include "SlTexture.h"
#include "SlSprite.h"


SlSprite::SlSprite(std::string name, SlTexture* texture)
  : name_(name)
  , texture_(texture)
{
  SDL_QueryTexture(texture_->texture_, nullptr, nullptr, &sourceRect_.w, &sourceRect_.h);
  addDefaultDestination();
#ifdef DEBUG
  std::cout << "[SlSprite::SlSprite] new sprite " << name_ << " from texture " << texture_->name_ << " w = " << sourceRect_.w  << " h = " << sourceRect_.h << std::endl;
#endif
}



SlSprite::SlSprite()
{}



SlSprite::SlSprite(std::string name)
  : name_(name)
{
}



SlSprite::SlSprite(SlTexture* texture)
  : texture_(texture)
{
}



SlSprite::~SlSprite()
{
#ifdef DEBUG
  std::cout << "[SlSprite::~SlSprite] deleting sprite " << name_  << std::endl;
#endif

}



void
SlSprite::addDefaultDestination()
{
  SlRenderSettings defSet;
  defSet.destinationRect = sourceRect_;
  destinations_.push_back(defSet);
}



void
SlSprite::addDestination(int x, int y, uint32_t renderOptions)
{
  SlRenderSettings toAdd;
  toAdd.destinationRect = sourceRect_;
  toAdd.destinationRect.x = x;
  toAdd.destinationRect.y = y;
  destinations_.push_back(toAdd);
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
  return rect;
}



bool
SlSprite::destinationDimension(int& width, int& height, unsigned int i)
{
  bool hasDestination = true;
  if (i >= destinations_.size() ){
#ifdef DEBUG
    std::cout << "[SlSprite::destinationDimension] attempting to access destination " << i << " out of " << destinations_.size() << std::endl;
#endif
    width = 0;
    height = 0;
    hasDestination = false;
  }
  else {
    width = destinations_.at(i).destinationRect.w;  
    height = destinations_.at(i).destinationRect.h;  
  }
  return hasDestination;  
}



bool
SlSprite::destinationOrigin(int& x, int& y, unsigned int i)
{
  bool hasDestination = true;
  if (i >= destinations_.size() ){
#ifdef DEBUG
    std::cout << "[SlSprite::destinationOrigin] attempting to access destination " << i << " out of " << destinations_.size() << std::endl;
#endif
    x = 0;
    y = 0;
    hasDestination = false;
  }
  else {
    x = destinations_.at(i).destinationRect.x;  
    y = destinations_.at(i).destinationRect.y;  
  }
  return hasDestination;  
}



bool
SlSprite::hasDestination()
{
  bool hasDestination = true;
  if (destinations_.size() == 0) hasDestination = false;
  return hasDestination;
}
  



int
SlSprite::render(SDL_Renderer* renderer)
{
  int hasRendered = 0;
#ifdef DEBUG
  if (destinations_.size() > 1) std::cout << "[SlSprite::render] " << name_ << ": rendering " << destinations_.size() << " destinations" << std::endl;
#endif

  for (unsigned int i = 0; i < destinations_.size() ; ++i){
    hasRendered = render(renderer, i);
    if (hasRendered != 0) {
      return hasRendered;
    }
  }
  return hasRendered;
}



int
SlSprite::render(SDL_Renderer* renderer, unsigned int i)
{
  int hasRendered = 0;
  if (i >= destinations_.size() ){
#ifdef DEBUG
    std::cout << "[SlSprite::render] attempting to render destination " << i << " out of " << destinations_.size() << std::endl;
#endif
    return -1;
  }
  SlRenderSettings& dest = destinations_.at(i);
    
  int modColor = (dest.renderOptions & SL_RENDER_COLORMOD);
  if ((modColor == SL_RENDER_COLORMOD) && !texture_->colorModIsSet) {
    //std::cout <<"[SlTexture::render] colormod on " << std::endl;
    SDL_SetTextureColorMod(texture_->texture_, dest.color[0], dest.color[1], dest.color[2] );
    texture_->colorModIsSet = true;
  }
  if (texture_->colorModIsSet && (modColor == 0)) {
    //std::cout <<"[SlTexture::render] colormod off " << std::endl;
    SDL_SetTextureColorMod(texture_->texture_, 0xFF, 0xFF, 0xFF);
    texture_->colorModIsSet = false;
  }
  
  int modAlpha = (dest.renderOptions & SL_RENDER_ALPHAMOD);
  if ((modAlpha == SL_RENDER_ALPHAMOD) && !texture_->alphaModIsSet) {
    //std::cout <<"[SlTexture::render] alphamod on " << std::endl;
    SDL_SetTextureBlendMode( texture_->texture_, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod(texture_->texture_, dest.color[3] );
    texture_->alphaModIsSet = true;
  }
  if (texture_->alphaModIsSet && (modAlpha == 0)){
    //    std::cout <<"[SlTexture::render] alphamod off " << std::endl;
    SDL_SetTextureBlendMode( texture_->texture_, SDL_BLENDMODE_NONE );
    //SDL_SetTextureAlphaMod(texture_, 0xFF );
    texture_->alphaModIsSet = false;
  }

  hasRendered = SDL_RenderCopy(renderer, texture_->texture_, &sourceRect_, & dest.destinationRect);
  if (hasRendered != 0) {
    std::cout << "[SlSprite::render] Error rendering sprite " << name_ << " detination " << i << " - " << SDL_GetError() << std::endl;
  }
  return hasRendered;
}



bool
SlSprite::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int i)
{
  bool hasDestination = true;
  return hasDestination;  
}



bool
SlSprite::setDestination(SDL_Rect dstRect, unsigned int i)
{
  bool hasDestination = true;
  return hasDestination;  
}



bool
SlSprite::setDestinationDimension(int width, int height, unsigned int i)
{
  bool hasDestination = true;
  return hasDestination;  
}



bool
SlSprite::setDestinationOrigin(int x, int y, unsigned int i)
{
  bool hasDestination = true;
  if (i >= destinations_.size()) {
#ifdef DEBUG
      std::cout << "[SlSprite::setDestinationOrigin] requested element: " << i << " destinations_.size(): " << destinations_.size() << std::endl; 
#endif
      hasDestination = false;
    }
    else {
      SDL_Rect& dest = destinations_.at(i).destinationRect;
      dest.x = x;
      dest.y = y;
    }
  return hasDestination;  
}



bool
SlSprite::setRenderOptions(uint32_t renderOptions, unsigned int i)
{
  bool hasDestination = true;
  return hasDestination;  
}


  

