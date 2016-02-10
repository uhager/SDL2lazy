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
#include <iterator>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlRenderItem.h"
#include "SlTextureManager.h"
#include "SlSpriteManager.h"
#include "SlManipulation.h"
#include "SlRenderQueueManipulation.h"

#include "SlManager.h"



SlManager::SlManager()
{
  this->initialize();
  parseIniFile();
}



SlManager::SlManager(const std::string& name, int width, int height)
{
  this->initialize();
  this->initializeWindow(name, width, height);

}



SlManager::~SlManager(void)
{
  this->clear();
  smngr_ = nullptr;
  tmngr_ = nullptr;
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
  std::map<std::string, SlManipulation*>::iterator mapItem;
  for (mapItem = renderManip_.begin(); mapItem != renderManip_.end() ; ++mapItem ) {
    delete ( mapItem->second );
  }
  renderManip_.clear();

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
  try {
    std::shared_ptr<SlSprite> sprite = smngr_->findSprite(name);
    if (sprite->size() <= destination){
#ifdef DEBUG
      std::cout << "[SlManager::createRenderItem] Failed to add sprite " << name << ": destination " << destination << " out of bounds (" << sprite->size() << ")" <<  std::endl;
#endif
      return item;
    }
    item = new SlRenderItem(sprite, destination);
  }
  catch (const std::exception& expt) {
    std::cerr << expt.what() << std::endl;
  }
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
  smngr_->deleteSprites(name);
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
SlManager::handleEvent(const SDL_Event& event)
{
  eventHandler_->handleEvent(event);
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
  
  valParser_ = SlValueParser();
  tmngr_ = std::unique_ptr<SlTextureManager>(new SlTextureManager( this ));
  //  smngr_ = std::make_unique<SlSpriteManager>( this );
  smngr_ = std::shared_ptr<SlSpriteManager>(new SlSpriteManager( this )); //!< Needs to be shared with SlRenderQueueManipulation items.
  eventHandler_ = std::unique_ptr<SlEventHandler>( new SlEventHandler() );
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
  
  valParser_.setDimensions(screen_width_, screen_height_);
  // tmngr_ = std::unique_ptr<SlTextureManager>(new SlTextureManager( this ));
  // smngr_ = std::shared_ptr<SlSpriteManager>(new SlSpriteManager( this )); //!< Needs to be shared with SlRenderQueueManipulation items.

  tmngr_->valParser = &valParser_;
  smngr_->initialize( &valParser_ );

  SlManipulation* toAdd;
  toAdd = new SlRMappend( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMinsertAfter( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMinsertBefore( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMswapIn( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMswapAt( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMtoggleOnOff( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMmoveAfter( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMmoveBefore( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMactivate( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMactivateIfInside( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMdeactivate( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMdeactivateIfInside( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMmoveBy( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;
  toAdd = new SlRMmoveActiveBy( smngr_.get(), &valParser_, &renderQueue_ );
  renderManip_[toAdd->name()] = toAdd;

  eventHandler_->addManipulations( renderManip_ );
  eventHandler_->addManipulations( smngr_->manipulations() );
}



void
SlManager::insertInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd, unsigned int destAfterThis)
{
  SlRenderItem* toInsert = createRenderItem( toAdd, destToAdd );
  if ( toInsert == nullptr ) 
    throw std::runtime_error( "[SlManager::insertInRenderQueueAfter] Couldn't create SlRenderItem for " + toAdd ) ;

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  afterThis ) && ( (*iter)->destination_ == destAfterThis ) ) {
      renderQueue_.insert( (++iter), toInsert );
      return;
    }
  }

  if ( iter == renderQueue_.end() ) 
    throw std::runtime_error( "[SlManager::insertInRenderQueueAfter] Couldn't find RenderItem " + afterThis + " to insert after." );
}



void
SlManager::insertInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd, unsigned int destBeforeThis)
{

  SlRenderItem* toInsert = createRenderItem( toAdd, destToAdd );
  if ( toInsert == nullptr ) 
    throw std::runtime_error( "[SlManager::insertInRenderQueueBefore] Couldn't create SlRenderItem for " + toAdd );

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  beforeThis ) && ( (*iter)->destination_ == destBeforeThis ) ) {
      renderQueue_.insert( (iter), toInsert );
      return;
    }
  }

  if ( iter == renderQueue_.end() )
    throw std::runtime_error( "[SlManager::insertInRenderQueueBefore] Couldn't find RenderItem " + beforeThis + " to insert before." );
}



void
SlManager::manipulateRenderQueue( const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters )
{
  auto iter = renderManip_.find(whatToDo);
  if ( iter == renderManip_.end() ) 
    throw std::invalid_argument("[SlManager::manipulateRenderQueue] Couldn't find object " + whatToDo );

    iter->second->manipulate(name, destination, parameters);
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
  if ( !input.is_open() ) 
    throw std::runtime_error("[SlManager::parseConfigurationFile] Couldn't open file " + filename );
  
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
      else if ( token == "event" ) {
	eventHandler_->parseEvent( input );
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
SlManager::parseIniFile(const std::string& filename)
{
  std::ifstream input(filename,std::ifstream::in);
  if ( !input.is_open() ) 
    throw std::runtime_error("[SlManager::parseIniFile] Couldn't open ini file " + filename );
  
  std::string line, token, name;
  getline(input,line);
  while ( input )
    {
      std::istringstream stream(line.c_str());
      stream >> token;
      if ( token[0] == '#' || token.empty() || token[0] == '\n' ) {
	/* empty line or comment */
      }
      else if ( token == "window" ) {
	std::string name;
	stream >> name;
	std::istream_iterator<unsigned int> uint_iter(stream), eof;
	std::vector<unsigned int> dims(uint_iter, eof);
	initializeWindow( name, dims.at(0), dims.at(1) );
      }
      else if ( token == "file" ) {
	std::string filename;
	stream >> filename;
	parseConfigurationFile(filename);
      }
      else {
#ifdef DEBUG
	std::cerr << "[SlManager::parseConfigurationFile] Unknown token " << token << std::endl;
#endif
      }
	
      token.clear();
      if ( input) getline(input,line);
    }
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
	std::istream_iterator<std::string> str_iter(stream), eof;
	std::vector<std::string> parameters(str_iter, eof);
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



void
SlManager::run()
{
  int quit = 0;
  while ( !quit ) {
    quit = eventHandler_->pollEvent();
    render();
  }
}


void
SlManager::setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int destination)
{
  smngr_->setSpriteColor(name, red, green, blue, alpha, destination);
}




void
SlManager::setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination)
{
  smngr_->setSpriteDestinationOrigin(name, x, y, destination);
}



void
SlManager::setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination)
{
   smngr_->setSpriteRenderOptions(name, renderOptions, destination);
}



void
SlManager::swapInRenderQueue(const std::string& toAdd, const std::string& toRemove, unsigned int destToAdd, unsigned int destToRemove)
{
  SlRenderItem* item = createRenderItem( toAdd, destToAdd );
  if ( item == nullptr )
    throw std::runtime_error( "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " + toAdd );

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  toRemove ) && ( (*iter)->destination_ == destToRemove ) ) {
      delete (*iter);
      *iter = item;
      return;
    }
  }

  if ( iter == renderQueue_.end() )
    throw std::runtime_error( "[SlManager::swapInRenderQueue] Couldn't find RenderItem " + toRemove + " to swap for." );
}



void
SlManager::swapInRenderQueueAtPosition(const std::string& toAdd, unsigned int destToAdd, unsigned int position )
{
  if ( position >= renderQueue_.size() ) 
    throw std::invalid_argument( "[SlManager::swapInRenderQueue] Error: Position " + std::to_string(position) + " is out of bounds. Render queue size is " + std::to_string(renderQueue_.size()) );
  
  SlRenderItem* item = createRenderItem( toAdd, destToAdd );
  if ( item == nullptr )
    throw std::runtime_error( "[SlManager::swapInRenderQueue] Couldn't create SlRenderItem for " + toAdd );

  std::vector<SlRenderItem*>::iterator iter = renderQueue_.begin() + position;
  delete (*iter);
  renderQueue_.erase( iter );
  renderQueue_.insert( iter, item );
}



void
SlManager::swapInRenderQueueLastPosition(const std::string& toAdd, unsigned int destToAdd )
{
  unsigned int position = renderQueue_.size() - 1 ;
  swapInRenderQueueAtPosition( toAdd, destToAdd, position );
}



void
SlManager::toggleRender(const std::string& toToggle, unsigned int destination, int onOrOff )
{
  if ( (onOrOff < -1) || (onOrOff > 1) )
    throw std::invalid_argument( "[SlManager::toggleRender] Unknown onOrOff selection " + std::to_string( onOrOff ) );

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin();  iter != renderQueue_.end() ; ++iter) {
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
      return;
    }
  }

  if ( iter == renderQueue_.end() ) 
    throw std::runtime_error( "[SlManager::toggleRender] Couldn't toggle sprite " + toToggle + " - sprite not in render queue." );
}



void
SlManager::toggleRenderOff(const std::string& toToggle, unsigned int destination )
{
  toggleRender(toToggle, destination, 0);
}



void
SlManager::toggleRenderOn(const std::string& toToggle, unsigned int destination )
{
  toggleRender(toToggle, destination, 1);
}
