// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpriteManager.h
  \brief SlSpriteManager class, has a vector of sprites and various methods to manipulate them.
*/

#ifndef SLSPRITEMANAGER_H
#define SLSPRITEMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class SlSprite;
class SlManager;
class SlTexture;
class SlValueParser;
class SlSpriteManipulation;


class SlSpriteManager
{
 public:
  /*! Initializes SDL, creates window with name and width, height
   */
  SlSpriteManager(SlManager* mngr);

  ~SlSpriteManager();

  /*! Delete copy constructor. The manager owns the window, don't want copies of that.
   */
  SlSpriteManager(const SlSpriteManager&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlSpriteManager& operator=(const SlSpriteManager&) = delete;

  /*! Centers the destination of the sprite in the destinationRect of the target sprite.\n
    Note that if the destination dimensions are changed afterwards, the sprite will no longer be centered.
   */
  void centerSpriteInSprite(const std::string& toCenter, const std::string& target, unsigned int destinationThis = 0, unsigned int destinationOther = 0);
  /*! Creates sprite based on the texture textureName.

    If either width or height are 0, the texture's width and height will be used.\n
    The default values will use the whole texture as a sprite.
    Calls SlSprite::addDefaultDestination(), so if you don't want to use that, either delete or adjust that destination.
   */
  std::shared_ptr<SlSprite> createSprite(SlTexture* texture, int x = 0, int y = 0, int width = 0, int height = 0);
  /*! Creates sprite based on the texture textureName.

    If either width or height are 0, the texture's width and height will be used.\n
    The default values will use the whole texture as a sprite.
    Calls SlSprite::addDefaultDestination(), so if you don't want to use that, either delete or adjust that destination.
   */
  std::shared_ptr<SlSprite> createSprite(const std::string& name, const std::string& textureName, int x = 0, int y = 0, int width = 0, int height = 0);
  /*! Delete the specified sprite and remove from #sprites_ .
   */
  void deleteSprite(const std::string& name);
  /*! Returns pointer to the sprite, nullptr if not found.
   */
  std::shared_ptr<SlSprite> findSprite(const std::string& name);
  /*! Read sprite configurations from file
   */
  void parseSprite(std::ifstream& input);
  /*! Read sprite placement from file.
   */
  void parseSpriteManipulation(std::ifstream& input);
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
  /*! Helper object to translate file input into values.
   */
  SlValueParser* valParser = nullptr;
					      
 protected:
  /*!Default constructor.
   */
  SlSpriteManager();
  /*! Deletes all sprites.
   */
  void clear();
  /*! Move the sprite based on configuration file.\
    Currently implemented whatToDo:\n
    setOrigin
   */
  void manipulateSprite(const std::string& name, unsigned int destination, const std::string& whatToDo, const std::vector<std::string>& parameters);
  
 private:
  /*! Holds all SlSprites that were created using SlSpriteManager methods.
    Sprites will be deleted when the SlSpriteManager instance is deleted.
   */
  std::vector<std::shared_ptr<SlSprite>> sprites_;
  /*! The running SlManager that called the constructor.
   */
  SlManager* mngr_ = nullptr;
};




#endif // SLSPRITEMANAGER_H
