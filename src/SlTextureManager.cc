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
#include <algorithm>

#include "SDL2/SDL_ttf.h"

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"
#include "SlFont.h"
#include "SlTextureManager.h"



SlTextureManager::SlTextureManager()
{
}


SlTextureManager::SlTextureManager(SlManager* manager)
  : mngr_(manager)
{
  if (TTF_Init() != 0){
    std::cerr << "[SlTextureManager::SlTextureManager] Error in TTF_init: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
}



SlTextureManager::~SlTextureManager(void)
{
  this->clear();
  TTF_Quit();
  mngr_ = nullptr;
}



void
SlTextureManager::addTexture(SlTexture* toAdd)
{
  if ( toAdd == nullptr ) return;
  textures_.push_back(toAdd);
}



void
SlTextureManager::clear()
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    delete (*iter);
  }
  textures_.clear();
  fonts_.clear();
}



SlTexture*
SlTextureManager::createTextureFromFile(const std::string& name, const std::string& filename)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromFile] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  toAdd = new SlTexture(name);
  toAdd->loadFromFile(mngr_->renderer(), filename);
  addTexture(toAdd);
  return toAdd;
}



SlTexture*
SlTextureManager::createTextureFromRectangle(const std::string& name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromRectangle] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  toAdd = new SlTexture(name);
  toAdd->createFromRectangle(mngr_->renderer(), width, height, red, green, blue, alpha);
  addTexture(toAdd);
  return toAdd;
}



SlTexture*
SlTextureManager::createTextureFromSpriteOnTexture(const std::string& name, const std::string& backgroundTexture, const std::string& foregroundSprite)
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
  toAdd = new SlTexture(name);
  toAdd->createFromSpriteOnTexture(mngr_->renderer(), background, foreground);
  addTexture(toAdd);
  return toAdd;
}



SlTexture* 
SlTextureManager::createTextureFromText(const std::string& name, const std::string& fontname, const std::string& message, int width)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromText] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  std::shared_ptr<SlFont> font = findFont(fontname);
  if ( font == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromText] Failed to create " << name << ": Couldn't find font " << fontname << std::endl;
#endif
    return toAdd;
  }

  toAdd = new SlTexture(name);
  toAdd->createFromText(mngr_->renderer(), font, message, width );
  addTexture(toAdd);
  return toAdd;
}



SlTexture*
SlTextureManager::createTextureFromTile(const std::string& name, const std::string& sprite, int width, int height)
{
  SlTexture* toAdd = findTexture(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::createTextureFromTile] Error: texture of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }

  std::shared_ptr<SlSprite> tile = mngr_->findSprite(sprite);

  toAdd = new SlTexture(name);
  toAdd->createFromTile(mngr_->renderer(), tile, width, height);
  addTexture(toAdd);
  return toAdd;
}



void
SlTextureManager::deleteTexture(const std::string& name)
{
  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    if ( (*iter)->name() == name){
      delete (*iter);
      textures_.erase(iter);
      break;
    }
  }
}



std::shared_ptr<SlFont>
SlTextureManager::findFont(const std::string& name)
{
  std::shared_ptr<SlFont> found = nullptr;
  auto iter = std::find_if( fonts_.begin(), fonts_.end(), 
			    [name](const std::shared_ptr<SlFont> font) -> bool {return font->name() == name; } );
  if ( iter == fonts_.end() ) {
#ifdef DEBUG
    std::cout << "[SlTextureManager::findFont] Couldn't find font " << name << std::endl;
#endif
    return found;
  }
  found = *iter;
  return found;
}


SlTexture*
SlTextureManager::findTexture(const std::string& name)
{
  SlTexture* result = nullptr;
  auto iter = std::find_if( textures_.begin(), textures_.end(),
			    [name](const SlTexture* tex) -> bool {return tex->name() == name; });
  if ( iter == textures_.end() ) {
    return result;
  }
  result = *iter;
  return result;
}



