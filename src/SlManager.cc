// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManager.cc

  SlManager implementation
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlRenderItem.h"
#include "SlTextureManager.h"
#include "SlSpriteManager.h"
#include "SlRenderQueueManipulation.h"

#include "SlManager.h"



SlManager::SlManager()
{
  this->initialize();
}



SlManager::SlManager(const std::string& name, int width, int height)
{
  this->initialize();
  this->initializeWindow(name, width, height);

  SlRenderQueueManipulation* toAdd;
  toAdd = new SlRMappend( smngr_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMinsertAfter( smngr_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
}



SlManager::~SlManager(void)
{
  this->clear();
  tmngr_ = nullptr;
  smngr_ = nullptr;
  SDL_DestroyRenderer(renderer_);
  renderer_ = nullptr;
  SDL_DestroyWindow( window_ );
  window_ = nullptr;
  SDL_Quit();
}



bool
SlManager::appendToRenderQueue(const std::string& name, unsigned int destination)
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
  std::vector<SlRenderItem*>::iterator item;
  for ( item = renderQueue_.begin(); item != renderQueue_.end() ; ++item) {
    delete (*item);
  }
  renderQueue_.clear();
}


SlRenderItem* 
SlManager::createRenderItem(const std::string& name, unsigned int destination)
{
#ifdef DEBUG
  std::cout << "[SlManager::createRenderItem] Creating item for " << name  << std::endl;
#endif
  SlRenderItem* item = nullptr;
  std::shared_ptr<SlSprite> sprite = smngr_->findSprite(name);
  if (sprite == nullptr) {
#ifdef DEBUG
    std::cerr << "[SlManager::createRenderItem] Couldn't find sprite " << name  << std::endl;
#endif
    return item;
  }
  if (sprite->size() <= destination){
#ifdef DEBUG
    std::cout << "[SlManager::createRenderItem] Failed to add sprite " << name << ": destination " << destination << " out of bounds (" << sprite->size() << ")" <<  std::endl;
#endif
    return item;
  }
  item = new SlRenderItem(sprite, destination);
  return item;
}



void
SlManager::deleteRenderItem(const std::string& name)
{
  std::vector<SlRenderItem*>::iterator item = renderQueue_.end() ;
  while ( item != renderQueue_.begin() ) {
    --item;
    if ( (*item)->sprite_->name() == name ) {
      delete (*item);
      renderQueue_.erase(item);
    }
  }
}



void
SlManager::deleteSprite(const std::string& name)
{
  deleteRenderItem(name);
  smngr_->deleteSprite(name);
}



void
SlManager::deleteTexture(const std::string& name)
{
  deleteSprite(name);
  tmngr_->deleteTexture(name);
}



std::shared_ptr<SlSprite>
SlManager::findSprite(const std::string& name)
{
  return smngr_->findSprite(name);
}



SlTexture*
SlManager::findTexture(const std::string& name)
{
  return tmngr_->findTexture(name);
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
  tmngr_ = std::unique_ptr<SlTextureManager>(new SlTextureManager( this ));
  //  smngr_ = std::make_unique<SlSpriteManager>( this );
  smngr_ = std::shared_ptr<SlSpriteManager>(new SlSpriteManager( this )); //!< Needs to be shared with SlRenderQueueManipulation items.
}



void
SlManager::initializeWindow(const std::string& name, int width, int height)
{
  screen_width_ = width;
  screen_height_ = height;
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
  
  valParser_ = SlValueParser(screen_width_, screen_height_);
  tmngr_->valParser = &valParser_;
  smngr_->valParser = &valParser_;
}



bool
SlManager::insertInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd, unsigned int destAfterThis)
{
  bool isInserted = false;
  SlRenderItem* toInsert = createRenderItem( toAdd, destToAdd );
  if ( toInsert == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " << toAdd  << std::endl;
#endif
    return isInserted;
  }

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  afterThis ) && ( (*iter)->destination_ == destAfterThis ) ) {
      renderQueue_.insert( (++iter), toInsert );
      isInserted = true;
      break;
    }
  }

  if ( !isInserted ) {
#ifdef DEBUG
    std::cout << "[SlManager::insertInRenderQueueAfter] Couldn't find RenderItem " << afterThis << " to insert after."  << std::endl;
#endif
  }
  return isInserted;
}



