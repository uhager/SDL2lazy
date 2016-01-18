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

  // SlTexture* wrongFileTest = mngr->createTextureFromFile("tile","resources/tacky_backgroun.png");
  // if (wrongFileTest == nullptr) {
  //   std::cout << "wrong file! " << std::endl;
  // }

  mngr->parseConfigurationFile();
  mngr->setSpriteColor("background", 0x40, 0xAA, 0xBB, 0xFF);

   
  //  mngr->setSpriteDestinationOrigin("tex2", 12, 12);
  //mngr->createTextureFromSpriteOnTexture("minimap", "tex1", "tex2");
  //  mngr->setSpriteDestinationOrigin("minimap", 20, 20 );
  //  mngr->setSpriteRenderOptions("minimap");

  mngr->appendToRenderQueue("tex2"); // appending just to check deleting.
  mngr->deleteTexture("tex1");
  mngr->deleteTexture("tex2");

  mngr->setSpriteColor("minimap", 0x40, 0xAA, 0xBB, 0xA0);

  /*
  mngr->setSpriteDestinationOrigin("upperright", SCREEN_WIDTH - 120,                 0  );
  mngr->setSpriteDestinationOrigin("lowerright", SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120);
  mngr->setSpriteDestinationOrigin("lowerleft" ,                0  , SCREEN_HEIGHT - 120); 
  mngr->appendToRenderQueue("background", 0);    
  mngr->appendToRenderQueue("upperright");
  mngr->appendToRenderQueue("lowerright");
  mngr->appendToRenderQueue("minimap", 0);
  mngr->appendToRenderQueue("message1");
  mngr->appendToRenderQueue("message2");
  mngr->appendToRenderQueue("up");
  */

  mngr->insertInRenderQueueAfter("upperleft", "background");
  mngr->insertInRenderQueueBefore("lowerleft", "minimap");

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
  
