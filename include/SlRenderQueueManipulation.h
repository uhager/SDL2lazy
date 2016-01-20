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
  SlRenderQueueManipulation(SlSpriteManager* smngr, SlValueParser* valParser, std::vector<SlRenderItem*>* renderQueue);
  virtual ~SlRenderQueueManipulation();

  /*! Creates new SlRenderItem for the specified sprite. \n
   */ 
  SlRenderItem* createRenderItem(const std::string& name, unsigned int destination);
  /*! The actual render queue manipulation, implemented in the derived classes.
   */
  virtual void manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters);

 protected:
  /*! Holds the items to be rendered. The front of the queue is rendered first (background) the last element is rendered last (foreground).
   */
  std::vector<SlRenderItem*>* renderQueue_;


};


/*! \class SlRMappend
 */
class SlRMappend : public SlRenderQueueManipulation
{
 public:
  SlRMappend(SlSpriteManager* smngr, SlValueParser* valParser, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMinsertAfter
 */
class SlRMinsertAfter : public SlRenderQueueManipulation
{
 public:
  SlRMinsertAfter(SlSpriteManager* smngr, SlValueParser* valParser, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMinsertBefore
 */
class SlRMinsertBefore : public SlRenderQueueManipulation
{
 public:
  SlRMinsertBefore(SlSpriteManager* smngr, SlValueParser* valParser, std::vector<SlRenderItem*>* renderQueue);
  void manipulate(const std::string& name, int destination, const std::vector<std::string>& parameters) override;
};




#endif   /* SLRENDERQUEUEMANIPULATION_H */
