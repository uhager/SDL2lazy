// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpritesheet.cc

  SlTexture implementation
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "SlSpritesheet.h"


SlSpritesheet::SlSpritesheet()
  : SlTexture()
{
}

SlSpritesheet::SlSpritesheet(std::string name)
  : SlTexture(name)
{
  textureInfo_.name = name;
}

// SlSpritesheet::~SlSpritesheet()
// {
// }



bool
SlSpritesheet::centerAllSpritesAt(int x, int y)
{
  bool result = true;
  for (auto &sprite: sprites){
    sprite.destinationRect.x = x - sprite.destinationRect.w / 2 ;
    sprite.destinationRect.y = y - sprite.destinationRect.h / 2 ;
  }
  return result;
}


bool
SlSpritesheet::centerAllSpritesIn(int x, int y, int w, int h)
{
  bool result = true;
  for (auto &sprite: sprites){
    sprite.destinationRect.x = x + (w - sprite.destinationRect.w) / 2 ;
    sprite.destinationRect.y = y + (h - sprite.destinationRect.h) / 2 ;
    }
  return result;
}


bool
SlSpritesheet::centerAllSpritesIn(SDL_Rect rect)
{
  bool result = true;
  result = centerAllSpritesIn(rect.x, rect.y, rect.w, rect.h);
  return result;
}



bool
SlSpritesheet::centerSpriteIn(std::string name, int x, int y, int w, int h)
{
  bool result = true;
  setCurrentSprite(name);
  if (currentSprite == sprites.end()){
    std::cout << "[SlSpritesheet::centerSpriteIn] unknown sprite " << std::endl;
    return false;
  }
  currentSprite->destinationRect.x = x + (w - currentSprite->destinationRect.w) / 2 ;
  currentSprite->destinationRect.y = y + (h - currentSprite->destinationRect.h) / 2 ;
  return result;
}


bool
SlSpritesheet::centerSpriteAt(std::string name, int x, int y)
{
  bool result = true;
  setCurrentSprite(name);
  if (currentSprite == sprites.end()){
    std::cout << "[SlSpritesheet::centerSpriteAt] unknown sprite " << std::endl;
    return false;
  }
  currentSprite->destinationRect.x = x +  currentSprite->destinationRect.w / 2 ;
  currentSprite->destinationRect.y = y +  currentSprite->destinationRect.h / 2 ;
  return result;
}

void
SlSpritesheet::setCurrentSprite(std::string name)
{
  for (auto sprite = sprites.begin() ; sprite != sprites.end() ; ++sprite) {
    if (name == sprite->name ) {
      currentSprite = sprite;
      return;
    }
  }
}

  
int
SlSpritesheet::findSprite(std::string name)
{
  int result = 0;
  for (unsigned int i = 0; i<sprites.size(); i++) {
    if (name.compare( sprites.at(i).name ) == 0) {
      result = i;
      break;
    }
  }
  return result;
}

bool
SlSpritesheet::readCoordSheet(std::string fileName)
{
  bool result = true;
  std::ifstream input(fileName,std::ifstream::in);
  std::string line;
  getline(input,line);
   while ( input)
    {
      result = readSourceRect(line);
      if (result == false) break;
      if ( input) getline(input,line);
    }
  return result;
}


bool
SlSpritesheet::readSourceRect(std::string line)
{
  bool result = true;
  SlTextureInfo toAdd;
  std::istringstream stream(line.c_str());
  stream >> toAdd.name;
  if (toAdd.name.size() == 0) return false;
  //  cout << name << endl;

  int coord[4];
  for (int i = 0; i<4 ; i++)
    {
      try {
	stream >> coord[i];
      }
      catch (std::exception& except) {
	std::cout << "[SlSpritesheet::readSourceRect] " << except.what() << std::endl;
	return false;
      }
    }
  toAdd.sourceRect.x = coord[0];
  toAdd.sourceRect.y = coord[1];
  toAdd.sourceRect.w = coord[2]-coord[0];
  toAdd.sourceRect.h = coord[3]-coord[1];
  toAdd.destinationRect.x = 0;
  toAdd.destinationRect.y = 0;
  toAdd.destinationRect.w = toAdd.sourceRect.w;
  toAdd.destinationRect.h = toAdd.sourceRect.h;
  toAdd.renderOptions = SL_RENDER_USE_SOURCE | SL_RENDER_USE_DESTINATION;
  sprites.push_back(toAdd);
  return result;
}

