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
  currentSprite = -1;
}

SlSpritesheet::SlSpritesheet(std::string name)
  : SlTexture(name)
{
  currentSprite = -1;
  textureInfo_.name = name;  
}

SlSpritesheet::~SlSpritesheet()
{
}



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
  int sprite = findSprite(name);
  if (sprite == -1){
    std::cout << "[SlSpritesheet::centerSpriteIn] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(sprite).destinationRect.x = x + (w - sprites.at(sprite).destinationRect.w) / 2 ;
  sprites.at(sprite).destinationRect.y = y + (h - sprites.at(sprite).destinationRect.h) / 2 ;
  return result;
}


bool
SlSpritesheet::centerSpriteAt(std::string name, int x, int y)
{
  bool result = true;
  int sprite = findSprite(name);
  if (sprite == -1){
    std::cout << "[SlSpritesheet::centerSpriteAt] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(sprite).destinationRect.x = x +  sprites.at(sprite).destinationRect.w / 2 ;
  sprites.at(sprite).destinationRect.y = y +  sprites.at(sprite).destinationRect.h / 2 ;
  return result;
}



int
SlSpritesheet::findSprite(std::string name)
{
  int result = -1;
  for (unsigned int i = 0; i<sprites.size(); i++) {
    if (name.compare( sprites.at(i).name ) == 0) {
      result = i;
      break;
    }
  }
  currentSprite = result;
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
  if (toAdd.name.size() == 0) return false;
  sprites.push_back(toAdd);
  return result;
}


bool
SlSpritesheet::renderSprite(SDL_Renderer* renderer, uint32_t renderOptions)
{
  bool result = true;
  if (currentSprite == -1) return result;  //< -1 (constructor default value) will cause no sprite to be rendered
  if ((renderOptions & SL_RENDER_USE_SOURCE) == 0){                 
    /*! sprites always use their source rectangle, 
      but explicitly setting SL_RENDER_USE_SOURCE allows setting 
      an arbitrary source of the sprite sheet 
      (could just use normal render though...)
    */
    textureInfo_.sourceRect = sprites.at(currentSprite).sourceRect;
    renderOptions |= SL_RENDER_USE_SOURCE ;
  }
  if ( (renderOptions & SL_RENDER_USE_DESTINATION) == 0) {
    textureInfo_.destinationRect = sprites.at(currentSprite).destinationRect;
    renderOptions |= SL_RENDER_USE_DESTINATION ;
  }
  
  textureInfo_.colorModIsSet = sprites.at(currentSprite).colorModIsSet;
  textureInfo_.alphaModIsSet = sprites.at(currentSprite).alphaModIsSet;
  result = render(renderer, renderOptions);
  sprites.at(currentSprite).colorModIsSet = textureInfo_.colorModIsSet;
  sprites.at(currentSprite).alphaModIsSet = textureInfo_.alphaModIsSet;
  return result;
}

bool
SlSpritesheet::renderSprite(std::string name, SDL_Renderer* renderer, uint32_t renderOptions)
{
  bool result = true;
  int sprite = findSprite(name);
  if (sprite == -1){
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
  int iSprite = findSprite(spriteName);
  if (iSprite == -1){
    std::cerr << "[SlSpritesheet::setDestinationOrigin] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(iSprite).destinationRect.x = x;
  sprites.at(iSprite).destinationRect.y = y;
  return result;
}


bool
SlSpritesheet::setDestinationDimension(std::string spriteName, int w, int h)
{
  bool result = true;
  int iSprite = findSprite(spriteName);
  if (iSprite == -1){
    std::cerr << "[SlSpritesheet::setDestinationDimension] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(iSprite).destinationRect.w = w;
  sprites.at(iSprite).destinationRect.h = h;
  return result;
}

bool
SlSpritesheet::setDestination(std::string spriteName, SDL_Rect destRect)
{
  bool result = true;
  int iSprite = findSprite(spriteName);
  if (iSprite == -1){
    std::cerr << "[SlSpritesheet::setDestination] unknown sprite " << std::endl;
    return false;
  }
  sprites.at(iSprite).destinationRect = destRect;
  return result;
}
  
