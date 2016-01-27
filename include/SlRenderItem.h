// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderItem.h
  \brief SlRenderItem class for SlRenderItem's renderQueue entries.
*/

#ifndef SLRENDERITEM_H
#define SLRENDERITEM_H

#include <memory>
#include <string>

#include "SlSprite.h"

/*! \class SlRenderItem
Entries for SlRenderItem::renderQueue.
Objects are identified by their name and destination which correspond to the SlSprite::name_ and SlSprite::destinations_ position.\n
The pointer to the sprite is added to avoid having ro repeatedly find the sprite by name.
*/
class SlRenderItem
{
 public:
  /*! Default constructor
   */
  SlRenderItem();
  /*! Constructor sets name and destiantion and points sprite to pSprite.
   */
  SlRenderItem(std::shared_ptr<SlSprite> pSprite, unsigned int dest);

  ~SlRenderItem();
  /*! Copy constructor, #sprite_ will point to same SlSprite object 
   */
  SlRenderItem(const SlRenderItem& toCopy);
  /*! Assignment, #sprite_ will point to same SlSprite object 
   */
  SlRenderItem& operator=(const SlRenderItem& rhs);
  /*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
   */
  bool operator==( const SlRenderItem& rhs)
  {
    return ( (sprite_->name() == rhs.sprite_->name() ) &&
	     (destination_    == rhs.destination_    ) );
  }
  /*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
   */
  bool operator!=( const SlRenderItem& rhs)
  {
    return !(*this == rhs);
  }

  /*! Sets values for all data members.
   */
  void initialize(const std::shared_ptr<SlSprite> pSprite, unsigned int dest);
  /*! Checks if the provided coordinates are inside the destination of this render item.
   */
  bool is_inside(const int& x, const int& y);
  /*! The sprite that will be rendered.
   */
  std::shared_ptr<SlSprite> sprite_;
  /*! The position in SlSprite::destinations_ .
   */
  unsigned int destination_ ;
  /*! Determines whether the item will be rendered. This avoids having to delete an item and create a new one each in the same position if a sprite is toggled on and off often.
   */
  bool renderMe_;
};




#endif // SLRENDERITEM_H
