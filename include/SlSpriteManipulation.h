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
  SlSpriteManipulation(SlSpriteManager* spritemanager, SlValueParser* valParser);
  ~SlSpriteManipulation();

  SlSpriteManipulation(const SlSpriteManipulation& other) = delete;
  SlSpriteManipulation& operator=(const SlSpriteManipulation& rhs) = delete;

  virtual void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters);
  std::string name() {return name_;}
  SlValueParser* valParser = nullptr;
  
 private:
  std::string name_ = "spriteManipulation";
  SlSpriteManager* smngr_ = nullptr;
};


/*! \class SlSetOrigin derived from SlSpriteManipulation. Sets sprite origin to given x and y coordinates.
 */ 
class SlSetOrigin : public SlSpriteManipulation
{
public:
  SlSetOrigin(SlSpriteManager* manager, SlValueParser* valParser);
  void manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters) override;

 private:
  std::string name_ = "setOrigin";
};





















#endif /* SLSPRITEMANIPULATION_H */
