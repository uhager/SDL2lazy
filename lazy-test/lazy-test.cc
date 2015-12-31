// part of SDL2lazy
// test program
// author: Ulrike Hager

#include <iostream>
#include <SDL2/SDL.h>

#include "SlTexture.h"
#include "SlSpritesheet.h"


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

  SlTexture* backgroundTexture = new SlTexture("background");
  backgroundTexture->loadFromFile(gRenderer,"resources/tacky_background.png");
  backgroundTexture->setColor(0xBB, 0x40, 0x80, 0x50);
  backgroundTexture->tileTexture(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  
  SlSpritesheet* arrowSheet = new SlSpritesheet("arrowsheet");
  arrowSheet->loadFromFile(gRenderer,"resources/arrowsheet_transp.png");
  arrowSheet->readCoordSheet("resources/arrowsheet.layout");
  arrowSheet->setCurrentSprite("up");
  
  SlTexture* miniMapBgTexture = new SlTexture("miniMapBg");
  miniMapBgTexture->setColor(0x00,0x00,0xC0,0x40);
  miniMapBgTexture->createFromRectangle(gRenderer,210,210);

  SlTexture* temp = new SlTexture("temp");
  temp->setColor(0x50,0x00,0xE0,0xFF);
  temp->createFromRectangle(gRenderer,186,186);
  temp->setDestinationOrigin(12, 12);
  bool check = miniMapBgTexture->renderTextureOnTexture(gRenderer, temp, SL_RENDER_USE_DESTINATION);
  if (!check){
    std::cout << "renderTextureOnTexture failed " << SDL_GetError() << std::endl;
    delete backgroundTexture;
    delete arrowSheet;
    delete miniMapBgTexture;
    delete temp;
    close();
    return -1;
  }
  delete temp;

  SlSpritesheet* corners = new SlSpritesheet("corners");
  corners->loadFromFile(gRenderer, "resources/corners.png");
  corners->readCoordSheet("resources/cornersheet.layout");
  corners->setDestinationOrigin("upperright", SCREEN_WIDTH - 120,                 0  ); 
  corners->setDestinationOrigin("lowerright", SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120); 
  corners->setDestinationOrigin("lowerleft" ,                0  , SCREEN_HEIGHT - 120); 
    
  SDL_Rect miniMapRect = {10,10,220,220};
  miniMapBgTexture->setDestination(miniMapRect);
  miniMapBgTexture->setRenderOptions( SL_RENDER_USE_DESTINATION );

  arrowSheet->centerAllSpritesIn(miniMapRect);

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
	      arrowSheet->setCurrentSprite("up");
	      backgroundTexture->setRenderOptions( SL_RENDER_DEFAULT );
	      break;
	    case SDLK_DOWN:
	      arrowSheet->setCurrentSprite("down");
	      backgroundTexture->setRenderOptions( SL_RENDER_COLORMOD );
	      break;
	    case SDLK_LEFT:
	      arrowSheet->setCurrentSprite("left");
	      miniMapBgTexture->setRenderOptions( SL_RENDER_USE_DESTINATION | SL_RENDER_ALPHAMOD );
	      break;
	    case SDLK_RIGHT:
	      arrowSheet->setCurrentSprite("right");
	      miniMapBgTexture->setRenderOptions( SL_RENDER_USE_DESTINATION );
	      break;
	    case SDLK_1:
	      corners->toggleSprite("upperleft");
	      break;
	    case SDLK_2:
	      corners->toggleSprite("upperright");
	      break;
	    case SDLK_3:
	      corners->toggleSprite("lowerright");
	      break;
	    case SDLK_4:
	      corners->toggleSprite("lowerleft");
	      break;
	    case SDLK_ESCAPE:
	      quit = true;
	      break;
	    }
	}
    }

    SDL_RenderClear(gRenderer);
    backgroundTexture->render(gRenderer);
    corners->render(gRenderer);
    miniMapBgTexture->render(gRenderer);
    arrowSheet->renderCurrentSprite(gRenderer);
    SDL_RenderPresent( gRenderer );
  }
  

  delete backgroundTexture;
  delete arrowSheet;
  delete miniMapBgTexture;
  delete corners;
  close();
  
  return 0;
}
  
