// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpriteManager.cc

  SlSpriteManager implementation
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include "SlSprite.h"
#include "SlTexture.h"
#include "SlManager.h"
#include "SlSpriteManipulation.h"

#include "SlSpriteManager.h"




SlSpriteManager::SlSpriteManager(SlManager* mngr)
  : mngr_(mngr)
{
  SlSpriteManipulation* toAdd;
  
  toAdd = new SlMSetOrigin( this, &valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlMCenterIn( this, &valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlMSetOptions( this, &valParser );
  manipulations_[toAdd->name()] = toAdd;
}



SlSpriteManager::~SlSpriteManager(void)
{
  this->clear();
  mngr_ = nullptr;
}



void
SlSpriteManager::centerSpriteInSprite(const std::string& toCenter, const std::string& target, unsigned int destinationThis, unsigned int destinationOther)
{
  std::shared_ptr<SlSprite> sprite = findSprite(toCenter);
  if ( sprite == nullptr ) {
#ifdef DEBUG
    std::cerr << "[SlSpriteManager::centerSpriteInSprite] Couldn't find sprite " << toCenter << std::endl;
#endif
    return;
  }

  std::shared_ptr<SlSprite> other = findSprite(target);
  if ( other == nullptr ) {
#ifdef DEBUG
    std::cerr << "[SlSpriteManager::centerSpriteInSprite] Couldn't find sprite " << other << std::endl;
#endif
    return;
  }
  sprite->centerInSprite( other, destinationThis, destinationOther );
}
  

void
SlSpriteManager::clear()
{
  for (auto iter = manipulations_.begin(); iter != manipulations_.end(); ++iter ) {
    delete iter->second;
  }
  manipulations_.clear();
  sprites_.clear();  
}



std::shared_ptr<SlSprite>
SlSpriteManager::createSprite(SlTexture* texture, int x, int y, int width, int height)
{
  std::shared_ptr<SlSprite> toAdd = findSprite(texture->name());
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Error: Sprite of name " << texture->name() << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  if ( texture == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Error: Invalid texture."  << std::endl;
#endif
  }
  else {
    toAdd = std::make_shared<SlSprite>(texture->name(), texture, x, y, width, height);
    sprites_.push_back(toAdd);
  }
  return toAdd;
}


std::shared_ptr<SlSprite>
SlSpriteManager::createSprite(const std::string& name, const std::string& textureName, int x, int y, int width, int height)
{
  std::shared_ptr<SlSprite> toAdd = findSprite(name);
  if ( toAdd ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Error: Sprite of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  SlTexture* tex = mngr_->findTexture(textureName);
  if ( tex == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Couldn't find texture " << textureName << " required for sprite " << name  << std::endl;
#endif
  }
  else {
    toAdd = std::make_shared<SlSprite>(name, tex, x, y, width, height);
    sprites_.push_back(toAdd);
  }
  return toAdd;
}



void
SlSpriteManager::deleteSprite(const std::string& name)
{
  std::vector<std::shared_ptr<SlSprite>>::iterator iter;
  for ( iter=sprites_.begin(); iter != sprites_.end(); ++iter){
    if ( (*iter)->name() == name){
      //      delete (*iter);
      sprites_.erase(iter);
      break;
    }
  }
}



std::shared_ptr<SlSprite>
SlSpriteManager::findSprite(const std::string& name)
{
  std::shared_ptr<SlSprite> result = nullptr;
  auto iter = std::find_if( sprites_.begin(), sprites_.end() ,
			    [name](const std::shared_ptr<SlSprite> sprite) -> bool { return sprite->name() == name; } ) ;

  if ( iter == sprites_.end() )
    result = nullptr;
  else
    result = *iter;
  return result;
}



void
SlSpriteManager::manipulateSprite(const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters)
{

  auto iter = manipulations_.find(whatToDo);
  if ( iter == manipulations_.end() ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::manipulateSprite] Couldn't find object to manipulate sprite " << name << std::endl;
#endif
    return;
  }
  iter->second->manipulateSprite(name, destination, parameters);
  return;
}

 

void
SlSpriteManager::parseSprite(std::ifstream& input)
{
  std::string line, token;
  std::string name, texture;
  std::vector<std::string> location;
  bool endOfConfig = false;
  
  getline(input,line);
  while ( !endOfConfig && input ) {
    std::istringstream stream(line.c_str());
    stream >> token;
    if ( token[0] == '#' || token.empty() ) {
      /* empty line or comment */
    }
    else if ( token == "end" ) {
      endOfConfig = true;
    }
    else if ( token == "name" ) {
      stream >> name ;
    }
    else if ( token == "texture" ) {
      stream >> texture ;
    }
    else if ( token == "location" ) {
      while ( !stream.eof() ){
	location.push_back("");
	stream >> location.back();
      }
    }
    
    else {
#ifdef DEBUG
      std::cerr << "[SlSpriteManager::parseSprite] Unknown token " << token << std::endl;
#endif
    }
    
    token.clear();
    if ( !endOfConfig ) getline(input,line);
  }

  if ( name.empty() || texture.empty() ) {
#ifdef DEBUG
    std::cerr << "[SlSpriteManager::parseSprite] Name or texture missing" << std::endl;
#endif
    return;
  }
  try {
  int loc[4];
  valParser->stringsToNumbers<int>(location, loc, 4);
  createSprite( name, texture, loc[0], loc[1], loc[2], loc[3] );
  }
  catch (const std::exception& expt) {
    std::cerr << "[SlSpriteManager::parseSprite] " << expt.what() << std::endl;
  }
  catch (...) {
    std::cerr << "[SlSpriteManager::parseSprite] Unknown exception" << std::endl;
  }
}


void
SlSpriteManager::parseSpriteManipulation(std::ifstream& input)
{
  std::string line, token;
  std::string name, whatToDo;
  unsigned int destination;
  bool endOfConfig = false;
  
  getline(input,line);
  while ( !endOfConfig && input ) {
    std::istringstream stream(line.c_str());
    stream >> token;
    if ( token[0] == '#' || token.empty() ) {
      /* empty line or comment */
    }
    else if ( token == "end" ) {
      endOfConfig = true;
    }
    else {
      try {
	name = token ;
	stream >> destination ;
	stream >> whatToDo ;
	std::vector<std::string> parameters;
	while ( !stream.eof() ){
	  parameters.push_back("");
	  stream >> parameters.back();
	}
	manipulateSprite( name, destination, whatToDo, parameters );
      }
      catch (const std::exception& expt) {
	std::cerr << "[SlSpriteManager::parseSpriteManipulation] " << expt.what() << std::endl;
      }
      catch (...) {
	std::cerr << "[SlSpriteManager::parseSpriteManipulation] Unknown exception at line: " << line << std::endl;
      }
    }
    token.clear();
    if ( !endOfConfig ) getline(input,line);
  }
}



bool
SlSpriteManager::setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int destination)
{
  bool isSet = false;
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::setSpriteColor] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setColor(red, green, blue, alpha, destination);
  return isSet;
}




bool
SlSpriteManager::setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination)
{
  bool isSet = false;
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::setSpriteDestinationOrigin] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setDestinationOrigin(x, y, destination);
  return isSet;
}



bool
SlSpriteManager::setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination)
{
  bool isSet = false;
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::setSpriteRenderOptions] Couldn't find sprite " << name  << std::endl;
#endif
    return isSet;
  }
  isSet = sprite->setRenderOptions(renderOptions, destination);
  return isSet;
}



