// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlEventHandler.cc

  SlEventHandler, SlEventObject, SlEventAction implementation
*/

#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "SlManipulation.h"
#include "SlRenderItem.h"

#include "SlEventHandler.h"


/*! \class SlEventAction
 */
void
SlEventAction::act(int mouse_x, int mouse_y)
{
  try {
    manipulation->manipulate( name, destination, parameters );
  }
  catch (const std::exception& expt) {
    std::cerr << expt.what() << std::endl;
  }
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
#ifdef DEBUG
  std::cout << "[SlEventObject::addAction] added manipulation " << manip->name() << " for sprite " << name << std::endl;
#endif
  SlEventAction toAdd;
  toAdd.name = name;
  toAdd.destination = destination;
  toAdd.manipulation = manip;
  toAdd.parameters = params;
  actions_.push_back( toAdd );
}




void
SlEventObject::trigger(int mouse_x, int mouse_y)
{
  for (auto action: actions_) {
    action.act();
  }
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
SlEventHandler::addManipulations(const std::map<std::string, SlManipulation*>& manip)
{
  manipulations_.insert( manip.begin(), manip.end() );
}



SlManipulation*
SlEventHandler::getManipulation(const std::string& whatToDo)
{
  auto manip = manipulations_.find( whatToDo );
  if ( manip == manipulations_.end() )
    throw std::invalid_argument( "[SlEventHandler::getManipulation] Error: invalid manipulation " + whatToDo );
  return manip->second;
}




int
SlEventHandler::handleEvent(const SDL_Event& event)
{
  std::string keyword = "";
  int mouse_x = -1, mouse_y = -1;

  if (event.type == SDL_QUIT) return 1;
  else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      SDL_GetMouseState( &mouse_x, &mouse_y );
      keyword = "is_mouse-down";
    }
  else if (event.type == SDL_MOUSEBUTTONUP) {
      SDL_GetMouseState( &mouse_x, &mouse_y );
      keyword = "is_mouse-up";
  }
  else if (event.type == SDL_MOUSEMOTION) {
    //    int relx, rely;
    SDL_GetMouseState( &mouse_x, &mouse_y );
    //   SDL_GetRelativeMouseState(&relx, &rely);
  }
  else if (event.type == SDL_KEYDOWN)
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
	case SDLK_ESCAPE:
	  return 1;
	}
    }

  auto iter = eventActions_.find( keyword );
  if ( iter != eventActions_.end() )   //!< undefined input is ignored.
    iter->second.trigger(mouse_x, mouse_y);
  
  return 0;
}



void
SlEventHandler::parseEvent(std::ifstream& input)
{
  std::string line, token;
  std::string key, whatToDo, spritename;
  int destination ;
  std::vector<std::string> parameters;
  bool endOfConfig = false;
  
  getline(input,line);
  while ( !endOfConfig && input ) {
    std::istringstream stream(line.c_str());
    stream >> token;
    if ( token[0] == '#' || token.empty() || token[0] == '\n'  ) {
      /* empty line or comment */
    }
    else if ( token == "end" ) {
      endOfConfig = true;
    }
    else {
      try {
	key = token;
	stream >> whatToDo;
	stream >> spritename;
	stream >> destination;
	while ( !stream.eof() ){
	  parameters.push_back("");
	  stream >> parameters.back();
	}
	if ( parameters.back().empty() )
	  parameters.pop_back();
	SlManipulation* manip = getManipulation( whatToDo );
	std::string keyword = "is_" + key;
	SlEventObject& obj = eventActions_[keyword];
	obj.addAction(spritename, destination, manip, parameters);
      }
      catch (const std::exception& expt ) {
	std::cerr << "[SlEventHandler::parseEvent] Error: " << expt.what() << std::endl;
      }
    }
    token.clear();
    parameters.clear();
    if ( !endOfConfig ) getline(input,line);
  }
}



int
SlEventHandler::pollEvent()
{
  int result = 0;  //!< 0 means don't quit, 1 means quit
  while (SDL_PollEvent(&event_)) {
    result = handleEvent(event_);
  }
  return result;
}
