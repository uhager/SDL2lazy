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
SlManager::clear()
{
  std::vector<SlRenderItem*>::iterator item;
  for ( item = renderQueue_.begin(); item != renderQueue_.end() ; ++item) {
    delete (*item);
  }
  renderQueue_.clear();

  std::vector<SlSprite*>::iterator sprite;
  for ( sprite = sprites_.begin(); sprite != sprites_.end(); ++sprite) {
    std::cout << "[SlManager::clear] next "<< std::endl;
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
SlManager::determineDimensions(std::vector<std::string> dimensions, int& width, int& height)
{
  bool validDimensions = false;
  
  if ( dimensions.size() != 2 ) {
#ifdef DEBUG
    std::cerr << "[SlManager::determineDimensions] Need 2 dimensions, found " << dimensions.size() << std::endl;
#endif
    return validDimensions;
  }
  
  if ( dimensions.at(0) == "SCREEN_WIDTH" )
    width = screen_width_;
  else {
    try {
      width = std::stoi( dimensions.at(0) );
    }
    catch (std::invalid_argument) {
      return validDimensions;
    }
  }
  
  if ( dimensions.at(1) == "SCREEN_HEIGHT" )
    height = screen_height_;
  else {
    try {
      height = std::stoi( dimensions.at(1) );
    }
    catch (std::invalid_argument) {
      return validDimensions;
    }
  }
  validDimensions = true;
  return validDimensions;
}



bool
SlManager::determineColors(std::vector<std::string> colors, uint8_t (&colArray)[4] ) 
{
  bool validColors = false;
  
  if ( colors.size() != 4 ) {
#ifdef DEBUG
    std::cerr << "[SlManager::determineColors] Need 4 colors, found " << colors.size() << std::endl;
#endif
    return validColors;
  }

  for ( unsigned int i = 0 ; i != colors.size() ; ++i ){
    try {
      colArray[i] = std::stoi( colors.at(i) );
    }
    catch (std::invalid_argument) {
      return validColors;
    }
  }

  validColors = true;
  return validColors;
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
  std::string name;
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
    else {
#ifdef DEBUG
      std::cerr << "[SlManager::parseSprites] Unknown token " << token << std::endl;
#endif
    }
    
    token.clear();
    if ( !endOfConfig ) getline(input,line);
  }
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
    int width, height;
    bool check = determineDimensions( dimensions, width, height );
    if ( !check ) {
#ifdef DEBUG
      std::cerr << "[SlManager::parseTexture] invalid dimensions for type " << type << std::endl;
#endif
      return;
    }

    if ( type == "tile") {
      createTextureFromTile( name, sprite, width, height );
    }

    else if ( type == "rectangle" ) {
      uint8_t colArray[4] = {0,0,0,0};
      bool check = determineColors( colors, colArray );
      if ( !check ) {
#ifdef DEBUG
	std::cerr << "[SlManager::parseTexture] invalid color for " << name  << std::endl;
#endif
	return;
      }
      createTextureFromRectangle( name, width, height, colArray[0], colArray[1], colArray[2], colArray[3] );
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
