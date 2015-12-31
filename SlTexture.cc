// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTexture.cc

  SlTexture implementation
*/
#include <iostream>
#include <string>

#include "SlTexture.h"

SlTexture::SlTexture()
{
  initialize();
}


SlTexture::SlTexture(std::string name)
{
  initialize();
  textureInfo_.name = name;
}

SlTexture::~SlTexture()
{
  std::cout << "Deleting " << textureInfo_.name << std::endl;
  SDL_DestroyTexture(texture_);
  texture_ = nullptr;
}


bool
SlTexture::createFromRectangle(SDL_Renderer* renderer)
{
  bool result = true;
  texture_ = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, textureInfo_.sourceRect.w, textureInfo_.sourceRect.h);

  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture_);
  SDL_SetRenderDrawColor( renderer, textureInfo_.color[0], textureInfo_.color[1], textureInfo_.color[2], textureInfo_.color[3] );
  SDL_RenderFillRect( renderer, &textureInfo_.sourceRect );

  SDL_SetRenderTarget(renderer, nullptr);
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear(renderer);
  
  textureInfo_.destinationRect = textureInfo_.sourceRect;
  return result;
}


bool
SlTexture::createFromRectangle(SDL_Renderer* renderer, SDL_Rect rectangle)
{
  bool result = true;
  textureInfo_.sourceRect = rectangle;
  createFromRectangle(renderer);
  return result;
}


bool
SlTexture::createFromRectangle(SDL_Renderer* renderer, int w, int h)
{
  bool result = true;
  textureInfo_.sourceRect.x = 0;
  textureInfo_.sourceRect.y = 0;
  textureInfo_.sourceRect.w = w;
  textureInfo_.sourceRect.h = h;
  createFromRectangle(renderer);
  return result;
}



bool
SlTexture::currentLocation(int& x, int &y)
{
  bool hasLocation = true;
  x = textureInfo_.destinationRect.x;
  y = textureInfo_.destinationRect.y;
  return hasLocation;
}



SDL_Rect
SlTexture::destination()
{
  return textureInfo_.destinationRect;
}



bool
SlTexture::destinationDimension(int& width, int& height)
{
  bool hasDimension = true;
  width = textureInfo_.destinationRect.w;
  height = textureInfo_.destinationRect.h;
  return hasDimension;
}


bool
SlTexture::dimension(int& width, int& height)
{
  bool hasDimension = true;
  width = textureInfo_.sourceRect.w;
  height = textureInfo_.sourceRect.h;
  return hasDimension;
}



void
SlTexture::initialize()
{
  texture_ = nullptr;
}



bool
SlTexture::initializeFromConfigFile(SDL_Renderer* renderer, std::string fileName)
{
  bool isInitialized = true;
  // to be continued
  return isInitialized;
}


bool
SlTexture::loadFromFile(SDL_Renderer* renderer, std::string fileName)
{
  bool isLoaded = false;
  if (texture_){
    std::cout << textureInfo_.name <<  " [SlTexture::loadFromFile] " << fileName << ": already has texture." << std::endl; 
    return isLoaded;
  }

  texture_ = IMG_LoadTexture(renderer, fileName.c_str());
  
  if( texture_ == nullptr ){
    std::cout << "Unable to create texture from " << fileName << " - SDL Error "<< SDL_GetError() ;
    isLoaded = false;
  }
  else{
    isLoaded = true;
  }
  SDL_QueryTexture(texture_,nullptr,nullptr,&(textureInfo_.sourceRect.w),&(textureInfo_.sourceRect.h) );
  textureInfo_.sourceRect.x = 0;
  textureInfo_.sourceRect.y = 0;
  textureInfo_.destinationRect = textureInfo_.sourceRect;
    
  return isLoaded;
}


std::string
SlTexture::name()
{
  std::string temp = textureInfo_.name;
  return temp;
}


bool
SlTexture::recoverOriginalSize()
{
  bool isRecovered = true;
  int check = SDL_QueryTexture(texture_, nullptr, nullptr, &textureInfo_.sourceRect.w, &textureInfo_.sourceRect.h);
  if (check == 0) {
    textureInfo_.sourceRect.x = 0;
    textureInfo_.sourceRect.y = 0;
    return isRecovered;
  }
  else
    return false;
}


bool
SlTexture::render(SDL_Renderer *renderer)
{
  bool isRendered = true;
  renderThis(renderer, textureInfo_);  
  return isRendered;
}


