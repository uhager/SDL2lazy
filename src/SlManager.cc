// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManager.cc

  SlManager implementation
*/

#include <iostream>
#include <string>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"

/*! The running instance. 
 */
static SlManager* gInstance = nullptr;

SlManager::SlManager()
{
  if ( gInstance == nullptr) {
    gInstance = this;
  }
  else
    {
      std::cout << "[SlManager::SlManager]  Manager is a singleton class and can only be constructed once." << std::endl;
      exit(1);
    }
  this->initialize();
}



SlManager::SlManager(std::string name, int width, int height)
{
  if ( gInstance == nullptr) {
    gInstance = this;
  }
  else
    {
      std::cout << "[SlManager::SlManager]  Manager is a singleton class and can only be constructed once." << std::endl;
      exit(1);
    }
  this->initialize();
  this->initializeWindow(name, width, height);
}



SlManager::~SlManager(void)
{
  this->clear();
  SDL_DestroyRenderer(renderer_);
  renderer_ = nullptr;
  SDL_DestroyWindow( window_ );
  window_ = nullptr;
  SDL_Quit();
}



void
SlManager::clear()
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    delete (*iter);
  }
  textures_.clear();      

  std::vector<SlSprite*>::iterator sprite;
  for ( sprite = sprites_.begin(); sprite != sprites_.end(); ++sprite) {
    delete (*sprite);
  }
  sprites_.clear();      
}



SlTexture*
SlManager::createTextureFromFile(std::string name, std::string fileName)
{
  SlTexture* toAdd = new SlTexture(name);
  toAdd->loadFromFile(renderer_, fileName);
  textures_.push_back(toAdd);
  return toAdd;
}


SlTexture*
SlManager::createTextureFromRectangle(std::string name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  SlTexture* toAdd = new SlTexture(name);
  toAdd->createFromRectangle(renderer_, width, height, red, green, blue, alpha);
  textures_.push_back(toAdd);
  return toAdd;
}



SlTexture*
SlManager::createTextureFromSpriteOnTexture(std::string name, SlTexture* backgroundTexture, SlSprite *foregroundSprite)
{
  SlTexture* toAdd = new SlTexture(name);
  toAdd->createFromSpriteOnTexture(renderer_, backgroundTexture, foregroundSprite);
  textures_.push_back(toAdd);
  return toAdd;
}



SlTexture*
SlManager::createTextureFromTile(std::string name, SlSprite* sprite, int width, int height)
{
  SlTexture* toAdd = new SlTexture(name);
  toAdd->createFromTile(renderer_, sprite, width, height);
  textures_.push_back(toAdd);
  return toAdd;
}



void
SlManager::deleteTexture(std::string name)
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    if ( (*iter)->name_ == name){
      delete (*iter);
      textures_.erase(iter);
    }
  }
}



void
SlManager::initialize()
{
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    std::cerr << "SDL could not initialize! SDL_Error: " <<  SDL_GetError() << std::endl;
    exit(1);
  }
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    std::cout << "IMG_Init " << SDL_GetError();
    SDL_Quit();
    exit(1);
  }
}



void
SlManager::initializeWindow(std::string name, int width, int height)
{
  window_ = SDL_CreateWindow( name.c_str() , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
  if( window_ == nullptr ){
    std::cerr << "Window could not be created! SDL_Error: " <<  SDL_GetError()  << std::endl;
    exit(1);
  }
  renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
  if( renderer_ == nullptr ){
    std::cerr << "Renderer could not be created " <<  SDL_GetError() << std::endl;
    exit(1);
  }
}


