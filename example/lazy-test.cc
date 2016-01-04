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
  SlManager *mngr = SlManager::Instance();
  mngr = new SlManager("lazy-test", SCREEN_WIDTH, SCREEN_HEIGHT) ;  

  // SlTexture* wrongFileTest = mngr->createTextureFromFile("tile","resources/tacky_backgroun.png");
  // if (wrongFileTest == nullptr) {
  //   std::cout << "wrong file! " << std::endl;
  // }

   mngr->parseConfigurationFile();
   
  mngr->setSpriteDestinationOrigin("tex2", 12, 12);
  mngr->createTextureFromSpriteOnTexture("minimap", "tex1", "tex2");
  mngr->setSpriteDestinationOrigin("minimap", 20, 20 );
  mngr->setSpriteColor("minimap", 0x40, 0xAA, 0xBB, 0xA0);
  mngr->setSpriteRenderOptions("minimap", SL_RENDER_ALPHAMOD | SL_RENDER_COLORMOD);

  mngr->deleteTexture("tex1");
  mngr->deleteTexture("tex2");

  mngr->centerSpriteInSprite("up", "minimap");
  mngr->centerSpriteInSprite("down", "minimap");
  mngr->centerSpriteInSprite("left", "minimap");
  mngr->centerSpriteInSprite("right", "minimap");
  
  mngr->appendToRenderQueue("background", 0);    
  mngr->appendToRenderQueue("minimap", 0);
  mngr->appendToRenderQueue("up");

  mngr->swapInRenderQueue( "down", "up", 0, 0 );

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
	      break;
	    case SDLK_DOWN:
	      mngr->swapInRenderQueueLastPosition( "down", 0 );
	      break;
	    case SDLK_LEFT:
	      mngr->swapInRenderQueueLastPosition( "left" );
	      break;
	    case SDLK_RIGHT:
	      mngr->swapInRenderQueueLastPosition( "right", 0 );
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

     mngr->render();
 
  }
  
  delete mngr;
  
  return 0;
}
  
