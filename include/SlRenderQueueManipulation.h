// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderQueueManipulation.h
  \brief SlRenderQueueManipulation class; base class for changing the render queue.
*/


#ifndef SLRENDERQUEUEMANIPULATION_H
#define SLRENDERQUEUEMANIPULATION_H


#include <string>
#include <vector>
#include <memory>

#include "SlManipulation.h"


class SlRenderItem;
class SlSpriteManager;
class SlSprite;

class SlRenderQueueManipulation : public SlManipulation
{
 public:
  /*! Use this constructor to set the required pointers. 
   */
  SlRenderQueueManipulation(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  /*! Default destructor.
   */
  virtual ~SlRenderQueueManipulation();

  /*! Creates new SlRenderItem for the specified sprite. \n
   */ 
  SlRenderItem* createRenderItem(const std::string& name, unsigned int destination);
  /*! The actual render queue manipulation, implemented in the derived classes.
   */
  virtual void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters);
  /*! Moves sprite with name toMoveName at destination destToMove before or after sprite with name targetName at destination targetDest.
    beforeOrAfter is 0 for before, 1 for after. Larger (smaller) values will result in the target item begin inserted further upstream (downstream) of the target. 
    (Handle with care, currently no test for valid iterators beyond +1...)   */
  void moveInRenderQueue(const std::string& toMoveName, const std::string& targetName, unsigned int destToMove, unsigned int targetDest, int beforeOrAfter);

 protected:
  /*! Holds the items to be rendered. The front of the queue is rendered first (background) the last element is rendered last (foreground).
   */
  std::vector<SlRenderItem*>* renderQueue_;


};


/*! \class SlRMappend
  Append new SlRenderItem to render queue.
 */
class SlRMappend : public SlRenderQueueManipulation
{
 public:
  SlRMappend(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMinsertAfter
  Insert new render item after the given one.
 */
class SlRMinsertAfter : public SlRenderQueueManipulation
{
 public:
  SlRMinsertAfter(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMinsertBefore
  Insert new render item in render queue before the given one.
 */
class SlRMinsertBefore : public SlRenderQueueManipulation
{
 public:
  SlRMinsertBefore(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMmoveAfter
  Insert new render item in render queue before the given one.
 */
class SlRMmoveAfter : public SlRenderQueueManipulation
{
 public:
  SlRMmoveAfter(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMmoveBefore
  Insert new render item in render queue before the given one.
 */
class SlRMmoveBefore : public SlRenderQueueManipulation
{
 public:
  SlRMmoveBefore(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMswapIn
  Swap new render item in place of other item in render queue.
 */
class SlRMswapIn : public SlRenderQueueManipulation
{
 public:
  SlRMswapIn(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMtoggleOnOff
  Toggles rendering of an item in the #renderQueue_ on and off. 
  Behaviour determined by int parameter: -1 to toggle, 0 to turn off, 1 to turn on.
 */
class SlRMtoggleOnOff : public SlRenderQueueManipulation
{
 public:
  SlRMtoggleOnOff(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMswapAt
  Swap new render item in place of other item in render queue.
 */
class SlRMswapAt : public SlRenderQueueManipulation
{
 public:
  SlRMswapAt(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMactivate
  Swap new render item in place of other item in render queue.
 */
class SlRMactivate : public SlRenderQueueManipulation
{
 public:
  SlRMactivate(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMdeactivate
  Swap new render item in place of other item in render queue.
 */
class SlRMdeactivate : public SlRenderQueueManipulation
{
 public:
  SlRMdeactivate(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMactivateIfInside
  Swap new render item in place of other item in render queue.
 */
class SlRMactivateIfInside : public SlRenderQueueManipulation
{
 public:
  SlRMactivateIfInside(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMdeactivateIfInside
  Swap new render item in place of other item in render queue.
 */
class SlRMdeactivateIfInside : public SlRenderQueueManipulation
{
 public:
  SlRMdeactivateIfInside(SlSpriteManager* smngr, SlValueParser* valPars, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, unsigned int destination, const std::vector<std::string>& parameters) override;
};




#endif   /* SLRENDERQUEUEMANIPULATION_H */
