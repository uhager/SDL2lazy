// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpriteManipulation.h
  \brief SlSpriteManipulation class; base class for changing sprite properties.
*/

#ifndef SLSPRITEMANIPULATION_H
#define SLSPRITEMANIPULATION_H

#include <string>
#include <vector>
#include <memory>



class SlSprite;
class SlSpriteManager;
class SlValueParser;

/*! \class SlSpriteManipulation, base class for changing sprite properties.
 */
class SlSpriteManipulation
{
 public:
  SlSpriteManipulation(SlSpriteManager* spritemanager, SlValueParser** valParser);
  virtual ~SlSpriteManipulation();

  SlSpriteManipulation(const SlSpriteManipulation& toCopy) ;
  SlSpriteManipulation& operator=(const SlSpriteManipulation& rhs) ;

  /*! The actual sprite manipulation, implemented in the derived classes.
   */
  virtual void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters);
  std::shared_ptr<SlSprite> verifySprite(std::string name, unsigned int destination);

  std::string name() {return name_;}
  /*! Helper class to translate strings read from file into integers
   */
  SlValueParser** valParser = nullptr;
  
  
 protected:
  /*! Name of the object must be identical to the keyword used in the configuration file.
   */
  std::string name_ = "spriteManipulation";
  SlSpriteManager* smngr_ = nullptr;
};


/*! \class SlMSetOrigin derived from SlSpriteManipulation. Sets sprite origin to given x and y coordinates.
 */ 
class SlMSetOrigin : public SlSpriteManipulation
{
public:
  SlMSetOrigin(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlMCenterIn derived from SlSpriteManipulation. Centers sprite in another sprite.
 */ 
class SlMCenterIn : public SlSpriteManipulation
{
public:
  SlMCenterIn(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};





















#endif /* SLSPRITEMANIPULATION_H */
