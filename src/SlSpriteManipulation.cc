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
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::SlSpriteManipulation] Created " << name_ << std::endl;
#endif
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
  if ( verified == nullptr ){
#ifdef DEBUG
    std::cout << "[SlSpriteManipulation::verifySprite] Couldn't find sprite to move " << sname << std::endl;
#endif
    return verified;
  }
  if ( destination >= verified->size() ){
#ifdef DEBUG
    std::cout << "[SlSpriteManipulation::verifySprite] Invalid destination for sprite " << sname << std::endl;
#endif
    verified = nullptr;
  }
  return verified;
}



/*! SlMSetOrigin implementation
 */
SlMSetOrigin::SlMSetOrigin(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "setOrigin";
#ifdef DEBUG
  std::cout << "[SlMSetOrigin::SlMSetOrigin] Created " << name_ << std::endl;
#endif
}



void
SlMSetOrigin::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlMSetOrigin::manipulateSprite] " << name_ << std::endl;
#endif
  
  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);
  if ( toMove == nullptr ){
#ifdef DEBUG
    std::cout << "[SlMSetOrigin::manipulateSprite] Invalid sprite or destination " << sname << std::endl;
#endif
    return;
  }

  int origin[2] ;
  bool check = (*valParser)->stringsToInts<int>( parameters, origin, 2 );
  if ( check ) toMove->setDestinationOrigin( origin[0], origin[1], destination) ; 
}



/*! SlMCenterIn implementation
 */
SlMCenterIn::SlMCenterIn(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "centerIn";
#ifdef DEBUG
  std::cout << "[SlMCenterIn::SlMCenterIn] Created " << name_ << std::endl;
#endif
}



void
SlMCenterIn::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlMCenterIn::manipulateSprite] " << name_ << std::endl;
#endif

  std::shared_ptr<SlSprite> toMove = verifySprite(sname, destination);
  if ( toMove == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlMCenterIn::manipulateSprite] Invalid sprite or destination " << sname << std::endl;
#endif
    return;
  }

  std::string targetname = parameters.at(0);
  int targetDest;
  if ( parameters.size() > 1) targetDest = std::stoul(parameters.at(1));
  else targetDest = 0;
  std::shared_ptr<SlSprite> target = verifySprite(targetname, targetDest);
  if ( target == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlMSetOrigin::manipulateSprite] Invalid sprite or destination " << target << std::endl;
#endif
    return;
  }
  toMove->centerInSprite( target, destination, targetDest);
}


/*! SlMSetOptions implementation
 */
SlMSetOptions::SlMSetOptions(SlSpriteManager* manager, SlValueParser** valPars)
  : SlSpriteManipulation(manager, valPars)
{
  name_ = "renderOptions";
#ifdef DEBUG
  std::cout << "[SlMSetOptions::SlMSetOptions] Created " << name_ << std::endl;
#endif
}



void
SlMSetOptions::manipulateSprite(std::string sname, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlMSetOptions::manipulateSprite] " << name_ << std::endl;
#endif

  std::shared_ptr<SlSprite> toChange = verifySprite(sname, destination);
  if ( toChange == nullptr ) {
#ifdef DEBUG
    std::cout << "[SlMCenterIn::manipulateSprite] Invalid sprite or destination " << sname << std::endl;
#endif
    return;
  }
  
  int options ;
  bool check = (*valParser)->stringsToRenderOptions( parameters, options );
  if ( check ) toChange->setRenderOptions( options, destination );
}
