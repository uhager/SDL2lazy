// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderQueueManipulation.cc

  SlRenderQueueManipulation and derived classes implementation
*/


#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <algorithm>

#include "SlSpriteManager.h"
#include "SlRenderItem.h"
#include "SlValueParser.h"

#include "SlRenderQueueManipulation.h"


SlRenderQueueManipulation::SlRenderQueueManipulation(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlManipulation(smngr, valPars)
{
  renderQueue_ = renderQueue;
  name_ = "renderQueueManipulation";
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::SlRenderQueueManipulation] Created " << name_ << std::endl;
#endif
}



SlRenderQueueManipulation::~SlRenderQueueManipulation()
{
  renderQueue_ = nullptr;
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::~SlRenderQueueManipulation] deleting " << name_ << std::endl;
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
SlRenderQueueManipulation::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlRenderQueueManipulation::manipulate] " << name_ << std::endl;
#endif
}



void
SlRenderQueueManipulation::moveInRenderQueue(const std::string& toMoveName, const std::string& targetName, unsigned int destToMove, unsigned int targetDest, int beforeOrAfter)
{
  
  auto iter1 = std::find_if( renderQueue_->begin(), renderQueue_->end(),
			     [toMoveName, destToMove](const SlRenderItem* item) -> bool { return ( (item)->sprite_->name() == toMoveName && (item)->destination_ == destToMove ); } );
  if ( iter1 == renderQueue_->end() ) 
    throw std::invalid_argument( "[SlManager::moveInRenderQueueAfter] Couldn't find item to move " + toMoveName );

  auto iter2 = std::find_if( renderQueue_->begin(), renderQueue_->end(),
			     [targetName, targetDest](const SlRenderItem* item) -> bool { return ( (item)->sprite_->name() == targetName && (item)->destination_ == targetDest ); } );
  if ( iter2 == renderQueue_->end() )
    throw std::invalid_argument( "[SlManager::moveInRenderQueueAfter] Couldn't find item to move after " + targetName );


  /* Cheat sheet
    std::rotate( iter2, iter1, iter1+1 );   // insert before, iter1 > iter 2
    std::rotate( iter2+1, iter1, iter1+1 );  // insert after, iter1 > iter 2
    std::rotate( iter1, iter1+1, iter2 );   // insert before, iter1 < iter 2
    std::rotate( iter1, iter1+1, iter2+1 );  // insert after, iter1 < iter 2
  */

  if ( iter1 == iter2 ) 
    return ;
  else if ( iter1 > iter2 )
    std::rotate( iter2 + beforeOrAfter, iter1, iter1+1 ); 
  else if ( iter1 < iter2 ) 
    std::rotate( iter1, iter1+1, iter2 + beforeOrAfter );
}





/*! \class SlRMappend implementation
 */
SlRMappend::SlRMappend(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "append";
#ifdef DEBUG
  std::cout << "[SlRMappend::SlRMappend] Created " << name_ << std::endl;
#endif
}



void 
SlRMappend::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
#ifdef DEBUG
  std::cout << "[SlRMappend::manipulate]" << name << std::endl;
#endif
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
SlRMinsertAfter::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
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
SlRMinsertBefore::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
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



/*! \class SlRMmoveBefore implementation
 */
SlRMmoveBefore::SlRMmoveBefore(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "moveBefore";
}



void 
SlRMmoveBefore::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
      throw std::invalid_argument("[SlRMmoveBefore::manipulate] Error: Wrong number of parameters.");

  std::string beforeThis = parameters.at(0);
  unsigned int destBeforeThis = std::stoi( parameters.at(1) );
  moveInRenderQueue(name, beforeThis, destination, destBeforeThis, 0);
}



/*! \class SlRMmoveAfter implementation
 */
SlRMmoveAfter::SlRMmoveAfter(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "moveAfter";
}



void 
SlRMmoveAfter::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2 )
    throw std::invalid_argument("[SlRMmoveAfter::manipulate] Error: Wrong number of parameters: Need 2 found " + std::to_string( parameters.size() ) );

  std::string beforeThis = parameters.at(0);
  unsigned int destBeforeThis = std::stoi( parameters.at(1) );
  moveInRenderQueue(name, beforeThis, destination, destBeforeThis, 1);
}



/*! \class SlRMswapIn implementation
 */
SlRMswapIn::SlRMswapIn(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "swap";
}



void 
SlRMswapIn::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
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



/*! \class SlRMtoggleOnOff implementation
 */
SlRMtoggleOnOff::SlRMtoggleOnOff(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "toggleOnOff";
}



