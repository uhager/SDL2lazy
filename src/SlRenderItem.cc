// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderItem.cc
  \brief Implementation of SlRenderItem class for SlRenderItem's renderQueue entries.
*/

#include <string>

#include "SlRenderItem.h"

SlRenderItem::SlRenderItem()
{
}


SlRenderItem::SlRenderItem(std::string name, SlSprite* pSprite, int dest)
  : name_(name)
  , destination_(dest)
{
  sprite_ = pSprite;
}


SlRenderItem::~SlRenderItem()
{
  sprite_ = nullptr;
}



SlRenderItem::SlRenderItem(const SlRenderItem& toCopy)
{
  name_ = toCopy.name_;
  destination_ = toCopy.destination_;
  sprite_ = toCopy.sprite_;
}



SlRenderItem&
SlRenderItem::operator=(const SlRenderItem& rhs)
{
  sprite_ = rhs.sprite_;
  destination_ = rhs.destination_;
  name_ = rhs.name_;
  return *this;
}



/*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
 */
bool operator==(const SlRenderItem& lhs, const SlRenderItem& rhs)
{
  return ( (lhs.name_        == rhs.name_       ) &&
	   (lhs.destination_ == rhs.destination_) );
}


/*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
 */
bool operator!=(const SlRenderItem& lhs, const SlRenderItem& rhs)
{
  return !(lhs == rhs);
}