std::shared_ptr<SlFont>
SlTextureManager::parseFont(std::ifstream& input)
{
  std::shared_ptr<SlFont> toAdd = nullptr;
  std::string line, token;
  bool endOfConfig = false;
  std::string name, file;
  std::vector<std::string> colors;
  int fontsize = 0;
  
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
    else if ( token == "name" ) {
      stream >> name ;
    }
    else if ( token == "file" ) {
      stream >> file ;
    }
    else if ( token == "size" ) {
      stream >> fontsize;
    }
    else if ( token == "color" || token == "colour") {
      while ( !stream.eof() ){
	colors.push_back("");
	stream >> colors.back();
      }
    }
    else {
#ifdef DEBUG
      std::cerr << "[SlTextureManager::parseFont] Unknown token " << token << std::endl;
#endif
    }
    token.clear();
    if (  !endOfConfig ) getline(input,line);
  }

  if ( name.empty() ) {
#ifdef DEBUG
    std::cerr << "[SlTextureManager::parseFont] No name found" << std::endl;
#endif
    return toAdd;
  }
  if ( fontsize == 0 ) {
#ifdef DEBUG
    std::cerr << "[SlTextureManager::parseFont] No size found for font " << name << std::endl;
#endif
    return toAdd;
  }

  try {
  toAdd = std::make_shared<SlFont>(name);
  valParser->stringsToNumbers<short>( colors, toAdd->color, 4 );
  toAdd->loadFont(file, fontsize);
  fonts_.push_back(toAdd);
  }
  catch (const std::exception& expt){
    std::cout << "[SlTextureManager::parseFont] " << expt.what() << std::endl; 
  }
  catch (...) {
    std::cerr << "[SlTextureManager::parseFont] Unknown exception for " << name  << std::endl;
  }
  return toAdd;
}




SlTexture*
SlTextureManager::parseTexture(std::ifstream& input)
{
  SlTexture* toAdd = nullptr;
  std::string line, token;
  bool endOfConfig = false;
  std::string name, type, file, sprite, font, texture, message;
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
    else if ( token == "name" ) {
      stream >> name ;
    }
    else if ( token == "sprite" ) {
      stream >> sprite;
    }
    else if ( token == "file" ) {
      stream >> file ;
    }
    else if ( token == "font" ) {
      stream >> font ;
    }
    else if ( token == "text" ) {
    getline(stream, message);
    message = message.substr(1);   //! <- message[0] is space between token and beginning of text.
    }
    else if ( token == "texture" ) {
      stream >> texture ;
    }
    else if ( token == "dimensions" || token == "width" ) {
      while ( !stream.eof() ){
	dimensions.push_back("");
	stream >> dimensions.back();
      }
    }
    else if ( token == "color" || token == "colour") {
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
    return toAdd;
  }

  try {  
    if ( type == "file" ) {
      toAdd = createTextureFromFile( name, file );
    }
  
    else if ( type == "tile" || type == "rectangle" ) {
      int dim[2];
      valParser->stringsToNumbers<int>( dimensions, dim, 2 );

      if ( type == "tile") {
	toAdd = createTextureFromTile( name, sprite, dim[0], dim[1] );
      }

      else if ( type == "rectangle" ) {
	short colArray[] = {0,0,0,0};
	valParser->stringsToNumbers<short>( colors, colArray, 4 );
	toAdd = createTextureFromRectangle( name, dim[0], dim[1], colArray[0], colArray[1], colArray[2], colArray[3] );
      }
    }

    else if ( type == "sprite-on-texture" ) {
      toAdd = createTextureFromSpriteOnTexture( name, texture, sprite ) ;
    }
    else if ( type == "text" ) {
      int width[1];
      valParser->stringsToNumbers<int>(dimensions, width, 1);
      toAdd = createTextureFromText( name, font, message, width[0] ) ;
    }
    else {
#ifdef DEBUG
      std::cerr << "[SlTextureManager::parseTexture] Unknown type "  << type << " for texture " << name << std::endl;
#endif
    }
  }
  catch (const std::exception& expt) {
    std::cout << "[SlTextureManager::parseTexture] " << expt.what() << std::endl;
  }
  catch (...) {
    std::cout << "[SlTextureManager::parseTexture] Unknown exception" << std::endl;
  }
  return toAdd;
}



