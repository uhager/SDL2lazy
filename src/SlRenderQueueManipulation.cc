// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderQueueManipulation.cc

  SlRenderQueueManipulation and derived classes implementation
*/


#include <stdexcept>
#include <memory>
#include <string>

#include "SlSpriteManager.h"
#include "SlRenderItem.h"

#include "SlRenderQueueManipulation.h"


SlRenderQueueManipulation::SlRenderQueueManipulation(SlSpriteManager* smngr, std::vector<SlRenderItem*>* renderQueue)
  : smngr_(smngr)
  , renderQueue_(renderQueue)
{
}



void 
SlRenderQueueManipulation::manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::manipulateQueue]" << std::endl;
#endif
}



std::shared_ptr<SlSprite>
SlRenderQueueManipulation::verifySprite(std::string sname, unsigned int destination)
{
  std::shared_ptr<SlSprite> verified = smngr_->findSprite(sname);
  if ( verified == nullptr )
    throw std::invalid_argument("Couldn't find sprite " + sname);

  if ( destination >= verified->size() )
    throw std::invalid_argument("Invalid destination for sprite " + sname);
  return verified;
}
