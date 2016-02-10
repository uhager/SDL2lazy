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
#include <iterator>

#include "SlSprite.h"
#include "SlTexture.h"
#include "SlManager.h"
#include "SlSpriteManipulation.h"

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
  


bool
SlSpriteManager::checkSpriteName(const std::string& name)
{
  try {
    findSprite( name );
    return true;
  }
  catch (const std::exception& ) {
    return false;
  }
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
  if ( checkSpriteName( texture->name() ) ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Error: Sprite of name " << texture->name() << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  std::shared_ptr<SlSprite> toAdd = nullptr;
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
  if ( checkSpriteName(name) ) {
#ifdef DEBUG
    std::cout << "[SlSpriteManager::createSprite] Error: Sprite of name " << name << " already exists."  << std::endl;
#endif
    return nullptr;
  }
  std::shared_ptr<SlSprite> toAdd = nullptr;
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



void
SlSpriteManager::deleteSprites(const std::string& textureName)
{
  for ( int i = sprites_.size()-1; i >= 0 ; --i ) {
    if ( sprites_.at(i)->textureName() == textureName) {
      mngr_->deleteRenderItem( sprites_.at(i)->name() );
      sprites_.erase( sprites_.begin() + i );
    }
  }
}



std::shared_ptr<SlSprite>
SlSpriteManager::findSprite(const std::string& name)
{
  auto iter = std::find_if( sprites_.begin(), sprites_.end() ,
			    [name](const std::shared_ptr<SlSprite> sprite) -> bool { return sprite->name() == name; } ) ;

  if ( iter == sprites_.end() )
    throw std::invalid_argument("[SlSpriteManager::findSprite] Couldn't find sprite " + name );

  return *iter;
}



void
SlSpriteManager::initialize( SlValueParser* valPars)
{
  valParser = valPars;
  
  SlManipulation* toAdd;
  toAdd = new SlSMsetOrigin( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlSMcenterIn( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlSMsetOptions( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlSMcenterAt( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlSMcolor( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
  toAdd = new SlSMsetAngle( this, valParser );
  manipulations_[toAdd->name()] = toAdd;
}



void
SlSpriteManager::manipulateSprite(const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters)
{
  auto iter = manipulations_.find(whatToDo);
  if ( iter == manipulations_.end() ) {
#ifdef DEBUG
    std::cerr << "[SlSpriteManager::manipulate] Couldn't find object to manipulate sprite " << name << std::endl;
#endif
    return;
  }
  iter->second->manipulate(name, destination, parameters);
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
      std::istream_iterator<std::string> str_iter(stream), eof;
      location = { str_iter, eof };
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
	std::istream_iterator<std::string> str_iter(stream), eof;
	std::vector<std::string> parameters( str_iter, eof );
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



void
SlSpriteManager::setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int destination)
{
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  sprite->setColor(red, green, blue, alpha, destination);
}




void
SlSpriteManager::setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination)
{
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  sprite->setDestinationOrigin(x, y, destination);
}



void
SlSpriteManager::setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination)
{
  std::shared_ptr<SlSprite> sprite = findSprite(name);
  sprite->setRenderOptions(renderOptions, destination);
}



