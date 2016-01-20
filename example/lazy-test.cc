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
  SlManager *mngr = new SlManager("lazy-test", SCREEN_WIDTH, SCREEN_HEIGHT) ;  

  mngr->parseConfigurationFile();

  mngr->appendToRenderQueue("tex2"); // appending just to check deleting.
  mngr->deleteTexture("tex1");
  mngr->deleteTexture("tex2");

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
	      mngr->swapInRenderQueueLastPosition( "up", 0 );
	      mngr->setSpriteRenderOptions( "background", SL_RENDER_DEFAULT );
	      break;
	    case SDLK_DOWN:
	      mngr->swapInRenderQueueLastPosition( "down", 0 );
	      mngr->setSpriteRenderOptions( "background", SL_RENDER_COLORMOD );
	      break;
	    case SDLK_LEFT:
	      mngr->swapInRenderQueueAtPosition( "left", 0, 8 );
	      mngr->setSpriteRenderOptions( "minimap", SL_RENDER_DEFAULT );
	      break;
	    case SDLK_RIGHT:
	      mngr->swapInRenderQueueAtPosition( "right", 0, 8 );
	      mngr->setSpriteRenderOptions( "minimap", SL_RENDER_ALPHAMOD );
	      break;
	    case SDLK_1:
	      mngr->toggleRender("upperleft");
	      break;
	    case SDLK_2:
	      mngr->toggleRender("upperright");
	      break;
	    case SDLK_3:
	      mngr->toggleRender("lowerright");
	      break;
	    case SDLK_4:
	      mngr->toggleRender("lowerleft");
	      break;
	    case SDLK_m:
	      mngr->toggleRenderOn("minimap");
	      break;
	    case SDLK_n:
	      mngr->toggleRenderOff("minimap");
	      break;
	    case SDLK_b:
	      mngr->moveInRenderQueueBefore("upperleft", "minimap");
	      break;
	    case SDLK_f:
	      mngr->moveInRenderQueueAfter("upperleft", "minimap", 0, 0);
	      break;
	    case SDLK_ESCAPE:
	      quit = true;
	      break;
	    }
	}
    }

     mngr->render();
 
  }
  
  delete mngr;
  
  return 0;
}
  
