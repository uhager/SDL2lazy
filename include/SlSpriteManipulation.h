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

  /*! Copy constructor. The #smngr_ and #valParser pointers of both objects will point to the same object; 
   */
  SlSpriteManipulation(const SlSpriteManipulation& toCopy) ;
  /*! Assignment. The #smngr_ and #valParser pointers of both objects will point to the same object; 
   */
  SlSpriteManipulation& operator=(const SlSpriteManipulation& rhs) ;

  /*! The actual sprite manipulation, implemented in the derived classes.
   */
  virtual void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters);
  /*! Name can be read but not set. It is defined by the function that the derived class implements so that the correct derived class can be called based on a keyword.
   */
  std::string name() {return name_;}
  /*! Checks the existence of the named sprite and the validity of the destinations.
    \retval sprite of that name if name and destination valid.
    \throws std::invalid_argument if invalid name or destination.
   */
  std::shared_ptr<SlSprite> verifySprite(std::string name, unsigned int destination);
  /*! Helper class to translate strings read from file into integers
   */
  SlValueParser** valParser = nullptr;
  
  
 protected:
  /*! Name of the object must be identical to the keyword used in the configuration file.
   */
  std::string name_ = "spriteManipulation";
  /*! The SlSpriteManager that initialized this instance. Needed to find sprites.
   */
  SlSpriteManager* smngr_ = nullptr;
};



/*! \class SlSMCenterIn derived from SlSpriteManipulation. Centers sprite in another sprite.
 */ 
class SlSMcenterAt : public SlSpriteManipulation
{
public:
  SlSMcenterAt(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMCenterIn derived from SlSpriteManipulation. Centers sprite in another sprite.
 */ 
class SlSMcenterIn : public SlSpriteManipulation
{
public:
  SlSMcenterIn(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMSetOptions derived from SlSpriteManipulation. Sets renderOptions for sprite.
 */ 
class SlSMsetOptions : public SlSpriteManipulation
{
public:
  SlSMsetOptions(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMSetOrigin derived from SlSpriteManipulation. Sets sprite origin to given x and y coordinates.
 */ 
class SlSMsetOrigin : public SlSpriteManipulation
{
public:
  SlSMsetOrigin(SlSpriteManager* manager, SlValueParser** valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;
};








#endif /* SLSPRITEMANIPULATION_H */
