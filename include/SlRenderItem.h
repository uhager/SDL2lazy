// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderItem.h
  \brief SlRenderItem class for SlRenderItem's renderQueue entries.
*/

#ifndef SLRENDERITEM_H
#define SLRENDERITEM_H

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
  SlRenderItem(std::string name, SlSprite* pSprite, unsigned int dest);

  ~SlRenderItem();
  /*! Copy constructor, #sprite will point to same SlSprite object 
   */
  SlRenderItem(const SlRenderItem& toCopy);
  /*! Assignment, #sprite will point to same SlSprite object 
   */
  SlRenderItem& operator=(const SlRenderItem& rhs);
  /*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
   */
  bool operator==( const SlRenderItem& rhs)
  {
    return ( (name_        == rhs.name_       ) &&
	     (destination_ == rhs.destination_) );
  }
  /*! SlRenderItems are equal when they have the same name and destination, i.e. the same sprite will be plotted in the same position.
   */
  bool operator!=( const SlRenderItem& rhs)
  {
    return !(*this == rhs);
  }

  /*! Sets values for all data members.
   */
  void initialize(std::string name, SlSprite* pSprite, unsigned int dest);
  
  std::string name_;
  SlSprite* sprite_;
  /*! The position in SlSprite::destinations_ .
   */
  unsigned int destination_ ;
};




#endif // SLRENDERITEM_H
