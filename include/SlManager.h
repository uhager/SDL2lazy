// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManager.h
  \brief SlManager class, has a Window with SDL_Renderer, manages textures and sprites. SlRenderItem struct for adding sprites to SlManager's renderQueue.
*/

#ifndef SLMANAGER_H
#define SLMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlTextureManager.h"
#include "SlSpriteManager.h"


class SlTexture;
class SlSprite;
class SlRenderItem;

class SlManager
{
 public:
  /*! Initializes SDL, creates window with name and width, height
   */
  SlManager(const std::string& name, int width, int height);

  ~SlManager(void);

  /*! Delete copy constructor. The manager owns the window, don't want copies of that.
   */
  SlManager(const SlManager&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlManager& operator=(const SlManager&) = delete;

  /*! Adds the sprite 'name' to the end of the #renderQueue_ .
    \retval false if sprite not found or destination out of bounds.
   */ 
  bool appendToRenderQueue(const std::string& name, unsigned int destination = 0);
  /*! Creates new SlRenderItem for the specified sprite. \n
    \retval false if sprite not found or destination out of bounds.
   */ 
  SlRenderItem* createRenderItem(const std::string& name, unsigned int destination);
  /*! Delete all render items with the specified name from the #renderQueue_ .
    Should be called before deleteSprite(const std::string& name).
   */
  void deleteRenderItem(const std::string& name);
  /*! Delete the specified sprite and remove from #sprites_ .
    Should be called before deleteTexture(const std::string& name).
   */
  void deleteSprite(const std::string& name);
   /*! Tells #tmngr_ to delete the specified texture and remove from SlTextureManager::textures_ . \n 
   */
  void deleteTexture(const std::string& name);
  /*! Extract integer values from strings read from file.
   */
  bool determineValues(const std::vector<std::string>& stringValues, int *values, unsigned int valueSize ); 
  /*! Returns pointer to the sprite, nullptr if not found. 
   */
  std::shared_ptr<SlSprite> findSprite(const std::string& name);
  /*! Returns pointer to the texture, nullptr if not found.
    Calls SlTextureManager #tmngr_ .
   */
  SlTexture* findTexture(const std::string& name);
  /*! Inserts the sprite into the #renderQueue_ after the specified sprite.
   */
  bool insertInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Inserts the sprite into the #renderQueue_ before the specified sprite.
   */
  bool moveInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Inserts the sprite into the #renderQueue_ after the specified sprite.
   */
  bool moveInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Inserts the sprite into the #renderQueue_ before the specified sprite.
   */
  bool insertInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Read texture and sprite definitions from configuration file.\n
    Default file name: "SlTextures.ini".
   */
  bool parseConfigurationFile(const std::string& filename = "SlTextureConfig.ini");
  /*! Render all items in the #renderQueue_ .
    \retval 0 if all renders well.
   */
  int render();
  /*! Temporary solution until all rendering related stuff happens in SlManager methods.
   */
  SDL_Renderer* renderer(){return renderer_;}
  /*! Returns the #screen_width_ of the window.
   */
  int screenWidth(){ return screen_width_; }
  /*! Returns the #screen_height_ of the window.
   */
  int screenHeight(){ return screen_height_; }
  /*! Sets color for SlSprite name at position i of SlSprite::destinations_.

    Color is use when using color mod to render, and when creating a texture from a rectangle.
   */
  bool setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF, unsigned int destination = 0);
  /*! Changes destinationRect for SlSprite name at position destination of SlSprite::destinations_.

    Sets where the sprite will be rendered.
  */
  bool setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination = 0);
  /*! Sets SlRenderOptions for SlSprite name at position i of SlSprite::destinations_.
    \retval false if i > SlSprite::destinations_ size.
   */
  bool setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination = 0);
  /*! Replaces the sprite 'toRemove' in the #renderQueue_ with sprite 'toAdd' .
    \retval false if sprite not found or destination out of bounds.
   */ 
  bool swapInRenderQueue(const std::string& toAdd, const std::string& toRemove, unsigned int destToAdd = 0, unsigned int destToRemove = 0);
  /*! Replaces the SlRenderItem at 'position' in the #renderQueue_ with sprite 'toAdd'. \n
    Note that the first item is in position 0.
   */
  bool swapInRenderQueueAtPosition(const std::string& toAdd, unsigned int destToAdd, unsigned int position );
  /*! Replaces the last item in the #renderQueue_ with sprite toAdd.
   */
  bool swapInRenderQueueLastPosition(const std::string& toAdd, unsigned int destToAdd = 0 );
  /*! Toggles rendering of an item in the #renderQueue_ on and off. 
    onOrOff is -1 to toggle, 0 to turn off, 1 to turn on.
   */
  bool toggleRender(const std::string& toToggle, unsigned int destination = 0, int onOrOff = -1);
  /*! Turns off rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  bool toggleRenderOff(const std::string& toToggle, unsigned int destination = 0);
  /*! Turns on rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  bool toggleRenderOn(const std::string& toToggle, unsigned int destination = 0);
					      
 protected:
  /*! The main (and only) window.
   */
  SDL_Window* window_ = nullptr;
  /*! The renderer tied to the main window.
   */
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
  void initializeWindow(const std::string& name, int width, int height);
  /*! Moves a SlRenderItem's position in the #renderQueue_ to before of after the target item.\n
    beforeOrAfter is 0 for before, 1 for after. Larger (smaller) values will result in the target item begin inserted further upstream (downstream) of the target. 
    (Handle with care, currently no test for valid iterators beyond +1...)
   */
  bool moveInRenderQueue(const std::string& toMoveName, const std::string& targetName, unsigned int destToMove = 0, unsigned int targetDest = 0, int beforeOrAfter = 0);
  
 private:
  /*! Holds the items to be rendered. The front of the queue is rendered first (background) the last element is rendered last (foreground).
   */
  std::vector<SlRenderItem*> renderQueue_;
  /*! Texture manager, creates, stores, deletes SlTexture objects.
   */
  std::unique_ptr<SlTextureManager> tmngr_  = nullptr;
  /*! Sprite manager, creates, stores, manipulates, and deletes SlSprite objects.
   */
  std::unique_ptr<SlSpriteManager> smngr_  = nullptr;
  /*! Window width.
   */
  int screen_width_ ;
  /*! Window height
   */
  int screen_height_ ;

};




#endif // SLMANAGER_H
