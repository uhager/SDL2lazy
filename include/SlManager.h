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
  /*! Load texture from image fileName
   */
  SlTexture* createTextureFromFile(std::string name, std::string fileName);
  /*! Creates SlTexture using a rectangle of dimension width x height filled with the specified colour.
   */
  SlTexture* createTextureFromRectangle(std::string name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
  /*! Create a new texture by rendering a sprite on this texture.
   */
  SlTexture* createTextureFromSpriteOnTexture(std::string name, SlTexture* backgroundTexture, SlSprite *foregroundSprite);
  /*! Creates a new texture with dimensions w x h and fills it with tiles of SlSprite tile. 

    The SlRenderSettings::destinationRect at position 0 in the SlSprite's #destinations_ is used to determine the width of the tile. 
    If the sprite's #destinations_ is empty, addDefaultDestination() is called which sets the destinationRect to equal the sourceRect.
  */
  SlTexture* createTextureFromTile(std::string name, SlSprite* sprite, int width, int height);
  /*! Delete the specified texture and remove from #textures_ .
   */
  void deleteTexture(std::string name);

  
					      
 protected:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  /*!Default constructor.
   */
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
  /*! Holds all SlTextures that were created using SlManager methods.
    Texture will be deleted when the SlManager instance is deleted.
   */
  std::vector<SlTexture*> textures_;
  /*! Holds all SlSprites that were created using SlManager methods.
    Sprites will be deleted when the SlManager instance is deleted.
   */
  std::vector<SlSprite*> sprites_;
  std::vector<SlRenderItem> renderQueue_;  

};


#endif // SLMANAGER_H