bool
SlManager::insertInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd, unsigned int destBeforeThis)
{
  bool isInserted = false;
  SlRenderItem* toInsert = createRenderItem( toAdd, destToAdd );
  if ( toInsert == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " << toAdd  << std::endl;
#endif
    return isInserted;
  }

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  beforeThis ) && ( (*iter)->destination_ == destBeforeThis ) ) {
      renderQueue_.insert( (iter), toInsert );
      isInserted = true;
      break;
    }
  }

  if ( !isInserted ) {
#ifdef DEBUG
    std::cout << "[SlManager::insertInRenderQueueBefore] Couldn't find RenderItem " << beforeThis << " to insert before."  << std::endl;
#endif
  }
  return isInserted;
}



void
SlManager::manipulateRenderQueue( const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters )
{
  auto iter = renderManip_.find(whatToDo);
  if ( iter == renderManip_.end() ) 
    throw std::invalid_argument("[SlManager::manipulateRenderQueue] Couldn't find object " + name );

    iter->second->manipulateQueue(name, destination, parameters);
}



bool
SlManager::moveInRenderQueue(const std::string& toMoveName, const std::string& targetName, unsigned int destToMove, unsigned int targetDest, int beforeOrAfter)
{
  bool isMoved = false;
  
  auto iter1 = std::find_if( renderQueue_.begin(), renderQueue_.end(),
			     [toMoveName, destToMove](const SlRenderItem* item) -> bool { return ( (item)->sprite_->name() == toMoveName && (item)->destination_ == destToMove ); } );
  if ( iter1 == renderQueue_.end() ) {
#ifdef DEBUG
    std::cout << "[SlManager::moveInRenderQueueAfter] Couldn't find item to move " << toMoveName   << std::endl;
#endif
    return isMoved;
  }

  auto iter2 = std::find_if( renderQueue_.begin(), renderQueue_.end(),
			     [targetName, targetDest](const SlRenderItem* item) -> bool { return ( (item)->sprite_->name() == targetName && (item)->destination_ == targetDest ); } );
  if ( iter2 == renderQueue_.end() ) {
#ifdef DEBUG
    std::cout << "[SlManager::moveInRenderQueueAfter] Couldn't find RenderItem to insert after: " << targetName << std::endl;
#endif
    return isMoved;
  }

  if ( iter1 == iter2 ) 
    return isMoved;

  /* Cheat sheet
    std::rotate( iter2, iter1, iter1+1 );   // insert before, iter1 > iter 2
    std::rotate( iter2+1, iter1, iter1+1 );  // insert after, iter1 > iter 2
    std::rotate( iter1, iter1+1, iter2 );   // insert before, iter1 < iter 2
    std::rotate( iter1, iter1+1, iter2+1 );  // insert after, iter1 < iter 2
  */

  if ( iter1 > iter2 )
    std::rotate( iter2 + beforeOrAfter, iter1, iter1+1 ); 
  else if ( iter1 < iter2 ) {
    std::rotate( iter1, iter1+1, iter2 + beforeOrAfter );
  }
  else
    return isMoved;

  isMoved = true;
  return isMoved;
}



bool
SlManager::moveInRenderQueueAfter(const std::string& toMoveName, const std::string& afterThis, unsigned int destToMove, unsigned int destAfterThis)
{
  bool isMoved = moveInRenderQueue( toMoveName, afterThis, destToMove, destAfterThis, 1);
  return isMoved;
}



bool
SlManager::moveInRenderQueueBefore(const std::string& toMoveName, const std::string& beforeThis, unsigned int destToMove, unsigned int destBeforeThis)
{
  bool isMoved = moveInRenderQueue( toMoveName, beforeThis, destToMove, destBeforeThis, 0);
  return isMoved;
}



bool
SlManager::parseConfigurationFile(const std::string& filename)
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
      if ( token[0] == '#' || token.empty() || token[0] == '\n' ) {
	/* empty line or comment */
      }
      else if ( token == "texture" ) {
	SlTexture* newTexture = tmngr_->parseTexture( input );
	if ( newTexture ) smngr_->createSprite(newTexture);
      }
      else if ( token == "sprite" ) {
	smngr_->parseSprite(input);
      }
      else if ( token == "manipulate" ) {
	smngr_->parseSpriteManipulation(input);
      }
      else if ( token == "font" ) {
	tmngr_->parseFont( input );
      }
      else if ( token == "renderqueue" ) {
	parseRenderQueueManipulation( input );
      }
      else {
#ifdef DEBUG
	std::cerr << "[SlManager::parseConfigurationFile] Unknown token " << token << std::endl;
#endif
      }
	
      token.clear();
      if ( input) getline(input,line);
    }

  return result;
}


