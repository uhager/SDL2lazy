// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlManipulation.cc

  SlManipulation and derived classes implementation
*/

#include <iostream>

#include "SlSpriteManager.h"
#include "SlSprite.h"
#include "SlValueParser.h"

#include "SlManipulation.h"


/*! SlManipulation base class.
 */
SlManipulation::SlManipulation(SlSpriteManager* manager, SlValueParser* valPars)
{
  smngr_ = manager;
  valParser = valPars;
#ifdef DEBUG
  std::cout << "[SlManipulation::SlManipulation] Creating " << name_ << std::endl;
#endif
}



SlManipulation::~SlManipulation()
{
  smngr_ = nullptr;
  valParser = nullptr;
#ifdef DEBUG
  std::cout << "[SlManipulation::~SlManipulation] Deleting " << name_ << std::endl;
#endif
}



SlManipulation::SlManipulation(const SlManipulation& toCopy)
{
  smngr_ = toCopy.smngr_;
  valParser = toCopy.valParser;
}



SlManipulation&
SlManipulation::operator=(const SlManipulation& rhs)
{
  smngr_ = rhs.smngr_;
  valParser = rhs.valParser;
  return *this;
}



void
SlManipulation::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlManipulation::manipulate] " << name_ << std::endl;
#endif
}



std::shared_ptr<SlSprite>
SlManipulation::verifySprite(const std::string& sname, unsigned int destination)
{
  std::shared_ptr<SlSprite> verified = smngr_->findSprite(sname);
  if ( verified == nullptr )
    throw std::invalid_argument("Couldn't find sprite " + sname);

  if ( destination >= verified->size() )
    throw std::invalid_argument("Invalid destination for sprite " + sname);
  return verified;
}


