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



/*! SlMSetOrigin implementation
 */
SlMSetOrigin::SlMSetOrigin(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "setOrigin";
}



void
SlMSetOrigin::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);

  int origin[2] ;
  (*valParser)->stringsToNumbers<int>( parameters, origin, 2 );
  toMove->setDestinationOrigin( origin[0], origin[1], destination) ; 
}



/*! SlMCenterIn implementation
 */
SlMCenterIn::SlMCenterIn(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerIn";
}



void
SlMCenterIn::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);

  std::string targetname = parameters.at(0);
  int targetDest;
  if ( parameters.size() > 1) targetDest = std::stoul(parameters.at(1));
  else targetDest = 0;
  std::shared_ptr<SlSprite> target = verifySprite(targetname, targetDest);
  toMove->centerInSprite( target, destination, targetDest);
}


/*! SlMSetOptions implementation
 */
SlMSetOptions::SlMSetOptions(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "renderOptions";
}



void
SlMSetOptions::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
  std::shared_ptr<SlSprite> toChange = verifySprite(sname, destination);
  
  int options ;
  bool check = (*valParser)->stringsToRenderOptions( parameters, options );
  if ( check ) toChange->setRenderOptions( options, destination );
}
