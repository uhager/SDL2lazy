// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderQueueManipulation.cc

  SlRenderQueueManipulation and derived classes implementation
*/


#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>

#include "SlSpriteManager.h"
#include "SlRenderItem.h"

#include "SlRenderQueueManipulation.h"


SlRenderQueueManipulation::SlRenderQueueManipulation(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue)
  : smngr_(smngr)
  , renderQueue_(renderQueue)
{
}



SlRenderQueueManipulation::~SlRenderQueueManipulation()
{
  smngr_ = nullptr;
  renderQueue_ = nullptr;
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation] deleting " << name_ << std::endl;
#endif
}



SlRenderItem* 
SlRenderQueueManipulation::createRenderItem(const std::string& name, unsigned int destination)
{
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::createRenderItem] Creating item for " << name  << std::endl;
#endif
  std::shared_ptr<SlSprite> sprite = verifySprite(name, destination);
  return new SlRenderItem(sprite, destination);
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




/*! \class SlRMappend implementation
 */
SlRMappend::SlRMappend(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, renderQueue )
{
  name_ = "append";
}



void 
SlRMappend::manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( toAdd ) {
    renderQueue_->push_back(toAdd);
  }
}



/*! \class SlRMinsertAfter implementation
 */
SlRMinsertAfter::SlRMinsertAfter(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, renderQueue )
{
  name_ = "insertAfter";
}



void 
SlRMinsertAfter::manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMinsertAfter::manipulateQueue] Error: no name given for object to insert after.");

  std::string afterThis = parameters.at(0);
  unsigned int destAfterThis = std::stoi( parameters.at(1) );

  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMinsertAfter::manipulateQueue] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  afterThis ) && ( (*iter)->destination_ == destAfterThis ) ) {
      renderQueue_->insert( (++iter), toAdd );
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error("[SlRMinsertAfter::manipulateQueue] Couldn't find RenderItem " + afterThis + " to insert after.");
}



/*! \class SlRMinsertBefore implementation
 */
SlRMinsertBefore::SlRMinsertBefore(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, renderQueue )
{
  name_ = "insertBefore";
}



void 
SlRMinsertBefore::manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMinsertBefore::manipulateQueue] Error: no name given for object to insert before.");

  std::string beforeThis = parameters.at(0);
  unsigned int destBeforeThis = std::stoi( parameters.at(1) );

  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMinsertBefore::manipulateQueue] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  beforeThis ) && ( (*iter)->destination_ == destBeforeThis ) ) {
      renderQueue_->insert( (iter), toAdd );
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error("[SlRMinsertBefore::manipulateQueue] Couldn't find RenderItem " + beforeThis + " to insert before.");

}
