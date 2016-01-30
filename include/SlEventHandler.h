// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlEventHandler.h
  \brief SlEventHandler class 
*/

#ifndef SLEVENTHANDLER_H
#define  SLEVENTHANDLER_H

#include <map>
#include <vector>
#include <string>

#include <SDL2/SDL.h>


class SlManipulation;
class SlRenderItem;

/*! \class SlEventAction
  This describes a single action that will be taken as a result of an input. 
 */
class SlEventAction
{
 public:
  SlEventAction(){};
  ~SlEventAction();
  /*! Calls the SlManipulation
   */
  void act(int mouse_x = -1, int mouse_y = -1);

  /*! The manipulation object that handles the action.
   */
  SlManipulation* manipulation = nullptr;
  /*! The sprite on which the manipulation will act. 
   */
  std::string name = "";
  /*! The sprite's destination.
    Warning: have to think of something if one SlEventAction changes the destination number.
   */
  int destination = 0;
  /*! The parameters with which the SlManipulation is called.
   */
  std::vector<std::string> parameters;
};


/*! \class SlEventObject
  Object associated with a specific user input. This object's SlEventActions will be performed when the corresponding input event is received.
 */
class SlEventObject
{
 public:
  /*! Default constructor
   */ 
  SlEventObject();
  /*! Default destructor
   */ 
  ~SlEventObject();
  /*! Delete copy constructor. Each EventObject is associated with a specific input.
   */
  SlEventObject(const SlEventObject&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlEventObject& operator=(const SlEventObject&) = delete;
  /*! Adds a new SlEventAction to #actions_.
   */
  void addAction(std::string name, int destination, SlManipulation* manip, std::vector<std::string> params );
  /*! Triggers the SlEventAction::act() for all objects in #actions_.
   */
  void trigger(int mouse_x = -1, int mouse_y = -1);
 private:
  /*! The SlEventActions that will be performed when this object is triggered.
   */
  std::vector<SlEventAction> actions_ ;
};


/*! \class SlEventHandler
  Takes a SDL_Event and calls the assigned SlEventObject.
 */
class SlEventHandler
{
 public:
  /*! Default constructor
   */ 
  SlEventHandler();
  /*! Default destructor 
   */
  ~SlEventHandler();
  /*! Delete copy constructor. EventHandler is unique to SlManager and its sprites.
   */
  SlEventHandler(const SlEventHandler&) = delete;
  /*! Deleted, same reason as copy constructor.
   */
  SlEventHandler& operator=(const SlEventHandler&) = delete;
  /*! Adds the given SlManipulations to #manipulations_.
   */
  void addManipulations(const std::map<std::string, SlManipulation*>& manip);
  /*! Returns specified SlManipulation from #manipulations_ based on whatToDo keyword.
    \throws std::invalid_argument if not found.
   */
  SlManipulation* getManipulation(const std::string& whatToDo);
  /*! The actual event handling, checking what SDL_Event was registered. Limited implimentation.
    \retval 1 if event was quit.
    \retval 0 otherwise.
   */
  int handleEvent(const SDL_Event& event);
  /*! Parse configuration file entry.
    Pattern: key   what      spritename     sprite_destination      parameters
   */ 
  void parseEvent(std::ifstream& input);
  /*! Check for events. 
    \retval 1 if event was quit.
    \retval 0 otherwise.
   */
  int pollEvent();

  
 private:
  /*! The string is the keyword for the event handling.
   */
  std::map<std::string, SlEventObject> eventActions_;
  /*! Contains pointers to all SlSpriteManipulations and SlRenderManipulations
   */
  std::map<std::string, SlManipulation*> manipulations_;
  /*! The event used for polling.
   */
  SDL_Event event_;

};


#endif  /*  SLEVENTHANDLER_H */
