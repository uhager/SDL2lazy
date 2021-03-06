// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManipulation.h
  \brief SlManipulation class; base class for changing the render queue.
*/


#ifndef SLMANIPULATION_H
#define SLMANIPULATION_H


#include <string>
#include <memory>
#include <vector>

class SlSpriteManager;
class SlSprite;
class SlValueParser;


class SlManipulation
{
 public:
  SlManipulation(SlSpriteManager* smngr, SlValueParser* valPars);
  virtual ~SlManipulation();

  /*! Copy constructor. The #smngr_ and #valParser pointers of both objects will point to the same object; 
   */
  SlManipulation(const SlManipulation& toCopy) ;
  /*! Assignment. The #smngr_ and #valParser pointers of both objects will point to the same object; 
   */
  SlManipulation& operator=(const SlManipulation& rhs) ;

  /*! The actual sprite manipulation, implemented in the derived classes.
   */
  virtual void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters);
  /*! Name can be read but not set. It is defined by the function that the derived class implements so that the correct derived class can be called based on a keyword.
   */
  std::string name() {return name_;}
  /*! Checks the existence of the named sprite and the validity of the destinations.
    \retval sprite of that name if name and destination valid.
    \throws std::invalid_argument if invalid name or destination.
   */
  std::shared_ptr<SlSprite> verifySprite(const std::string& name, unsigned int destination);
  /*! Helper class to translate strings read from file into integers
   */
  SlValueParser* valParser = nullptr;
  
  
 protected:
  /*! Name of the object must be identical to the keyword used in the configuration file.
   */
  std::string name_ = "manipulation";
  /*! The SlSpriteManager that initialized this instance. Needed to find sprites.
   */
  SlSpriteManager* smngr_ = nullptr;
};



#endif  /* SLMANIPULATION_H */
