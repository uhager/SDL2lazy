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
SlSpriteManipulation::SlSpriteManipulation(SlSpriteManager* manager, SlValueParser** valPars)
{
  smngr_ = manager;
  valParser = valPars;
}



SlSpriteManipulation::~SlSpriteManipulation()
{
  smngr_ = nullptr;
  valParser = nullptr;
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::~SlSpriteManipulation] Deleting " << name_ << std::endl;
#endif
}



SlSpriteManipulation::SlSpriteManipulation(const SlSpriteManipulation& toCopy)
{
  smngr_ = toCopy.smngr_;
  valParser = toCopy.valParser;
}



SlSpriteManipulation&
SlSpriteManipulation::operator=(const SlSpriteManipulation& rhs)
{
  smngr_ = rhs.smngr_;
  valParser = rhs.valParser;
  return *this;
}



void
SlSpriteManipulation::manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::manipulateSprite] " << name_ << std::endl;
#endif
}



std::shared_ptr<SlSprite>
SlSpriteManipulation::verifySprite(std::string sname, unsigned int destination)
{
  std::shared_ptr<SlSprite> verified = smngr_->findSprite(sname);
  if ( verified == nullptr )
    throw std::invalid_argument("Couldn't find sprite " + sname);

  if ( destination >= verified->size() )
    throw std::invalid_argument("Invalid destination for sprite " + sname);
  return verified;
}



/*! SlSMcenterAt implementation
 */
SlSMcenterAt::SlSMcenterAt(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerAt";
}



void
SlSMcenterAt::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);

  int targetLocation[2];
  (*valParser)->stringsToNumbers<int>( parameters, targetLocation, 2 );
  toMove->centerAt( targetLocation[0], targetLocation[1] );
}



/*! SlSMcenterIn implementation
 */
SlSMcenterIn::SlSMcenterIn(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerIn";
}



void
SlSMcenterIn::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);

  std::string targetname = parameters.at(0);
  int targetDest;
  if ( parameters.size() > 1) targetDest = std::stoul(parameters.at(1));
  else targetDest = 0;
  std::shared_ptr<SlSprite> target = verifySprite(targetname, targetDest);
  toMove->centerInSprite( target, destination, targetDest);
}



/*! SlSMsetOptions implementation
 */
SlSMsetOptions::SlSMsetOptions(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "renderOptions";
}



void
SlSMsetOptions::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(sname, destination);
  
  int options ;
  bool check = (*valParser)->stringsToRenderOptions( parameters, options );
  if ( check ) toChange->setRenderOptions( options, destination );
}



/*! SlSMsetOrigin implementation
 */
SlSMsetOrigin::SlSMsetOrigin(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "setOrigin";
}



void
SlSMsetOrigin::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);

  int origin[2] ;
  (*valParser)->stringsToNumbers<int>( parameters, origin, 2 );
  toMove->setDestinationOrigin( origin[0], origin[1], destination) ; 
}



/*! SlSMcolor implementation
 */
SlSMcolor::SlSMcolor(SlSpriteManager* manager, SlValueParser** valParser)
  : SlSpriteManipulation(manager, valParser)
{
  name_ = "color";
}



void 
SlSMcolor::manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(name, destination);
  
  short colours[4] ;
  (*valParser)->stringsToNumbers<short>( parameters, colours, 4 );
  toChange->setColor( colours[0], colours[1], colours[2], colours[3], destination );
}
