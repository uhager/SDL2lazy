// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderItem.cc
  \brief Implementation of SlRenderItem class for SlRenderItem's renderQueue entries.
*/

#include <iostream>
#include <string>

#include "SDL2/SDL.h"

#include "SlRenderItem.h"


SlRenderItem::SlRenderItem()
{
  initialize(nullptr, 0 );
}


SlRenderItem::SlRenderItem(std::shared_ptr<SlSprite> pSprite, unsigned int dest)
{
  initialize(pSprite, dest);
}


SlRenderItem::~SlRenderItem()
{
#ifdef DEBUG
  std::cout << "[SlRenderItem::~SlRenderItem] Deleting item for sprite " << sprite_->name()  << std::endl;
#endif
  sprite_ = nullptr;
}



SlRenderItem::SlRenderItem(const SlRenderItem& toCopy)
{
  destination_ = toCopy.destination_;
  sprite_ = toCopy.sprite_;
  renderMe_ = false ;
}



SlRenderItem&
SlRenderItem::operator=(const SlRenderItem& rhs)
{
  sprite_ = rhs.sprite_;
  destination_ = rhs.destination_;
  renderMe_ = false ;
  return *this;
}



void
SlRenderItem::initialize(const std::shared_ptr<SlSprite> pSprite, unsigned int dest)
{
  destination_ = dest;
  sprite_ = pSprite;
  renderMe_ = true;
}



bool
SlRenderItem::is_inside(const int& x, const int& y)
{
  return sprite_->is_inside(x, y, destination_);
}



