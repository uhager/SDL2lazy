// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManager.h
  \brief SlManager class, has a Window with SDL_Renderer, manages textures and sprites. SlRenderItem struct for adding sprites to SlManager's renderQueue.
*/

#ifndef SLMANAGER_H
#define SLMANAGER_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlTexture.h"
#include "SlSprite.h"
#include "SlRenderItem.h"



class SlManager
{
 public:
  /*! Initializes SDL, creates window with name and width, height
   */
  SlManager(std::string name, int width, int height);

  ~SlManager(void);

  /*! Delete copy constructor. The manager owns the window, don't want copies of that.
   */
  SlManager(const SlManager&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlManager& operator=(const SlManager&) = delete;
  /*! The running instance of SlManager. Currently only one manager allowed. Might change that as a way to have several windows each with a manager and a renderer.
   */
  static SlManager& Instance(void);
  /*! Temporary solution until all rendering related stuff happens in SlManager methods.
   */
  SDL_Renderer* renderer(){return renderer_;}
  
 protected:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  SlManager(void);
  /*! Deletes all textures and sprites, empties render queue.
   */
  void clear();
  /*! Initializes SDL
   */
  void initialize();
  /*!  Initializes window
   */
  void initializeWindow(std::string name, int width, int height);

 private:
  std::vector<SlTexture*> textures;
  std::vector<SlSprite*> sprites;
  std::vector<SlRenderItem> renderQueue;  

};


#endif // SLMANAGER_H
