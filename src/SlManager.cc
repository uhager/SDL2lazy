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



SlSprite*
SlManager::createSprite(std::string name, std::string textureName, int x, int y, int width, int height)
{
  SlSprite* toAdd = nullptr;
  SlTexture* tex = findTexture(textureName);
  if ( tex == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlManager::createSprite] Couldn't find texture " << textureName << " required for sprite " << name  << std::endl;
#endif
  }
  else {
    toAdd = new SlSprite(name, tex, x, y, width, height);
    sprites_.push_back(toAdd);
  }
  return toAdd;
}



SlTexture*
SlManager::createTextureFromFile(std::string name, std::string fileName)
{
  SlTexture* toAdd = new SlTexture(name);
  bool check = toAdd->loadFromFile(renderer_, fileName);
  if (check == false) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromFile] Couldn't load texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    textures_.push_back(toAdd);
  }
  return toAdd;
}



SlTexture*
SlManager::createTextureFromRectangle(std::string name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  SlTexture* toAdd = new SlTexture(name);
  int check = toAdd->createFromRectangle(renderer_, width, height, red, green, blue, alpha);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromRectangle] Couldn't create texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    textures_.push_back(toAdd);
  }
  return toAdd;
}



SlTexture*
SlManager::createTextureFromSpriteOnTexture(std::string name, std::string backgroundTexture, std::string foregroundSprite)
{
  SlTexture* toAdd = nullptr;   
  SlTexture* background = findTexture(backgroundTexture);
  if (background == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromSpriteOnTexture] Failed to create " << name << ": Couldn't find background texture " << backgroundTexture  << std::endl;
#endif
    return toAdd;
  }
  SlSprite* foreground = findSprite(foregroundSprite);
  if (foreground == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromSpriteOnTexture] Failed to create " << name << ": Couldn't find foreground sprite " << foregroundSprite << std::endl;
#endif
    return toAdd;
  }
  toAdd = new SlTexture(name);
  int check = toAdd->createFromSpriteOnTexture(renderer_, background, foreground);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromSpriteOnTexture] Couldn't create texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    textures_.push_back(toAdd);
  }
  return toAdd;
}



SlTexture*
SlManager::createTextureFromTile(std::string name, std::string sprite, int width, int height)
{
  SlTexture* toAdd = nullptr;

  SlSprite* tile = findSprite(sprite);
  if (tile == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromTile] Failed to create " << name << ": Couldn't find sprite " << sprite << std::endl;
#endif
    return toAdd;
  }

  toAdd = new SlTexture(name);
  int check = toAdd->createFromTile(renderer_, tile, width, height);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromTile] Couldn't create texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    textures_.push_back(toAdd);
  }
  return toAdd;
}



void
SlManager::deleteSprite(std::string name)
{
  std::vector<SlSprite*>::iterator iter;
  for ( iter=sprites_.begin(); iter != sprites_.end(); ++iter){
    if ( (*iter)->name_ == name){
      delete (*iter);
      sprites_.erase(iter);
      break;
    }
  }
}



void
SlManager::deleteTexture(std::string name)
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    if ( (*iter)->name_ == name){
      delete (*iter);
      textures_.erase(iter);
      break;
    }
  }
}



SlSprite*
SlManager::findSprite(std::string name)
{
  SlSprite* result = nullptr;
  std::vector<SlSprite*>::iterator iter;
  for ( iter=sprites_.begin(); iter != sprites_.end(); ++iter){
    if ( (*iter)->name_ == name){
      result = *iter;
      break;
    }
  }
  return result;
}



SlTexture*
SlManager::findTexture(std::string name)
{
  SlTexture* result = nullptr;
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    if ( (*iter)->name_ == name){
      result = *iter;
      break;
    }
  }
  return result;
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


