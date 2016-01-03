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

  /*! Adds the sprite 'name' to the end of the #renderQueue_ .
    \retval false if sprite not found or destination out of bounds.
   */ 
  bool appendToRenderQueue(std::string name, unsigned int destination = 0);
  /*! Creates new SlRenderItem for the specified sprite. \n
    \retval false if sprite not found or destination out of bounds.
   */ 
  SlRenderItem* createRenderItem(std::string name, unsigned int destination);
  /*! Creates sprite based on the texture textureName.

    If either width or height are 0, the texture's width and height will be used.\n
    The default values will use the whole texture as a sprite.
    Calls SlSprite::addDefaultDestination(), so if you don't want to use that, either delete or adjust that destination.
   */
  SlSprite* createSprite(std::string name, std::string textureName, int x = 0, int y = 0, int width = 0, int height = 0);
  /*! Load texture from image filename
   */
  SlTexture* createTextureFromFile(std::string name, std::string filename);
  /*! Creates SlTexture using a rectangle of dimension width x height filled with the specified colour. \n
    Creates SlSprite of same name that holds the whole texture.
   */
  SlTexture* createTextureFromRectangle(std::string name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
  /*! Create a new texture by rendering a sprite on this texture.  \n
    Creates SlSprite of same name that holds the whole texture.
   */
  SlTexture* createTextureFromSpriteOnTexture(std::string name, std::string backgroundTexture, std::string foregroundSprite);
  /*! Creates a new texture with dimensions w x h and fills it with tiles of SlSprite tile.  \n
    Creates SlSprite of same name that holds the whole texture.

    The SlRenderSettings::destinationRect at position 0 in the SlSprite's #destinations_ is used to determine the width of the tile. 
    If the sprite's #destinations_ is empty, addDefaultDestination() is called which sets the destinationRect to equal the sourceRect.
  */
  SlTexture* createTextureFromTile(std::string name, std::string sprite, int width, int height);
  /*! Delete the specified sprite and remove from #sprites_ .
   */
  void deleteSprite(std::string name);
  /*! Delete the specified texture and remove from #textures_ . \n 
    Also deletes the SlSprite of same name that was created automatically with the texture.
   */
  void deleteTexture(std::string name);
  /*! Returns pointer to the sprite, nullptr if not found.
   */
  SlSprite* findSprite(std::string name);
  /*! Returns pointer to the texture, nullptr if not found.
   */
  SlTexture* findTexture(std::string name);
  /*! The running instance of SlManager. Currently only one manager allowed. Might change that as a way to have several windows each with a manager and a renderer.
   */
  static SlManager* Instance(void) {return instance_;}
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
  /*! Sets color for SlSprite name at position i of #destinations_.

    Color is use when using color mod to render, and when creating a texture from a rectangle.
   */
  bool setSpriteColor(std::string name, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF, unsigned int destination = 0);
  /*! Changes destinationRect for SlSprite name at position destination of #destinations_.

    Sets where the sprite will be rendered.
  */
  bool setSpriteDestinationOrigin(std::string name,  int x, int y, unsigned int destination = 0);
  /*! Sets SlRenderOptions for SlSprite name at position i of #destinations_.
    \retval false if i > #destinations_.size()
   */
  bool setSpriteRenderOptions(std::string name, uint32_t renderOptions, unsigned int destination = 0);
  
					      
 protected:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  /*!Default constructor.
   */
  SlManager(void);

  /*! Adds toAdd to #textures_ and creates SlSprite of same name. 
   */
  void addTexture(SlTexture* toAdd);
  /*! Deletes all textures and sprites, empties render queue.
   */
  void clear();
  /*! Initializes SDL
   */
  void initialize();
  /*!  Initializes window
   */
  void initializeWindow(std::string name, int width, int height);
  /*! Read sprite configurations from file
   */
  void parseSprites(std::ifstream& input);
  /*! Read texture configurations from file
   */
  void parseTextures(std::ifstream& input);

 private:
  /*! The running instance of SlManager. Currently only one manager allowed. Might change that as a way to have several windows each with a manager and a renderer.
   */
  static SlManager* instance_;
  /*! Holds all SlTextures that were created using SlManager methods.
    Texture will be deleted when the SlManager instance is deleted.
   */
  std::vector<SlTexture*> textures_;
  /*! Holds all SlSprites that were created using SlManager methods.
    Sprites will be deleted when the SlManager instance is deleted.
   */
  std::vector<SlSprite*> sprites_;
  std::vector<SlRenderItem*> renderQueue_;  

};




#endif // SLMANAGER_H
