// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderItem.cc
  \brief Implementation of SlRenderItem class for SlRenderItem's renderQueue entries.
*/

#include <iostream>
#include <string>

#include "SlRenderItem.h"

SlRenderItem::SlRenderItem()
{
  initialize( "", nullptr, -1 );
}


SlRenderItem::SlRenderItem(std::string name, SlSprite* pSprite, int dest)
{
  initialize(name, pSprite, dest);
}


SlRenderItem::~SlRenderItem()
{
  sprite_ = nullptr;
#ifdef DEBUG
    std::cout << "[SlRenderItem::~SlRenderItem] Deleting item for sprite " << name_  << std::endl;
#endif
}



SlRenderItem::SlRenderItem(const SlRenderItem& toCopy)
{
  name_ = toCopy.name_;
  destination_ = toCopy.destination_;
  sprite_ = toCopy.sprite_;
}



void
SlRenderItem::initialize(std::string name, SlSprite* pSprite, int dest)
{
  name_ = name;
  destination_ = dest;
  sprite_ = pSprite;
}



SlRenderItem&
SlRenderItem::operator=(const SlRenderItem& rhs)
{
  sprite_ = rhs.sprite_;
  destination_ = rhs.destination_;
  name_ = rhs.name_;
  return *this;
}

