// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTexture.cc

  SlTexture implementation
*/
#include <iostream>
#include <string>

#include "SlSprite.h"
#include "SlTexture.h"

SlTexture::SlTexture()
{
  texture_ = nullptr;
}



SlTexture::SlTexture(const std::string& name)
{
  texture_ = nullptr;
  name_ = name;
#ifdef DEBUG
  std::cout << "[SlTexture::SlTexture] Creating " << name_ << std::endl;
#endif // DEBUG
}



SlTexture::~SlTexture()
{
#ifdef DEBUG
  std::cout << "[SlTexture::~SlTexture] Deleting " << name_ << std::endl;
#endif // DEBUG
  SDL_DestroyTexture(texture_);
  texture_ = nullptr;
}



int
SlTexture::createFromConfigFile(const SDL_Renderer* renderer, const std::string& fileName)
{
  int created = 0;
  // to be continued
  return created;
}



int
SlTexture::createFromRectangle(SDL_Renderer* renderer, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
{
  int result = 0;
  
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if (texture_ == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTexture::createFromRectangle] Failed to create texture " << SDL_GetError() << std::endl;
#endif
    return 1;
  }
  
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture_);
  SDL_SetRenderDrawColor( renderer, red, green, blue, alpha );
  SDL_Rect sourceRect = {0,0,width,height};
  result = SDL_RenderFillRect( renderer, &sourceRect );

  SDL_SetRenderTarget(renderer, nullptr);
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderClear(renderer);
  
  return result;
}



int
SlTexture::createFromSpriteOnTexture(SDL_Renderer *renderer, SlTexture* backgroundTexture, const std::shared_ptr<SlSprite> foregroundSprite)
{
  int result = 0;

  int width, height;
  SDL_QueryTexture(backgroundTexture->texture_, nullptr, nullptr, &width, &height);
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if (texture_ == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTexture::createFromSpriteOnTexture] Failed to create texture " << SDL_GetError() << std::endl;
#endif
    return 1;
  }
  
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture_);
  result = SDL_RenderCopy(renderer, backgroundTexture->texture_, nullptr, nullptr);
  foregroundSprite->render(renderer);

  SDL_SetRenderTarget(renderer, nullptr);
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderClear(renderer);

  return result;
}



int
SlTexture::createFromTile(SDL_Renderer *renderer, const std::shared_ptr<SlSprite> tile, int width, int height)
{
  int result = 0;
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if (texture_ == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTexture::createFromTile] Failed to create texture " << SDL_GetError() << std::endl;
#endif
    return 1;
  }

  SDL_SetRenderTarget(renderer, texture_);
  SDL_RenderClear(renderer);

  int currXPos = 0, currYPos = 0;
  int stepWidth, stepHeight;
  if (!tile->hasDestination()) tile->addDefaultDestination();
  
  tile->destinationDimension(stepWidth, stepHeight);
  if ( stepWidth <= 0 || stepHeight <= 0 ) {
#ifdef DEBUG
    std::cout << "[SlTexture::createFromTiles] wrong step size." << std::endl; 
#endif
    return -1;
  }
  tile->clearDestinations();
  
  while ( currYPos < height) {
    tile->addDestination(currXPos, currYPos);
    if ( currXPos + stepWidth < width) {
      currXPos  += stepWidth;
    }
    else {
      currXPos = 0;
      currYPos += stepHeight;
    }
  }
  result = tile->render(renderer);
  tile->clearDestinations();
  
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderClear(renderer);

  return result;
}



bool
SlTexture::dimensions(int& width, int& height)
{
  bool hasDimensions = true;
  if (texture_ == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTexture::dimensions] no texture for " << name_ << std::endl;
#endif
    hasDimensions = false;
    return hasDimensions;
  }
  SDL_QueryTexture(texture_, nullptr, nullptr, &width, &height);
  return hasDimensions;
}



bool
SlTexture::loadFromFile(SDL_Renderer* renderer, const std::string& fileName)
{
  bool isLoaded = false;
  if (texture_){
    std::cout << name_ <<  " [SlTexture::loadFromFile] " << fileName << ": already has texture." << std::endl; 
    return isLoaded;
  }

  texture_ = IMG_LoadTexture(renderer, fileName.c_str());
  
  if( texture_ == nullptr ){
    std::cout << "Unable to create texture from " << fileName << " - SDL Error "<< SDL_GetError() ;
    return isLoaded;
  }
  else{
    isLoaded = true;
  }
  return isLoaded;
}
