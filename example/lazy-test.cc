// part of SDL2lazy
// test program
// author: Ulrike Hager

#include <iostream>
#include <SDL2/SDL.h>

#include "SlTexture.h"
#include "SlSprite.h"


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

bool init();
void close();


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;


void
close()
{
  SDL_DestroyWindow( gWindow );
  gWindow = nullptr;
  SDL_Quit();
}

bool
init()
{
  bool result = false;
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    std::cout << "SDL could not initialize! SDL_Error: " <<  SDL_GetError() << std::endl;
    return result;
  }
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    std::cout << "IMG_Init " << SDL_GetError();
    SDL_Quit();
    return 1;
  }

  //Create window
  gWindow = SDL_CreateWindow( "wrapper test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  if( gWindow == nullptr ){
    std::cout << "Window could not be created! SDL_Error: " <<  SDL_GetError()  << std::endl;
    return result;
  }
  gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
  if( gRenderer == nullptr ){
    std::cout << "Renderer could not be created " <<  SDL_GetError() << std::endl;
    return result;
  }
  result = true;
  return result;
}
  

int main()
{
  if (!init()){
    std::cout << "BOOOoooo!" << std::endl;
    return -1;
  }

  SlTexture* backgroundTile = new SlTexture("tile");
  backgroundTile->loadFromFile(gRenderer,"resources/tacky_background.png");
  SlSprite* tile = new SlSprite("tile",backgroundTile);
  SlTexture* bgTexture = new SlTexture("background");
  bgTexture->createFromTiles(gRenderer, tile, SCREEN_WIDTH, SCREEN_HEIGHT);
  SlSprite* background = new SlSprite("background", bgTexture);
  delete backgroundTile;
  delete tile;
  
  SlTexture* mmTexture1 = new SlTexture("tex1");
  mmTexture1->createFromRectangle(gRenderer, 210, 210, 0x00, 0x00, 0xC0, 0xFF);
  SlTexture* tempTex = new SlTexture("temp");
  tempTex->createFromRectangle(gRenderer, 186, 186, 0x50, 0x00, 0xE0, 0xFF);
  SlSprite* tempSprite = new SlSprite("temp",tempTex);
  tempSprite->setDestinationOrigin(12, 12);

  SlTexture* miniMapTexture = new SlTexture("minimap");
  miniMapTexture->createFromSpriteOnTexture(gRenderer, mmTexture1, tempSprite);
  delete tempSprite;
  delete tempTex;
  delete mmTexture1;
  
  SlSprite* miniMapBg = new SlSprite("miniMapBg", miniMapTexture);
  miniMapBg->setDestinationOrigin( 20, 20 );
  miniMapBg->setColor(0x40, 0xAA, 0xBB, 0x50);
  miniMapBg->setRenderOptions(SL_RENDER_ALPHAMOD | SL_RENDER_COLORMOD);

  SlTexture* arrowTexture = new SlTexture("arrowTexture");
  arrowTexture->loadFromFile(gRenderer,"resources/arrowsheet_transp.png");
  SlSprite* upArrow = new SlSprite("up", arrowTexture, 0, 160, 80, 140);
  upArrow->setDestinationOrigin(25,25);
  
  bool quit = false;
  SDL_Event event;
    
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) quit = true;
      else if (event.type == SDL_KEYDOWN)
	{
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_UP:
	      break;
	    case SDLK_DOWN:
	      break;
	    case SDLK_LEFT:
	      break;
	    case SDLK_RIGHT:
	      break;
	    case SDLK_1:
	      break;
	    case SDLK_2:
	      break;
	    case SDLK_3:
	      break;
	    case SDLK_4:
	      break;
	    case SDLK_ESCAPE:
	      quit = true;
	      break;
	    }
	}
    }

    SDL_RenderClear(gRenderer);
    background->render(gRenderer);
    miniMapBg->render(gRenderer);
    upArrow->render(gRenderer);
    SDL_RenderPresent( gRenderer );
  }
  

  delete bgTexture;
  delete background;
  delete miniMapTexture;
  delete miniMapBg;
  close();
  
  return 0;
}
  
