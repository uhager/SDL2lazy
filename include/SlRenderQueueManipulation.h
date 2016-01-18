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


class SlRenderItem;
class SlSpriteManager;
class SlSprite;

class SlRenderQueueManipulation
{
 public:
  SlRenderQueueManipulation(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue);
  virtual ~SlRenderQueueManipulation();

  /*! Creates new SlRenderItem for the specified sprite. \n
   */ 
  SlRenderItem* createRenderItem(const std::string& name, unsigned int destination);
  /*! The actual render queue manipulation, implemented in the derived classes.
   */
  virtual void manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters);
  /*! Name can be read but not set. It is defined by the function that the derived class implements so that the correct derived class can be called based on a keyword.
   */
  std::string name() {return name_;}
  /*! Checks the existence of the named sprite and the validity of the destinations.
    \retval sprite of that name if name and destination valid.
    \throws std::invalid_argument if invalid name or destination.
   */
  std::shared_ptr<SlSprite> verifySprite(std::string name, unsigned int destination);


 protected:
  /*! Name of the object must be identical to the keyword used in the configuration file.
   */
  std::string name_ = "renderQueueManipulation";
  /*! The SlManager that initialized this instance. Needed to find sprites.
   */
  std::shared_ptr<SlSpriteManager> smngr_ = nullptr;
  /*! Holds the items to be rendered. The front of the queue is rendered first (background) the last element is rendered last (foreground).
   */
  std::vector<SlRenderItem*>* renderQueue_;


};


/*! \class SlRMappend
 */
class SlRMappend : public SlRenderQueueManipulation
{
 public:
  SlRMappend(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue);
  void manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters) override;
};



/*! \class SlRMinsertAfter
 */
class SlRMinsertAfter : public SlRenderQueueManipulation
{
 public:
  SlRMinsertAfter(std::shared_ptr<SlSpriteManager> smngr, std::vector<SlRenderItem*>* renderQueue);
  void manipulateQueue(const std::string& name, int destination, const std::vector<std::string>& parameters) override;
};




#endif   /* SLRENDERQUEUEMANIPULATION_H */
