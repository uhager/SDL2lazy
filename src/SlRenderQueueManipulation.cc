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


SlRenderQueueManipulation::SlRenderQueueManipulation(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlManipulation(smngr, valPars)
{
  renderQueue_ = renderQueue;
  name_ = "renderQueueManipulation";
}



SlRenderQueueManipulation::~SlRenderQueueManipulation()
{
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
SlRenderQueueManipulation::manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::manipulate]" << std::endl;
#endif
}



/*! \class SlRMappend implementation
 */
SlRMappend::SlRMappend(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "append";
}



void 
SlRMappend::manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( toAdd ) {
    renderQueue_->push_back(toAdd);
  }
}



/*! \class SlRMinsertAfter implementation
 */
SlRMinsertAfter::SlRMinsertAfter(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "insertAfter";
}



void 
SlRMinsertAfter::manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMinsertAfter::manipulate] Error: no name given for object to insert after.");

  std::string afterThis = parameters.at(0);
  unsigned int destAfterThis = std::stoi( parameters.at(1) );

  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMinsertAfter::manipulate] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  afterThis ) && ( (*iter)->destination_ == destAfterThis ) ) {
      renderQueue_->insert( (++iter), toAdd );
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error("[SlRMinsertAfter::manipulate] Couldn't find RenderItem " + afterThis + " to insert after.");
}



/*! \class SlRMinsertBefore implementation
 */
SlRMinsertBefore::SlRMinsertBefore(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "insertBefore";
}



void 
SlRMinsertBefore::manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMinsertBefore::manipulate] Error: no name given for object to insert before.");

  std::string beforeThis = parameters.at(0);
  unsigned int destBeforeThis = std::stoi( parameters.at(1) );

  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMinsertBefore::manipulate] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  beforeThis ) && ( (*iter)->destination_ == destBeforeThis ) ) {
      renderQueue_->insert( (iter), toAdd );
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error("[SlRMinsertBefore::manipulate] Couldn't find RenderItem " + beforeThis + " to insert before.");

}



/*! \class SlRMswapIn implementation
 */
SlRMswapIn::SlRMswapIn(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "swap";
}



void 
SlRMswapIn::manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMswapIn::manipulate] Error: no name given for object to insert before.");

  std::string toReplace = parameters.at(0);
  unsigned int destToReplace = std::stoi( parameters.at(1) );

  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMswapIn::manipulate] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  toReplace ) && ( (*iter)->destination_ == destToReplace ) ) {
      delete (*iter);
      *iter = toAdd;
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error("[SlRMswapIn::manipulate] Couldn't find RenderItem " + toReplace + " to swap with.");
}