void
SlRMtoggleOnOff::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  int onOrOff = -1;
  if ( parameters.size() == 1 ) {
    try {
      onOrOff = std::stoi( parameters.at(0) );
    }
    catch (const std::exception& expt) {
      std::cerr << "[SlRMtoggleOnOff::manipulate] Error: " << expt.what() << std::endl ;
    }
  }
  
 if ( (onOrOff < -1) || (onOrOff > 1) )
   throw std::invalid_argument( "[SlRMtoggleOnOff::manipulate] Invalid toggle option " + parameters.at(0) );

 std::vector<SlRenderItem*>::iterator iter;
  for ( iter = renderQueue_->begin(); iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  name ) && ( (*iter)->destination_ == destination ) ) {
      switch (onOrOff){
      case -1:
	(*iter)->renderMe_ = !((*iter)->renderMe_) ;
	return;
      case 0:	
	(*iter)->renderMe_ = false ;
	return;
      case 1:
	(*iter)->renderMe_ = true ;
	return;
      }
    }
  }

  if ( iter == renderQueue_->end() ) 
    throw std::invalid_argument( "[SlRMtoggleOnOff::manipulate] Couldn't find sprite to toggle: " + name );
}




/*! \class SlRMswapAt implementation
 */
SlRMswapAt::SlRMswapAt(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "swapAt";
}



void 
SlRMswapAt::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  if (parameters.size() != 1 )
      throw std::invalid_argument("[SlRMswapAt::manipulate] Error: No position given to swap in object " + name);

  unsigned int position = std::stoi( parameters.at(0) );
  if ( position < 0 || position >= renderQueue_->size() )
    throw std::invalid_argument("[SlRMswapAt::manipulate] Error: Invalid position " + std::to_string(position) + " to swap in object " + name);
    
  SlRenderItem* toAdd = nullptr;
  toAdd = createRenderItem(name, destination);
  if ( !toAdd ) 
    throw std::runtime_error("[SlRMswapAt::manipulate] Couldn't create render item for " + name);

  std::vector<SlRenderItem*>::iterator iter = renderQueue_->begin() + position;
  delete (*iter);
  renderQueue_->erase( iter );
  renderQueue_->insert( iter, toAdd );

}



/*! \class SlRMactivate implementation
 */
SlRMactivate::SlRMactivate(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "activate";
}



void
SlRMactivate::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  auto iter = renderQueue_->begin();
  for ( ; iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  name ) && ( (*iter)->destination_ == destination ) ) {
      (*iter)->isActive = true;
      return;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error( "[SlRMactivate::manipulate] Error: Couldn't find SlRenderItem for " + name ) ;
}



/*! \class SlRMactivateIfInside implementation
 */
SlRMactivateIfInside::SlRMactivateIfInside(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "activateIfInside";
}



void
SlRMactivateIfInside::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{

  auto iter = renderQueue_->begin();
  for ( ; iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  name ) && ( (*iter)->destination_ == destination ) ) {
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error( "[SlRMactivateIfInside::manipulate] Error: Couldn't find SlRenderItem for " + name ) ;

  int coord[2];
  valParser->stringsToNumbers<int>( parameters, coord, 2 );
  if ( (*iter)->is_inside( coord[0], coord[1] )) 
    (*iter)->isActive = true;
}



/*! \class SlRMdeactivate implementation
 */
SlRMdeactivate::SlRMdeactivate(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "deactivate";
}



void
SlRMdeactivate::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{
  auto iter = renderQueue_->begin();
  for ( ; iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  name ) && ( (*iter)->destination_ == destination ) ) {
      (*iter)->isActive = false;
      return;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error( "[SlRMdeactivate::manipulate] Error: Couldn't find SlRenderItem for " + name ) ;
}




/*! \class SlRMdeactivateIfInside implementation
 */
SlRMdeactivateIfInside::SlRMdeactivateIfInside(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue)
  : SlRenderQueueManipulation( smngr, valPars, renderQueue )
{
  name_ = "deactivateIfInside";
}



void
SlRMdeactivateIfInside::manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters)
{

  auto iter = renderQueue_->begin();
  for ( ; iter != renderQueue_->end() ; ++iter) {
    if ( ( (*iter)->sprite_->name() ==  name ) && ( (*iter)->destination_ == destination ) ) {
      break;
    }
  }

  if ( iter == renderQueue_->end() )
    throw std::runtime_error( "[SlRMactivateIfInside::manipulate] Error: Couldn't find SlRenderItem for " + name ) ;

  int coord[2];
  valParser->stringsToNumbers<int>( parameters, coord, 2 );
  if ( (*iter)->is_inside( coord[0], coord[1] ) )
    (*iter)->isActive = false;
}
