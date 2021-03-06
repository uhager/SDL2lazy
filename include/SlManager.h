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
#include "SlValueParser.h"
#include "SlEventHandler.h"

class SlTexture;
class SlSprite;
class SlRenderItem;
class SlManipulation;


class SlManager
{
 public:
  /*!Default constructor. Parses "SlApplication.ini" for window name and dimensions, and configuration file names.
   */
  SlManager();
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
    \retval nullptr if sprite not found or destination out of bounds.
   */ 
  SlRenderItem* createRenderItem(const std::string& name, unsigned int destination);
  /*! Delete all render items with the specified name from the #renderQueue_ .
    Should be called before deleteSprite(const std::string& name).
   */
  void deleteRenderItem(const std::string& name);
  /*! Tells #smngr_ to delete the specified sprite. Also deletes all associated entries in #renderQueue_ .
   */
  void deleteSprite(const std::string& name);
   /*! Tells #tmngr_ to delete the specified texture and remove from SlTextureManager::textures_ . Also deletes all associates SlSprites.
   */
  void deleteTexture(const std::string& name);
  /*! Returns pointer to the sprite, nullptr if not found. 
   */
  std::shared_ptr<SlSprite> findSprite(const std::string& name);
  /*! Returns pointer to the texture, nullptr if not found.
    Calls SlTextureManager #tmngr_ .
   */
  SlTexture* findTexture(const std::string& name);
  /*! Pass event on to SlEventHandler #eventHandler_ . Deprecated, just call run().
   */ 
  inline void handleEvent(const SDL_Event& event);
  /*! Inserts the sprite into the #renderQueue_ after the specified sprite.
   */
  void insertInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Inserts the sprite into the #renderQueue_ before the specified sprite.
   */
  void insertInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Moves the render item into the #renderQueue_ before the specified sprite.
   */
  bool moveInRenderQueueBefore(const std::string& toAdd, const std::string& beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Moves the render item into the #renderQueue_ after the specified sprite.
   */
  bool moveInRenderQueueAfter(const std::string& toAdd, const std::string& afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Move the render item based on configuration file.\
    Currently implemented whatToDo:\n
    append
   */
  void manipulateRenderQueue(const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters );
  /*! Add sprites to render queue, change to order of the queue. (Currently only implemented: append.)
   */
  void parseRenderQueueManipulation( std::ifstream& input );
  /*! Read texture and sprite definitions from configuration file.\n
    Default file name: "SlTextures.ini".
   */
  bool parseConfigurationFile(const std::string& filename = "SlTextureConfig.ini");
  /*! Reads application name, screen dimensions and config file names from from ini file "SlApplication.ini".\n
   */
  void parseIniFile(const std::string& filename = "SlApplication.ini");
  /*! Render all items in the #renderQueue_ .
    \retval 0 if all renders well.
   */
  void render();
  /*! Temporary solution until all rendering related stuff happens in SlManager methods.
   */
  SDL_Renderer* renderer(){return renderer_;}
  /*! Run the event - render loop.
   */
  void run();
  /*! Returns the #screen_width_ of the window.
   */
  int screenWidth(){ return screen_width_; }
  /*! Returns the #screen_height_ of the window.
   */
  int screenHeight(){ return screen_height_; }
  /*! Sets color for SlSprite name at position i of SlSprite::destinations_.

    Color is use when using color mod to render, and when creating a texture from a rectangle.
   */
  inline void setSpriteColor(const std::string& name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF, unsigned int destination = 0);
  /*! Changes destinationRect for SlSprite name at position destination of SlSprite::destinations_.

    Sets where the sprite will be rendered.
  */
  inline void setSpriteDestinationOrigin(const std::string& name,  int x, int y, unsigned int destination = 0);
  /*! Sets SlRenderOptions for SlSprite name at position i of SlSprite::destinations_.
    \retval false if i > SlSprite::destinations_ size.
   */
  inline void setSpriteRenderOptions(const std::string& name, uint32_t renderOptions, unsigned int destination = 0);
  /*! Replaces the sprite 'toRemove' in the #renderQueue_ with sprite 'toAdd' .
    \retval false if sprite not found or destination out of bounds.
   */ 
  void swapInRenderQueue(const std::string& toAdd, const std::string& toRemove, unsigned int destToAdd = 0, unsigned int destToRemove = 0);
  /*! Replaces the SlRenderItem at 'position' in the #renderQueue_ with sprite 'toAdd'. \n
    Note that the first item is in position 0.
   */
  void swapInRenderQueueAtPosition(const std::string& toAdd, unsigned int destToAdd, unsigned int position );
  /*! Replaces the last item in the #renderQueue_ with sprite toAdd.
   */
  void swapInRenderQueueLastPosition(const std::string& toAdd, unsigned int destToAdd = 0 );
  /*! Toggles rendering of an item in the #renderQueue_ on and off. 
    onOrOff is -1 to toggle, 0 to turn off, 1 to turn on.
   */
  void toggleRender(const std::string& toToggle, unsigned int destination = 0, int onOrOff = -1);
  /*! Turns off rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  inline void toggleRenderOff(const std::string& toToggle, unsigned int destination = 0);
  /*! Turns on rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  inline void toggleRenderOn(const std::string& toToggle, unsigned int destination = 0);
					      
 protected:
  /*! The main (and only) window.
   */
  SDL_Window* window_ = nullptr;
  /*! The renderer tied to the main window.
   */
  SDL_Renderer* renderer_ = nullptr;
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
  std::shared_ptr<SlSpriteManager> smngr_  = nullptr;
  /*! Event handler, parses events in configuration file, triggers SlManipulation on key input.
   */
  std::unique_ptr<SlEventHandler> eventHandler_  = nullptr;
  /*! Window width.
   */
  int screen_width_ ;
  /*! Window height
   */
  int screen_height_ ;
  /*! Helper object to translate file input into values.
   */
  SlValueParser valParser_;
  /*! Holds the various SlRenderQueueManipulations for the different tasks.
    The map key is the name of the manipulation, which is also the keyword used in the configuration file, the mapped value is the object that will do the actual work.
   */
  std::map<std::string, SlManipulation*> renderManip_;

};




#endif // SLMANAGER_H
