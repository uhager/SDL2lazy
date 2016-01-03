// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManager.cc

  SlManager implementation
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"



/*! The running instance. 
 */
SlManager* SlManager::instance_ = nullptr;

SlManager::SlManager()
{
  if ( instance_ == nullptr) {
    instance_ = this;
  }
  else
    {
      std::cout << "[SlManager::SlManager]  One manager per application." << std::endl;
      exit(1);
    }
  this->initialize();
}



SlManager::SlManager(std::string name, int width, int height)
{
  if ( instance_ == nullptr) {
    instance_ = this;
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
SlManager::addTexture(SlTexture* toAdd)
{
  textures_.push_back(toAdd);
  SlSprite* sprite = createSprite(toAdd->name_, toAdd->name_);
  sprites_.push_back(sprite);
}




bool
SlManager::appendToRenderQueue(std::string name, unsigned int destination)
{
  bool result = true;
  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( toAdd ) {
  renderQueue_.push_back(toAdd);
  }
  else {
#ifdef DEBUG
    std::cout << "[SlManager::appendToRenderQueue] Couldn't add sprite " << name  << " at " << destination << " to queue."  << std::endl;
#endif
  }
  return result;
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

  std::vector<SlRenderItem*>::iterator item;
  for ( item = renderQueue_.begin(); item != renderQueue_.end() ; ++item) {
    delete (*item);
  }
  renderQueue_.clear();
}


SlRenderItem* 
SlManager::createRenderItem(std::string name, unsigned int destination)
{
#ifdef DEBUG
    std::cout << "[SlManager::createRenderItem] Creating item for " << name  << std::endl;
#endif
  SlRenderItem* item = nullptr;
  SlSprite* sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::createRenderItem] Couldn't find sprite " << name  << std::endl;
#endif
    return item;
  }
  if (sprite->destinations_.size() <= destination){
#ifdef DEBUG
    std::cout << "[SlManager::createRenderItem] Failed to add sprite " << name << ": destination " << destination << " out of bounds (" << sprite->destinations_.size() << ")" <<  std::endl;
#endif
    return item;
  }
  item = new SlRenderItem(name, sprite, destination);
  return item;
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
SlManager::createTextureFromFile(std::string name, std::string filename)
{
  SlTexture* toAdd = new SlTexture(name);
  bool check = toAdd->loadFromFile(renderer_, filename);
  if (check == false) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromFile] Couldn't load texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    addTexture(toAdd);
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
    addTexture(toAdd);
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
    addTexture(toAdd);
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
  int check = toAdd->createFromTile(renderer_, tile
				    , width, height);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlManager::createTextureFromTile] Couldn't create texture."  << std::endl;
#endif
    delete toAdd;
    toAdd = nullptr;
  }
  else {
    addTexture(toAdd);
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
  deleteSprite(name);
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



bool
SlManager::parseConfigurationFile(std::string filename)
{
  bool result = true;
  std::ifstream input(filename,std::ifstream::in);
  if ( !input.is_open() ) {
#ifdef DEBUG
    std::cout << "[SlManager::parseConfigurationFile] Couldn't open file " << filename << std::endl;
#endif
    return false;
  }
  std::string line, token;
  getline(input,line);
  while ( input )
    {
      std::istringstream stream(line.c_str());
      stream >> token;
      if ( token[0] == '#' || token.empty() ) {
	/* empty line or comment */
      }
      else if ( token == "textures" ) {
	parseTextures( input );
      }
      else if ( token == "sprites" ) {
	parseSprites(input);
      }
	
      if ( input) getline(input,line);
    }

   return result;
}



void
SlManager::parseSprites(std::ifstream& input)
{
  std::cout << "[SlManager::parseSprites]" << std::endl;
}



void
SlManager::parseTextures(std::ifstream& input)
{
  std::cout << "[SlManager::parseTextures]" << std::endl;
}



int
SlManager::render()
{
  int result;
  
  SDL_RenderClear( renderer_ );
 
  for (auto& item: renderQueue_){
    result = (item->sprite_)->render( renderer_, (item->destination_) );
    if (result != 0) {
#ifdef DEBUG
      std::cout << "[SlManager::render] Couldn't render sprite " << item->name_ << std::endl;
#endif
      return result;
    }
  }

  SDL_RenderPresent( renderer_ );
  return result;
}



bool
SlManager::setSpriteColor(std::string name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int destination)
{
  bool isSet = false;
  SlSprite* sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::setSpriteColor] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setColor(red, green, blue, alpha, destination);
  return isSet;
}




bool
SlManager::setSpriteDestinationOrigin(std::string name,  int x, int y, unsigned int destination)
{
  bool isSet = false;
  SlSprite* sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::setSpriteDestinationOrigin] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setDestinationOrigin(x, y, destination);
  return isSet;
}



bool
SlManager::setSpriteRenderOptions(std::string name, uint32_t renderOptions, unsigned int destination)
{
  bool isSet = false;
  SlSprite* sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlManager::setSpriteRenderOptions] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setRenderOptions(renderOptions, destination);
  return isSet;
}
