// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlEventHandler.cc

  SlEventHandler, SlEventObject, SlEventAction implementation
*/

#include "SlManipulation.h"

#include "SlEventHandler.h"


/*! \class SlEventAction
 */
void
SlEventAction::act()
{
  manipulation->manipulate( name, destination, parameters );
}



SlEventAction::~SlEventAction()
{
  manipulation = nullptr;
}



/*! \class SlEventObject
 */
SlEventObject::SlEventObject()
{
}



SlEventObject::~SlEventObject()
{
}



void
SlEventObject::addAction(std::string name, int destination, SlManipulation* manip, std::vector<std::string> params )
{
  SlEventAction toAdd;
  toAdd.name = name;
  toAdd.destination = destination;
  toAdd.manipulation = manip;
  toAdd.parameters = params;
  actions.push_back( toAdd );
}



/*! \class SlEventHandler
 */
SlEventHandler::SlEventHandler()
{
}



SlEventHandler::~SlEventHandler()
{
}



void
SlEventHandler::addManipulations(std::map<std::string, SlManipulation*> manip)
{
  manipulations_.insert( manip.begin(), manip.end() );
}



void
SlEventHandler::handleEvent(const SDL_Event& event)
{
  std::string keyword = "";
  if (event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.sym)
	{
	case SDLK_UP:
	  keyword = "is_up";
	  break;
	case SDLK_DOWN:
	  keyword = "is_down";
	  break;
	case SDLK_LEFT:
	  keyword = "is_left";
	  break;
	case SDLK_RIGHT:
	  keyword = "is_right";
	  break;
	case SDLK_1:
	  keyword = "is_1";
	  break;
	case SDLK_2:
	  keyword = "is_2";
	  break;
	case SDLK_3:
	  keyword = "is_3";
	  break;
	case SDLK_4:
	  keyword = "is_4";
	  break;
	case SDLK_m:
	  keyword = "is_m";
	  break;
	case SDLK_n:
	  keyword = "is_n";
	  break;
	case SDLK_b:
	  keyword = "is_b";
	  break;
	case SDLK_f:
	  keyword = "is_f";
	  break;
	}
    }
  
}



void
SlEventHandler::parseEvent(std::ifstream& input)
{
}

