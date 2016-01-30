// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTexture.cc

  SlTexture implementation
*/
#include <iostream>
#include <string>
#include <stdexcept>

#include "SlSprite.h"
#include "SlFont.h"

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



SlTexture*
SlTexture::createFromRectangle(SDL_Renderer* renderer, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
{
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if (texture_ == nullptr) 
    throw std::runtime_error("Failed to create texture " + std::string( SDL_GetError() ));
  
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture_);
  SDL_SetRenderDrawColor( renderer, red, green, blue, alpha );
  SDL_Rect sourceRect = {0,0,width,height};
  int check = SDL_RenderFillRect( renderer, &sourceRect );
  if ( check != 0 )
    throw std::runtime_error("Couldn't render rectangle: " + std::string( SDL_GetError() ));
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderClear(renderer);

  return this;
}



SlTexture*
SlTexture::createFromSpriteOnTexture(SDL_Renderer *renderer, SlTexture* backgroundTexture, const std::shared_ptr<SlSprite> foregroundSprite)
{
  int width, height;
  SDL_QueryTexture(backgroundTexture->texture_, nullptr, nullptr, &width, &height);
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if (texture_ == nullptr) 
    throw std::runtime_error("Failed to create texture " + std::string( SDL_GetError() ));
  
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture_);
  int check = SDL_RenderCopy(renderer, backgroundTexture->texture_, nullptr, nullptr);
  if ( check != 0 )
    throw std::runtime_error("Couldn't render background: " + std::string( SDL_GetError() ));
  foregroundSprite->render(renderer);

  SDL_SetRenderTarget(renderer, nullptr);
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderClear(renderer);

  return this;
}



SlTexture*
SlTexture::createFromText(SDL_Renderer *renderer, const std::shared_ptr<SlFont> font, const std::string& message, int width)
{
  SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font->font(), message.c_str(), font->sdlcolor(), width);
  if (surf == nullptr)
    throw std::runtime_error("Failed to create surface from text: " + std::string( SDL_GetError() ));

  texture_ = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
  if (texture_ == nullptr) 
    throw std::runtime_error("Failed to create texture " + std::string( SDL_GetError() ));

  return this;
}



SlTexture*
SlTexture::createFromTile(SDL_Renderer *renderer, const std::shared_ptr<SlSprite> tile, int width, int height)
{

  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  if ( texture_ == nullptr )
    throw std::runtime_error("Couldn't create texture " + name_ + ": " + std::string( SDL_GetError() ));

  SDL_SetRenderTarget(renderer, texture_);
  SDL_RenderClear(renderer);

  int currXPos = 0, currYPos = 0;
  int stepWidth, stepHeight;
  if (!tile->hasDestination()) tile->addDefaultDestination();
  
  tile->destinationDimension(stepWidth, stepHeight);
  if ( stepWidth <= 0 || stepHeight <= 0 ) 
    throw std::runtime_error("Failed to create texture from tiles: wrong step size." );
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
  tile->render(renderer);
  tile->clearDestinations();
  
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderClear(renderer);

  return this;
}



void
SlTexture::dimensions(int& width, int& height)
{

  if (texture_ == nullptr) 
    throw std::runtime_error( "[SlTexture::dimensions] no texture for " + name_ );
  SDL_QueryTexture(texture_, nullptr, nullptr, &width, &height);

}



SlTexture*
SlTexture::loadFromFile(SDL_Renderer* renderer, const std::string& fileName)
{
  if (texture_)
    throw std::runtime_error("Texture " + name_ + " already has a texture." );
  
  texture_ = IMG_LoadTexture(renderer, fileName.c_str());
  
  if( texture_ == nullptr )
    throw std::runtime_error("Unable to create texture from " + fileName + " " + SDL_GetError() );

  return this;
}
