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

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"





SlManager::SlManager()
{
  this->initialize();
}



SlManager::SlManager(std::string name, int width, int height)
{
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
  if ( toAdd == nullptr ) return;
  textures_.push_back(toAdd);
  createSprite(toAdd->name_, toAdd->name_);
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
SlManager::centerSpriteInSprite(std::string toCenter, std::string target, unsigned int destinationThis, unsigned int destinationOther)
{
  SlSprite* sprite = findSprite(toCenter);
  if ( sprite == nullptr ) {
#ifdef DEBUG
    std::cerr << "[SlManager::centerSpriteInSprite] Couldn't find sprite " << toCenter << std::endl;
#endif
    return;
  }

  SlSprite* other = findSprite(target);
  if ( other == nullptr ) {
#ifdef DEBUG
    std::cerr << "[SlManager::centerSpriteInSprite] Couldn't find sprite " << other << std::endl;
#endif
    return;
  }

  sprite->centerInSprite( other, destinationThis, destinationOther );
}
  

void
SlManager::clear()
{
  std::vector<SlRenderItem*>::iterator item;
  for ( item = renderQueue_.begin(); item != renderQueue_.end() ; ++item) {
    delete (*item);
  }
  renderQueue_.clear();

  std::vector<SlSprite*>::iterator sprite;
  for ( sprite = sprites_.begin(); sprite != sprites_.end(); ++sprite) {
    delete (*sprite);
    (*sprite) = nullptr ;
  }
  sprites_.clear();  

  std::vector<SlTexture*>::iterator iter;
  for ( iter=textures_.begin(); iter != textures_.end(); ++iter){
    delete (*iter);
  }
  textures_.clear();
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



bool
SlManager::determineValues(std::vector<std::string> stringValues, int *values, unsigned int valueSize ) 
{
  bool validValues = false;
  
  if ( stringValues.size() != valueSize ) {
#ifdef DEBUG
    std::cerr << "[SlManager::determineColors] Need 4 colors, found " << stringValues.size() << std::endl;
#endif
    return validValues;
  }

  for ( unsigned int i = 0 ; i != stringValues.size() ; ++i , ++values){
    if ( stringValues.at(i) == "SCREEN_WIDTH" ) {
      (*values) = screen_width_;
    }
    else if ( stringValues.at(i) == "SCREEN_HEIGHT" ) {
      (*values) = screen_height_;
    }
    else {
      try {
	(*values) = std::stoi( stringValues.at(i) );
      }
      catch (std::invalid_argument) {
	return validValues;
      }
    }
  }

  validValues = true;
  return validValues;
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
}



bool
SlManager::insertInRenderQueueAfter(std::string toAdd, std::string afterThis, unsigned int destToAdd, unsigned int destAfterThis)
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
    if ( ( (*iter)->name_ ==  afterThis ) && ( (*iter)->destination_ == destAfterThis ) ) {
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
SlManager::insertInRenderQueueBefore(std::string toAdd, std::string beforeThis, unsigned int destToAdd, unsigned int destBeforeThis)
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
    if ( ( (*iter)->name_ ==  beforeThis ) && ( (*iter)->destination_ == destBeforeThis ) ) {
      renderQueue_.insert( (++iter), toInsert );
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
      if ( token[0] == '#' || token.empty() || token[0] == '\n' ) {
	/* empty line or comment */
      }
      else if ( token == "texture" ) {
	parseTexture( input );
      }
      else if ( token == "sprite" ) {
	parseSprite(input);
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
SlManager::parseSprite(std::ifstream& input)
{
  std::cout << "[SlManager::parseSprites]" << std::endl;
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
      std::cerr << "[SlManager::parseSprites] Unknown token " << token << std::endl;
#endif
    }
    
    token.clear();
    if ( !endOfConfig ) getline(input,line);
  }

  if ( name.empty() || texture.empty() ) {
#ifdef DEBUG
    std::cerr << "[SlManager::parseTexture] Name or texture missing" << std::endl;
#endif
    return;
  }
  int loc[4];
  bool check = determineValues(location, loc, 4);
  if ( !check ) {
#ifdef DEBUG
    std::cerr << "[SlManager::parseTexture] Invalid location for " << name << std::endl;
#endif
    return;
  }
  createSprite( name, texture, loc[0], loc[1], loc[2], loc[3] );
  
}




void
SlManager::parseTexture(std::ifstream& input)
{
  std::string line, token;
  bool endOfConfig = false;
  std::string name, type, file, sprite;
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
      std::cerr << "[SlManager::parseTexture] Unknown token " << token << std::endl;
#endif
    }
    token.clear();
    if (  !endOfConfig ) getline(input,line);
  }

  
  if ( name.empty() ) {
#ifdef DEBUG
    std::cerr << "[SlManager::parseTexture] No name found" << std::endl;
#endif
    return;
  }
  
  if ( type == "file" ) {
    createTextureFromFile( name, file );
  }
  
  else if ( type == "tile" || type == "rectangle" ) {
    int dim[2];
    bool check = determineValues( dimensions, dim, 2 );
    if ( !check ) {
#ifdef DEBUG
      std::cerr << "[SlManager::parseTexture] invalid dimensions for type " << type << std::endl;
#endif
      return;
    }

    if ( type == "tile") {
      createTextureFromTile( name, sprite, dim[0], dim[1] );
    }

    else if ( type == "rectangle" ) {
      int colArray[] = {0,0,0,0};
      bool check = determineValues( colors, colArray, 4 );
      if ( !check ) {
#ifdef DEBUG
	std::cerr << "[SlManager::parseTexture] invalid color for " << name  << std::endl;
#endif
	return;
      }
      createTextureFromRectangle( name, dim[0], dim[1], colArray[0], colArray[1], colArray[2], colArray[3] );
    }
  }
  else {
#ifdef DEBUG
    std::cerr << "[SlManager::parseTexture] Unknown type "  << type << " for texture " << name << std::endl;
#endif
  }
}



int
SlManager::render()
{
  int result;
  
  SDL_RenderClear( renderer_ );
 
  for (auto& item: renderQueue_){
    if ( item->renderMe_ ) {
      result = (item->sprite_)->render( renderer_, (item->destination_) );
      if (result != 0) {
#ifdef DEBUG
	std::cout << "[SlManager::render] Couldn't render sprite " << item->name_ << std::endl;
#endif
	return result;
      }
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



bool
SlManager::swapInRenderQueue(std::string toAdd, std::string toRemove, unsigned int destToAdd, unsigned int destToRemove)
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
    if ( ( (*iter)->name_ ==  toRemove ) && ( (*iter)->destination_ == destToRemove ) ) {
      delete (*iter);
      renderQueue_.erase( iter );
      renderQueue_.insert( iter, item );
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
SlManager::swapInRenderQueueAtPosition(std::string toAdd, unsigned int destToAdd, unsigned int position )
{
  bool isSwapped = false;
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
SlManager::swapInRenderQueueLastPosition(std::string toAdd, unsigned int destToAdd )
{
  bool isSwapped = false;
  unsigned int position = renderQueue_.size() - 1 ;
  isSwapped = swapInRenderQueueAtPosition( toAdd, destToAdd, position );
  return isSwapped;
}



bool
SlManager::toggleRender(std::string toToggle, unsigned int destination )
{
  bool isToggled = false;
  
  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_.begin(); iter != renderQueue_.end() ; ++iter) {
    if ( ( (*iter)->name_ ==  toToggle ) && ( (*iter)->destination_ == destination ) ) {
      (*iter)->renderMe_ = !((*iter)->renderMe_) ;
      isToggled = true;
      break;
    }
  }

  if ( !isToggled ) {
#ifdef DEBUG
    std::cout << "[SlManager::toggleRender] Couldn't toggle sprite " << toToggle << " - sprite not in render queue."  << std::endl;
#endif
  }
  return isToggled;
}
