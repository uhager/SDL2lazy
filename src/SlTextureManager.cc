// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTextureManager.cc

  SlTextureManager implementation
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"
#include "SlTextureManager.h"



SlTextureManager::SlTextureManager()
{
}


SlTextureManager::SlTextureManager(SlManager* manager)
  : mngr_(manager)
{

}



SlTextureManager::~SlTextureManager(void)
{
  this->clear();
}



void
SlTextureManager::addTexture(SlTexture* toAdd)
{
  if ( toAdd == nullptr ) return;
  textures_.push_back(toAdd);
  mngr_->createSprite(toAdd->name_, toAdd->name_);
}

  

void
SlTextureManager::clear()
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    delete (*iter);
  }
  textures_.clear();
}



SlTexture*
SlTextureManager::createTextureFromFile(std::string name, std::string filename)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromFile] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  toAdd = new SlTexture(name);
  bool check = toAdd->loadFromFile(mngr_->renderer(), filename);
  if (check == false) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromFile] Couldn't load texture."  << std::endl;
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
SlTextureManager::createTextureFromRectangle(std::string name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromRectangle] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  toAdd = new SlTexture(name);
  int check = toAdd->createFromRectangle(mngr_->renderer(), width, height, red, green, blue, alpha);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromRectangle] Couldn't create texture."  << std::endl;
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
SlTextureManager::createTextureFromSpriteOnTexture(std::string name, std::string backgroundTexture, std::string foregroundSprite)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromSpriteOnTexture] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }

  SlTexture* background = findTexture(backgroundTexture);
  if (background == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromSpriteOnTexture] Failed to create " << name << ": Couldn't find background texture " << backgroundTexture  << std::endl;
#endif
    return toAdd;
  }
  std::shared_ptr<SlSprite> foreground = mngr_->findSprite(foregroundSprite);
  if (foreground == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromSpriteOnTexture] Failed to create " << name << ": Couldn't find foreground sprite " << foregroundSprite << std::endl;
#endif
    return toAdd;
  }
  toAdd = new SlTexture(name);
  int check = toAdd->createFromSpriteOnTexture(mngr_->renderer(), background, foreground);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromSpriteOnTexture] Couldn't create texture."  << std::endl;
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
SlTextureManager::createTextureFromTile(std::string name, std::string sprite, int width, int height)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromTile] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }

  std::shared_ptr<SlSprite> tile = mngr_->findSprite(sprite);
  if (tile == nullptr) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromTile] Failed to create " << name << ": Couldn't find sprite " << sprite << std::endl;
#endif
    return toAdd;
  }

  toAdd = new SlTexture(name);
  int check = toAdd->createFromTile(mngr_->renderer(), tile, width, height);
  if (check != 0) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromTile] Couldn't create texture."  << std::endl;
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
SlTextureManager::deleteTexture(std::string name)
{
  mngr_->deleteSprite(name);
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    if ( (*iter)->name_ == name){
      delete (*iter);
      textures_.erase(iter);
      break;
    }
  }
}



SlTexture*
SlTextureManager::findTexture(std::string name)
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
SlTextureManager::parseTexture(std::ifstream& input)
{
  std::string line, token;
  bool endOfConfig = false;
  std::string name, type, file, sprite, texture;
  std::vector<std::string> dimensions;
  std::vector<std::string> colors;
  
  getline(input,line);
  while ( !endOfConfig && input ) {
    std::istringstream stream(line.c_str());
    stream >> token;
    if ( token[0] == '#' || token.empty() || token[0] == '\n' ) {
      /* empty line or comment */
    }
    else if ( token == "end" ) {
      endOfConfig = true;
    }
    else if ( token == "type" ) {
      stream >> type;
    }
    else if ( token == "sprite" ) {
      stream >> sprite;
    }
    else if ( token == "name" ) {
      stream >> name ;
    }
    else if ( token == "file" ) {
      stream >> file ;
    }
    else if ( token == "texture" ) {
      stream >> texture ;
    }
    else if ( token == "dimensions" ) {
      while ( !stream.eof() ){
	dimensions.push_back("");
	stream >> dimensions.back();
      }
    }
    else if ( token == "color" ) {
      while ( !stream.eof() ){
	colors.push_back("");
	stream >> colors.back();
      }
    }
    else {
#ifdef DEBUG
      std::cerr << "[SlTextureManager::parseTexture] Unknown token " << token << std::endl;
#endif
    }
    token.clear();
    if (  !endOfConfig ) getline(input,line);
  }

  
  if ( name.empty() ) {
#ifdef DEBUG
    std::cerr << "[SlTextureManager::parseTexture] No name found" << std::endl;
#endif
    return;
  }
  
  if ( type == "file" ) {
    createTextureFromFile( name, file );
  }
  
  else if ( type == "tile" || type == "rectangle" ) {
    int dim[2];
    bool check = mngr_->determineValues( dimensions, dim, 2 );
    if ( !check ) {
#ifdef DEBUG
      std::cerr << "[SlTextureManager::parseTexture] invalid dimensions for type " << type << std::endl;
#endif
      return;
    }

    if ( type == "tile") {
      createTextureFromTile( name, sprite, dim[0], dim[1] );
    }

    else if ( type == "rectangle" ) {
      int colArray[] = {0,0,0,0};
      bool check = mngr_->determineValues( colors, colArray, 4 );
      if ( !check ) {
#ifdef DEBUG
	std::cerr << "[SlTextureManager::parseTexture] invalid color for " << name  << std::endl;
#endif
	return;
      }
      createTextureFromRectangle( name, dim[0], dim[1], colArray[0], colArray[1], colArray[2], colArray[3] );
    }
  }

  else if ( type == "sprite-on-texture" ) {
    createTextureFromSpriteOnTexture( name, texture, sprite ) ;
  }
  else {
#ifdef DEBUG
    std::cerr << "[SlTextureManager::parseTexture] Unknown type "  << type << " for texture " << name << std::endl;
#endif
  }
}


