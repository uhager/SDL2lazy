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



class SlTexture;
class SlSprite;
class SlRenderItem;
class SlTextureManager;

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

  /*! Adds the sprite 'name' to the end of the #renderQueue_ .
    \retval false if sprite not found or destination out of bounds.
   */ 
  bool appendToRenderQueue(std::string name, unsigned int destination = 0);
  /*! Centers the destination of the sprite in the destinationRect of the target sprite.\n
    Note that if the destination dimensions are changed afterwards, the sprite will no longer be centered.
   */
  void centerSpriteInSprite(std::string toCenter, std::string target, unsigned int destinationThis = 0, unsigned int destinationOther = 0);
  /*! Creates new SlRenderItem for the specified sprite. \n
    \retval false if sprite not found or destination out of bounds.
   */ 
  SlRenderItem* createRenderItem(std::string name, unsigned int destination);
  /*! Creates sprite based on the texture textureName.

    If either width or height are 0, the texture's width and height will be used.\n
    The default values will use the whole texture as a sprite.
    Calls SlSprite::addDefaultDestination(), so if you don't want to use that, either delete or adjust that destination.
   */
  std::shared_ptr<SlSprite> createSprite(std::string name, std::string textureName, int x = 0, int y = 0, int width = 0, int height = 0);
  /*! Delete the specified sprite and remove from #sprites_ .
   */
  void deleteSprite(std::string name);
   /*! Tells #tmngr_ to delete the specified texture and remove from SlTextureManager::textures_ . \n 
    Also deletes the SlSprite of same name that was created automatically with the texture.
   */
  void deleteTexture(std::string name);
  /*! Extract integer values from strings read from file.
   */
  bool determineValues(std::vector<std::string> stringValues, int *values, unsigned int valueSize ); 
  /*! Returns pointer to the sprite, nullptr if not found.
   */
  std::shared_ptr<SlSprite> findSprite(std::string name);
  /*! Returns pointer to the texture, nullptr if not found.
   */
  SlTexture* findTexture(std::string name);
  /*! Inserts the sprite into the #renderQueue_ after the specified sprite.
   */
  bool insertInRenderQueueAfter(std::string toAdd, std::string afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Inserts the sprite into the #renderQueue_ before the specified sprite.
   */
  bool moveInRenderQueueBefore(std::string toAdd, std::string beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Inserts the sprite into the #renderQueue_ after the specified sprite.
   */
  bool moveInRenderQueueAfter(std::string toAdd, std::string afterThis, unsigned int destToAdd = 0, unsigned int destAfterThis = 0);
  /*! Inserts the sprite into the #renderQueue_ before the specified sprite.
   */
  bool insertInRenderQueueBefore(std::string toAdd, std::string beforeThis, unsigned int destToAdd = 0, unsigned int destBeforeThis = 0);
  /*! Read texture and sprite definitions from configuration file.\n
    Default file name: "SlTextures.ini".
   */
  bool parseConfigurationFile(std::string filename = "SlTextureConfig.ini");
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
  bool setSpriteColor(std::string name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF, unsigned int destination = 0);
  /*! Changes destinationRect for SlSprite name at position destination of SlSprite::destinations_.

    Sets where the sprite will be rendered.
  */
  bool setSpriteDestinationOrigin(std::string name,  int x, int y, unsigned int destination = 0);
  /*! Sets SlRenderOptions for SlSprite name at position i of SlSprite::destinations_.
    \retval false if i > SlSprite::destinations_ size.
   */
  bool setSpriteRenderOptions(std::string name, uint32_t renderOptions, unsigned int destination = 0);
  /*! Replaces the sprite 'toRemove' in the #renderQueue_ with sprite 'toAdd' .
    \retval false if sprite not found or destination out of bounds.
   */ 
  bool swapInRenderQueue(std::string toAdd, std::string toRemove, unsigned int destToAdd = 0, unsigned int destToRemove = 0);
  /*! Replaces the SlRenderItem at 'position' in the #renderQueue_ with sprite 'toAdd'. \n
    Note that the first item is in position 0.
   */
  bool swapInRenderQueueAtPosition(std::string toAdd, unsigned int destToAdd, unsigned int position );
  /*! Replaces the last item in the #renderQueue_ with sprite toAdd.
   */
  bool swapInRenderQueueLastPosition(std::string toAdd, unsigned int destToAdd = 0 );
  /*! Toggles rendering of an item in the #renderQueue_ on and off. 
    onOrOff is -1 to toggle, 0 to turn off, 1 to turn on.
   */
  bool toggleRender(std::string toToggle, unsigned int destination = 0, int onOrOff = -1);
  /*! Turns off rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  bool toggleRenderOff(std::string toToggle, unsigned int destination = 0);
  /*! Turns on rendering for the specified SlRenderItem in the #renderQueue_ .
   */
  bool toggleRenderOn(std::string toToggle, unsigned int destination = 0);
					      
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
  void initializeWindow(std::string name, int width, int height);
  /*! Move the sprite based on configuration file.\
    Currently implemented whatToDo:\n
    setOrigin
   */
  void moveSprite(std::string name, unsigned int destination, std::string whatToDo, std::vector<std::string> coordinates);
  /*! Read sprite configurations from file
   */
  void parseSprite(std::ifstream& input);
  /*! Read sprite placement from file.
   */
  void parseSpriteMovement(std::ifstream& input);
  
 private:
  /*! Holds all SlSprites that were created using SlManager methods.
    Sprites will be deleted when the SlManager instance is deleted.
   */
  std::vector<std::shared_ptr<SlSprite>> sprites_;
  /*! Holds the items to be rendered. The front of the queue is rendered first (background) the last element is rendered last (foreground).
   */
  std::vector<SlRenderItem*> renderQueue_;
  /*! Texture manager, creates, stores, deletes textures
   */
  SlTextureManager* tmngr_ = nullptr;
  /*! Window width.
   */
  int screen_width_ ;
  /*! Window height
   */
  int screen_height_ ;

};




#endif // SLMANAGER_H