void
SlManager::parseRenderQueueManipulation( std::ifstream& input )
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
	manipulateRenderQueue( name, destination, whatToDo, parameters );
      }
      catch (const std::exception& expt) {
	std::cerr << "[SlManager::parseRenderQueueManipulation] " << expt.what() << std::endl;
      }
      catch (...) {
	std::cerr << "[SlManager::parseRenderQueueManipulation] Unknown exception at line: " << line << std::endl;
      }
    }
    token.clear();
    if ( !endOfConfig ) getline(input,line);
  }
}



void
SlManager::render()
{
  SDL_RenderClear( renderer_ );
 
  for (auto& item: renderQueue_){
    if ( item->renderMe_ ) {
      try {
        (item->sprite_)->render( renderer_, (item->destination_) );
      }
      catch (const std::runtime_error& expt){
	std::cerr << expt.what() << std::endl;
      }
    }
  }

  SDL_RenderPresent( renderer_ );
}



bool
SlManager::setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int destination)
{
  return smngr_->setSpriteColor(name, red, green, blue, alpha, destination);
}




bool
SlManager::setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination)
{
  return smngr_->setSpriteDestinationOrigin(name, x, y, destination);
}



bool
SlManager::setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination)
{
  return smngr_->setSpriteRenderOptions(name, renderOptions, destination);
}



bool
SlManager::swapInRenderQueue(const std::string& toAdd, const std::string& toRemove, unsigned int destToAdd, unsigned int destToRemove)
{
  bool isSwapped = false;
  SlRenderItem* item = createRenderItem( toAdd, destToAdd );
  if ( item == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " << toAdd  << std::endl;
#endif
    return isSwapped;
  }

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  toRemove ) && ( (*iter)->destination_ == destToRemove ) ) {
      delete (*iter);
      *iter = item;
      isSwapped = true;
      break;
    }
  }

  if ( !isSwapped ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Couldn't find RenderItem " << toRemove << " to swap for"  << std::endl;
#endif
  }
  return isSwapped;
}



bool
SlManager::swapInRenderQueueAtPosition(const std::string& toAdd, unsigned int destToAdd, unsigned int position )
{
  bool isSwapped = false;
  if ( position >= renderQueue_.size() ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Error: Position " << position << " is out of bounds. Render queue size is " << renderQueue_.size()  << std::endl;
#endif
    return isSwapped;
  }
  
  SlRenderItem* item = createRenderItem( toAdd, destToAdd );
  if ( item == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " << toAdd  << std::endl;
#endif
    return isSwapped;
  }

  std::vector<SlRenderItem*>::iterator iter = renderQueue_.begin() + position;
  delete (*iter);
  renderQueue_.erase( iter );
  renderQueue_.insert( iter, item );
  isSwapped = true;

  if ( !isSwapped ) {
#ifdef DEBUG
    std::cout << "[SlManager::swapInRenderQueueAtPosition] Couldn't swap in sprite " << toAdd << " at position " << position << "."  << std::endl;
#endif
  }
  return isSwapped;
}



bool
SlManager::swapInRenderQueueLastPosition(const std::string& toAdd, unsigned int destToAdd )
{
  bool isSwapped = false;
  unsigned int position = renderQueue_.size() - 1 ;
  isSwapped = swapInRenderQueueAtPosition( toAdd, destToAdd, position );
  return isSwapped;
}



bool
SlManager::toggleRender(const std::string& toToggle, unsigned int destination, int onOrOff )
{
  bool isToggled = false;
  if ( (onOrOff < -1) || (onOrOff > 1) ){
#ifdef DEBUG
    std::cout << "[SlManager::toggleRender] Unknown onOrOff selection " << onOrOff << std::endl;
#endif
    return isToggled;
  }    
  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); ( ( iter != renderQueue_.end() ) && ( !isToggled ) ) ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  toToggle ) && ( (*iter)->destination_ == destination ) ) {
      switch (onOrOff){
      case -1:
	(*iter)->renderMe_ = !((*iter)->renderMe_) ;
	break;
      case 0:	
	(*iter)->renderMe_ = false ;
	break;
      case 1:
	(*iter)->renderMe_ = true ;
	break;
      }
      isToggled = true;
    }
  }

  if ( !isToggled ) {
#ifdef DEBUG
    std::cout << "[SlManager::toggleRender] Couldn't toggle sprite " << toToggle << " - sprite not in render queue."  << std::endl;
#endif
  }
  return isToggled;
}



bool
SlManager::toggleRenderOff(const std::string& toToggle, unsigned int destination )
{
  return toggleRender(toToggle, destination, 0);
}



bool
SlManager::toggleRenderOn(const std::string& toToggle, unsigned int destination )
{
  return toggleRender(toToggle, destination, 1);
}