bool
SlTexture::renderThis(SDL_Renderer *renderer, SlTextureInfo& textureInfo)
{
  bool isRendered = true;
  
  SDL_Rect *dstRect = nullptr;
  SDL_Rect *srcRect = nullptr;
  if (textureInfo.renderOptions & SL_RENDER_USE_DESTINATION) dstRect = &(textureInfo.destinationRect);
  if (textureInfo.renderOptions & SL_RENDER_USE_SOURCE) srcRect = &(textureInfo.sourceRect);
  
  int modColor = (textureInfo.renderOptions & SL_RENDER_COLORMOD);
  if ((modColor == SL_RENDER_COLORMOD) && !textureInfo.colorModIsSet) {
    //std::cout <<"[SlTexture::render] colormod on " << std::endl;
    SDL_SetTextureColorMod(texture_, textureInfo.color[0], textureInfo.color[1], textureInfo.color[2] );
    textureInfo.colorModIsSet = true;
  }
  if (textureInfo.colorModIsSet && (modColor == 0)) {
    //std::cout <<"[SlTexture::render] colormod off " << std::endl;
    SDL_SetTextureColorMod(texture_, 0xFF, 0xFF, 0xFF);
    textureInfo.colorModIsSet = false;
  }
  
  int modAlpha = (textureInfo.renderOptions & SL_RENDER_ALPHAMOD);
  if ((modAlpha == SL_RENDER_ALPHAMOD) && !textureInfo.alphaModIsSet) {
    //std::cout <<"[SlTexture::render] alphamod on " << std::endl;
    SDL_SetTextureBlendMode( texture_, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod(texture_, textureInfo.color[3] );
    textureInfo.alphaModIsSet = true;
  }
  if (textureInfo.alphaModIsSet && (modAlpha == 0)){
    //    std::cout <<"[SlTexture::render] alphamod off " << std::endl;
    SDL_SetTextureBlendMode( texture_, SDL_BLENDMODE_NONE );
    //SDL_SetTextureAlphaMod(texture_, 0xFF );
    textureInfo.alphaModIsSet = false;
  }

  int check = SDL_RenderCopy(renderer, texture_, srcRect, dstRect);
  if (check != 0) isRendered = false;
  return isRendered;
}


bool
SlTexture::renderWithOptions(SDL_Renderer *renderer, uint32_t renderOptions)
{
  bool isRendered = true;
  textureInfo_.renderOptions = renderOptions;
  isRendered = render(renderer);
  return isRendered;
}


bool
SlTexture::renderTextureOnTexture(SDL_Renderer *renderer, SlTexture *toAdd)
{
  bool isRendered = true;
  isRendered = renderTextureOnTexture(renderer, toAdd, toAdd->textureInfo_.renderOptions);
  return isRendered;
}



bool
SlTexture::renderTextureOnTexture(SDL_Renderer *renderer, SlTexture *toAdd, uint32_t renderOptions)
{
  bool isRendered = true;
  SDL_SetRenderTarget(renderer, texture_);
  isRendered = toAdd->renderWithOptions(renderer, renderOptions);
  SDL_SetRenderTarget(renderer, nullptr);
  return isRendered;
}



bool
SlTexture::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  bool result = true;
  textureInfo_.color[0] = red;
  textureInfo_.color[1] = green;
  textureInfo_.color[2] = blue;
  textureInfo_.color[3] = alpha;
  return result;
}



bool
SlTexture::setDestination(SDL_Rect dstRect)
{
  bool isSet = true;
  textureInfo_.destinationRect = dstRect;
  return isSet;
}


bool
SlTexture::setDestinationDimension(int w, int h)
{
  bool isSet = true;
  textureInfo_.destinationRect.w = w;
  textureInfo_.destinationRect.h = h;
  return isSet;
}


bool
SlTexture::setDestinationOrigin(int x, int y)
{
  bool isSet = true;
  textureInfo_.destinationRect.x = x;
  textureInfo_.destinationRect.y = y;
  return isSet;
}


void
SlTexture::setRenderOptions(uint32_t renderOptions)
{
  textureInfo_.renderOptions = renderOptions;
}



bool
SlTexture::tileTexture(SDL_Renderer *renderer, int width, int height)
{
  bool isRendered = true;
  SDL_Texture *newTexture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetRenderTarget(renderer, newTexture);
  SDL_RenderClear(renderer);

  textureInfo_.destinationRect.x = 0;
  textureInfo_.destinationRect.y = 0; 
  textureInfo_.destinationRect.w = textureInfo_.sourceRect.w;
  textureInfo_.destinationRect.h = textureInfo_.sourceRect.h;
  textureInfo_.renderOptions = SL_RENDER_USE_SOURCE | SL_RENDER_USE_DESTINATION;
  
  while ( textureInfo_.destinationRect.y < height) {
    isRendered = renderThis(renderer, textureInfo_);
    if (isRendered == false) return isRendered;
    if ( textureInfo_.destinationRect.x + textureInfo_.sourceRect.w < width) {
      textureInfo_.destinationRect.x += textureInfo_.sourceRect.w;
    }
    else {
      textureInfo_.destinationRect.x = 0;
      textureInfo_.destinationRect.y += textureInfo_.sourceRect.h;
    }
  }

  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderClear(renderer);
  textureInfo_.sourceRect.x = textureInfo_.sourceRect.y = 0;
  textureInfo_.sourceRect.w = width;
  textureInfo_.sourceRect.h = height;
  textureInfo_.destinationRect = textureInfo_.sourceRect;

  SDL_DestroyTexture(texture_);
  texture_ = newTexture;
  return isRendered;  
}
