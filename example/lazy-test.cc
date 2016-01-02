// part of SDL2lazy
// test program
// author: Ulrike Hager

#include <iostream>
#include <SDL2/SDL.h>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;


int main()
{
  SlManager* gMngr = new SlManager("lazy-test", SCREEN_WIDTH, SCREEN_HEIGHT) ;  
  
  SlTexture* backgroundTile = new SlTexture("tile");
  backgroundTile->loadFromFile(gMngr->renderer(),"resources/tacky_background.png");
  SlSprite* tile = new SlSprite("tile",backgroundTile);
  SlTexture* bgTexture = new SlTexture("background");
  bgTexture->createFromTiles(gMngr->renderer(), tile, SCREEN_WIDTH, SCREEN_HEIGHT);
  SlSprite* background = new SlSprite("background", bgTexture);
  delete backgroundTile;
  delete tile;
  
  SlTexture* mmTexture1 = new SlTexture("tex1");
  mmTexture1->createFromRectangle(gMngr->renderer(), 210, 210, 0x00, 0x00, 0xC0, 0xFF);
  SlTexture* tempTex = new SlTexture("temp");
  tempTex->createFromRectangle(gMngr->renderer(), 186, 186, 0x50, 0x00, 0xE0, 0xFF);
  SlSprite* tempSprite = new SlSprite("temp",tempTex);
  tempSprite->setDestinationOrigin(12, 12);

  SlTexture* miniMapTexture = new SlTexture("minimap");
  miniMapTexture->createFromSpriteOnTexture(gMngr->renderer(), mmTexture1, tempSprite);
  delete tempSprite;
  delete tempTex;
  delete mmTexture1;
  
  SlSprite* miniMapBg = new SlSprite("miniMapBg", miniMapTexture);
  miniMapBg->setDestinationOrigin( 20, 20 );
  miniMapBg->setColor(0x40, 0xAA, 0xBB, 0x50);
  miniMapBg->setRenderOptions(SL_RENDER_ALPHAMOD | SL_RENDER_COLORMOD);

  SlTexture* arrowTexture = new SlTexture("arrowTexture");
  arrowTexture->loadFromFile(gMngr->renderer(),"resources/arrowsheet_transp.png");
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

    SDL_RenderClear(gMngr->renderer());
    background->render(gMngr->renderer());
    miniMapBg->render(gMngr->renderer());
    upArrow->render(gMngr->renderer());
    SDL_RenderPresent( gMngr->renderer() );
  }
  

  delete bgTexture;
  delete background;
  delete miniMapTexture;
  delete miniMapBg;
  delete upArrow;
  delete arrowTexture;  
  delete gMngr;
  
  return 0;
}
  