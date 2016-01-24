// part of SDL2lazy
// test program
// author: Ulrike Hager

#include <iostream>
#include <SDL2/SDL.h>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlManager.h"

// const int SCREEN_WIDTH = 1000;
// const int SCREEN_HEIGHT = 800;


int main()
{
  SlManager *mngr = new SlManager() ;  

  // SlManager *mngr = new SlManager("lazy-test", SCREEN_WIDTH, SCREEN_HEIGHT) ;  
  // mngr->parseIniFile();

  mngr->appendToRenderQueue("tex2"); // appending just to check deleting.
  mngr->deleteTexture("tex1");
  mngr->deleteTexture("tex2");

  bool quit = false;
  SDL_Event event;
    
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      mngr->handleEvent(event);
      if (event.type == SDL_QUIT) quit = true;
      else if (event.type == SDL_KEYDOWN)
	{
	  switch(event.key.keysym.sym)
	    {
	    // case SDLK_UP:
	    //   mngr->swapInRenderQueueLastPosition( "up", 0 );
	    //   mngr->setSpriteRenderOptions( "background", SL_RENDER_DEFAULT );
	    //   break;
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
  
