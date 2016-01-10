// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlSpriteManipulation.cc

  SlSpriteManipulation and derived classes implementation
*/

#include <iostream>

#include "SlSpriteManipulation.h"


/*! SlSpriteManipulation base class.
 */
SlSpriteManipulation::SlSpriteManipulation(SlSpriteManager* manager, SlValueParser* valPars)
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
}



void
SlSpriteManipulation::manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::manipulateSprite] " << name_ << std::endl;
#endif
}



/*! SlSetOrigin implementation
 */
SlSetOrigin::SlSetOrigin(SlSpriteManager* manager, SlValueParser* valPars)
  : SlSpriteManipulation(manager, valPars)
{
#ifdef DEBUG
  std::cout << "[SlSetOrigin::SlSetOrigin] Created " << name_ << std::endl;
#endif
}



void
SlSetOrigin::manipulateSprite(std::string name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlSpriteManipulation::manipulateSprite] " << name_ << std::endl;
#endif
    
}


