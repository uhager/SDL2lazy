// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpriteManipulation.cc

  SlSpriteManipulation and derived classes implementation
*/

#include <iostream>
#include <memory>

#include "SlSpriteManager.h"
#include "SlSprite.h"
#include "SlValueParser.h"
#include "SlSpriteManipulation.h"


/*! SlSpriteManipulation base class.
 */
SlSpriteManipulation::SlSpriteManipulation(SlSpriteManager* manager, SlValueParser* valPars)
  : SlManipulation(manager, valPars)
{
  name_ = "spriteManipulation";
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::SlSpriteManipulation] Created " << name_ << std::endl;
#endif
}



SlSpriteManipulation::~SlSpriteManipulation()
{
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::~SlSpriteManipulation] Deleting " << name_ << std::endl;
#endif
}
  


void
SlSpriteManipulation::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::manipulate] " << name_ << std::endl;
#endif
}



/*! SlSMcenterAt implementation
 */
SlSMcenterAt::SlSMcenterAt(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerAt";
}



void
SlSMcenterAt::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(name, destination);

  int targetLocation[2];
  valParser->stringsToNumbers<int>( parameters, targetLocation, 2 );
  toMove->centerAt( targetLocation[0], targetLocation[1] );
}



/*! SlSMcenterIn implementation
 */
SlSMcenterIn::SlSMcenterIn(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerIn";
#ifdef DEBUG
  std::cout << "[SlSMcenterIn::SlSMcenterIn] Created " << name_ << std::endl;
#endif
}



void
SlSMcenterIn::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlSMcenterIn::manipulate] " << name << std::endl;
#endif
  std::shared_ptr<SlSprite> toMove = verifySprite(name, destination);

  std::string targetname = parameters.at(0);
  int targetDest;
  if ( parameters.size() > 1) targetDest = std::stoul(parameters.at(1));
  else targetDest = 0;
  std::shared_ptr<SlSprite> target = verifySprite(targetname, targetDest);
  toMove->centerInSprite( target, destination, targetDest);
}



/*! SlSMsetOptions implementation
 */
SlSMsetOptions::SlSMsetOptions(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "renderOptions";
}



void
SlSMsetOptions::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(name, destination);
  
  int options ;
  bool check = valParser->stringsToRenderOptions( parameters, options );
  if ( check ) toChange->setRenderOptions( options, destination );
}



/*! SlSMsetOrigin implementation
 */
SlSMsetOrigin::SlSMsetOrigin(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "setOrigin";
}



void
SlSMsetOrigin::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(name, destination);

  int origin[2] ;
  valParser->stringsToNumbers<int>( parameters, origin, 2 );
  toMove->setDestinationOrigin( origin[0], origin[1], destination) ; 
}



/*! SlSMcolor implementation
 */
SlSMcolor::SlSMcolor(SlSpriteManager* manager, SlValueParser* valParser)
  : SlSpriteManipulation(manager, valParser)
{
  name_ = "color";
}



void 
SlSMcolor::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(name, destination);
  
  short colours[4] ;
  valParser->stringsToNumbers<short>( parameters, colours, 4 );
  toChange->setColor( colours[0], colours[1], colours[2], colours[3], destination );
}



/*! SlSMsetAngle implementation
 */
SlSMsetAngle::SlSMsetAngle(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "setAngle";
}



void
SlSMsetAngle::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(name, destination);
  
  double angle ;
  valParser->stringsToNumbers<double>( parameters, &angle, 1 );
  toChange->setAngle( angle, destination );
}



