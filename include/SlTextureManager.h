// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlTextureManager.h
  \brief SlTextureManager class creates and stores textures.
*/

#ifndef SLTEXTUREMANAGER_H
#define SLTEXTUREMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>



class SlManager;
class SlSprite;
class SlTexture;
class SlValueParser;
class SlFont;


class SlTextureManager
{
 public:
  /*! Initializes SDL, creates window with name and width, height
   */
  SlTextureManager(SlManager* manager);

  ~SlTextureManager();

  /*! Delete copy constructor. The manager owns the window, don't want copies of that.
   */
  SlTextureManager(const SlTextureManager&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlTextureManager& operator=(const SlTextureManager&) = delete;

  /*! Load texture from image filename
   */
  SlTexture* createTextureFromFile(const std::string& name, const std::string& filename);
  /*! Creates SlTexture using a rectangle of dimension width x height filled with the specified colour. \n
    Creates SlSprite of same name that holds the whole texture.
   */
  SlTexture* createTextureFromRectangle(const std::string& name, int width, int height, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
  /*! Create a new texture by rendering a sprite on this texture.  \n
    Creates SlSprite of same name that holds the whole texture.
   */
  SlTexture* createTextureFromSpriteOnTexture(const std::string& name, const std::string& backgroundTexture, const std::string& foregroundSprite);
/*! Create a new texture from the message using the specified font.
 */
SlTexture* createTextureFromText(const std::string& name, const std::string& fontname, const std::string& message); 
  /*! Creates a new texture with dimensions w x h and fills it with tiles of SlSprite tile.  \n
    Creates SlSprite of same name that holds the whole texture.

    The SlRenderSettings::destinationRect at position 0 in the SlSprite::destinations_ is used to determine the width of the tile. 
    If the SlSprite::destinations_ is empty, addDefaultDestination() is called which sets the destinationRect to equal the sourceRect.
  */
  SlTexture* createTextureFromTile(const std::string& name, const std::string& sprite, int width, int height);
  /*! Delete the specified texture and remove from #textures_ . \n 
    Also deletes the SlSprite of same name that was created automatically with the texture.
   */
  void deleteTexture(const std::string& name);
  /*! Returns pointer to the named font. 
    \retval nullptr if not found
   */
  std::shared_ptr<SlFont> findFont(const std::string& name);
  /*! Returns pointer to the named texture.
    \retval nullptr if not found
   */
  SlTexture* findTexture(const std::string& name);
  /*! Read font file, size, colour from file
  */
  std::shared_ptr<SlFont> parseFont(std::ifstream& input);
  /*! Read texture configurations from file
  */
  SlTexture* parseTexture(std::ifstream& input);
  /*! Helper object to translate file input into values.
   */
  SlValueParser* valParser = nullptr;
  
 protected:
  /*!Default constructor.
   */
  SlTextureManager(void);
  /*! Adds toAdd to #textures_ and creates SlSprite of same name. 
   */
  void addTexture(SlTexture* toAdd);
  /*! Deletes all textures and sprites, empties render queue.
   */
  void clear();

  
 private:
  /*! Holds all SlTextures that were created using SlTextureManager methods.
    Texture will be deleted when the SlTextureManager instance is deleted.
   */
  std::vector<SlTexture*> textures_;
  /*! Pointer to the running SlManager that created this TextureManager.
   */
  SlManager* mngr_ = nullptr;
  /*! Font used for rendering. This is kept open until program exits to reduce overhead from opening and closing font file.
   */
std::vector<std::shared_ptr<SlFont>> fonts_ ;

};




#endif // SLTEXTUREMANAGER_H
