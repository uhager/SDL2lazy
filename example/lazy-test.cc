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

  mngr->run();
   
  delete mngr;
  
  return 0;
}
  