bool
SlSpritesheet::render(SDL_Renderer *renderer)
{
  bool isRendered = true;
  if (currentSprite == sprites.end()) return false;  //< no sprite to be rendered
  if ((currentSprite->renderOptions & SL_RENDER_USE_SOURCE) == 0){                 
    /*! sprites always use their source rectangle, 
      but explicitly setting SL_RENDER_USE_SOURCE allows setting 
      an arbitrary source of the sprite sheet 
      (could just use normal render though...)
    */
    currentSprite->renderOptions |= SL_RENDER_USE_SOURCE ;
  }
  if ( (currentSprite->renderOptions & SL_RENDER_USE_DESTINATION) == 0) {
    currentSprite->renderOptions |= SL_RENDER_USE_DESTINATION ;
  }
  isRendered = renderThis(renderer, *currentSprite);
  return isRendered;
}


bool
SlSpritesheet::renderSprite(SDL_Renderer* renderer, uint32_t renderOptions)
{
  bool result = true;
  if (currentSprite == sprites.end()) return false;  //< no sprite to be rendered
  currentSprite->renderOptions = renderOptions;
  result = render(renderer);
  return result;
}


bool
SlSpritesheet::renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions)
{
  bool result = true;
  setCurrentSprite(name);
  if (currentSprite == sprites.end()){
    std::cerr << "[SlSpritesheet::renderSprite] unknown sprite " << std::endl;
    return false;
  }
  result = renderSprite(renderer, renderOptions);
  return result;
}



bool
SlSpritesheet::setDestinationOrigin(std::string spriteName, int x, int y)
{
  bool result = true;
  int i = findSprite(spriteName);
  if (i == -1){
    std::cerr << "[SlSpritesheet::setDestinationOrigin] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(i).destinationRect.x = x;
  sprites.at(i).destinationRect.y = y;
  return result;
}


bool
SlSpritesheet::setDestinationDimension(std::string spriteName, int w, int h)
{
  bool result = true;
  int i = findSprite(spriteName);
  if (i == -1){
    std::cerr << "[SlSpritesheet::setDestinationDimension] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(i).destinationRect.w = w;
  sprites.at(i).destinationRect.h = h;
  return result;
}

bool
SlSpritesheet::setDestination(std::string spriteName, SDL_Rect destRect)
{
  bool result = true;
  int i = findSprite(spriteName);
  if (i == -1){
    std::cerr << "[SlSpritesheet::setDestination] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(i).destinationRect = destRect;
  return result;
}
  

void
SlSpritesheet::setSpriteRenderOption(std::string name, uint32_t renderOptions)
{
  int i = findSprite(name);
  if (i == -1){
    std::cerr << "[SlSpritesheet::setDestination] unknown sprite " << std::endl;
    return;
  }
  setSpriteRenderOption(i, renderOptions);
}


void
SlSpritesheet::setSpriteRenderOption(uint32_t renderOptions)
{
  currentSprite->renderOptions = renderOptions;
}


void
SlSpritesheet::setSpriteRenderOption(unsigned int i, uint32_t renderOptions)
{
  if ( i >= sprites.size() ){
    std::cerr << "[SlSpritesheet::setDestination] unknown sprite " << std::endl;
    return;
  }
  renderOptions |= SL_RENDER_USE_SOURCE;
  sprites.at(i).renderOptions = renderOptions;
}
