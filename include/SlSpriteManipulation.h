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

#include "SlManipulation.h"

class SlSprite;
class SlSpriteManager;
class SlValueParser;

/*! \class SlSpriteManipulation, base class for changing sprite properties.
 */
class SlSpriteManipulation : public SlManipulation
{
 public:
  SlSpriteManipulation(SlSpriteManager* spritemanager, SlValueParser* valPars);
  virtual ~SlSpriteManipulation();
  /*! The actual sprite manipulation, implemented in the derived classes.
   */
  virtual void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters);
};



/*! \class SlSMCenterIn derived from SlSpriteManipulation. Centers sprite in another sprite.
 */ 
class SlSMcenterAt : public SlSpriteManipulation
{
public:
  SlSMcenterAt(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMCenterIn derived from SlSpriteManipulation. Centers sprite in another sprite.
 */ 
class SlSMcenterIn : public SlSpriteManipulation
{
public:
  SlSMcenterIn(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMSetOptions derived from SlSpriteManipulation. Sets renderOptions for sprite.
 */ 
class SlSMsetOptions : public SlSpriteManipulation
{
public:
  SlSMsetOptions(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMSetOrigin derived from SlSpriteManipulation. Sets sprite origin to given x and y coordinates.
 */ 
class SlSMsetOrigin : public SlSpriteManipulation
{
public:
  SlSMsetOrigin(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMcolor derived from SlSpriteManipulation. Sets renderOptions for sprite.
 */ 
class SlSMcolor : public SlSpriteManipulation
{
public:
  SlSMcolor(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlSMsetAngle derived from SlSpriteManipulation. Sets renderOptions for sprite.
 */ 
class SlSMsetAngle : public SlSpriteManipulation
{
public:
  SlSMsetAngle(SlSpriteManager* manager, SlValueParser* valPars);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};







#endif /* SLSPRITEMANIPULATION_H */
