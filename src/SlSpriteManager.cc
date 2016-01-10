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

#include "SlSpriteManager.h"




SlSpriteManager::SlSpriteManager(SlManager* mngr)
  : mngr_(mngr)
{
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
SlSpriteManager::moveSprite(const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = findSprite(name);

  if ( toMove == nullptr ){
#ifdef DEBUG
    std::cout << "[SlSpriteManager::moveSprite] Couldn't find sprite to move " << name << std::endl;
#endif
    return;
  }
  if ( destination >= toMove->size() ){
#ifdef DEBUG
    std::cout << "[SlSpriteManager::moveSprite] Invalid destination for sprite " << name << std::endl;
#endif
    return;
  }

  if ( whatToDo == "setOrigin" ) {
    int origin[2] ;
    bool check = mngr_->determineValues( parameters, origin, 2 );
    if ( check ) toMove->setDestinationOrigin( origin[0], origin[1], destination) ; 
  }
  if ( whatToDo == "centerIn" ) {
    std::string target = parameters.at(0);
    int targetDest;
    if ( parameters.size() > 1) targetDest = std::stoul(parameters.at(1));
    else targetDest = 0;
    centerSpriteInSprite( name, target, destination, targetDest) ; 
  }
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
  int loc[4];
  bool check = mngr_->determineValues(location, loc, 4);
  if ( !check ) {
#ifdef DEBUG
    std::cerr << "[SlSpriteManager::parseSprite] Invalid location for " << name << std::endl;
#endif
    return;
  }
  createSprite( name, texture, loc[0], loc[1], loc[2], loc[3] );
  
}


void
SlSpriteManager::parseSpriteMovement(std::ifstream& input)
{
  std::string line, token;
  std::string name, whatToDo;
  unsigned int destination;
  std::vector<std::string> coordinates;
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
	while ( !stream.eof() ){
	  coordinates.push_back("");
	  stream >> coordinates.back();
	}
	moveSprite( name, destination, whatToDo, coordinates );
      }
      catch (std::exception) {
#ifdef DEBUG
	std::cerr << "[SlSpriteManager::parseSpriteMovement] Error at line: " << line << std::endl;
#endif
	;
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



