// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlEventHandler.cc

  SlEventHandler, SlEventObject, SlEventAction implementation
*/

#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "SlManipulation.h"

#include "SlEventHandler.h"


/*! \class SlEventAction
 */
void
SlEventAction::act()
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
SlEventObject::trigger()
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

  auto iter = eventActions_.find( keyword );
  if ( iter == eventActions_.end() ) return;  //!< undefined input is ignored.
  iter->second.trigger();
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
	std::string keyword = "is_" + key;
	auto manip = manipulations_.find( whatToDo );
	if ( manip == manipulations_.end() )
	  throw std::invalid_argument( "[SlEventHandler::parseEvent] Error: invalid manipulation " + whatToDo ); 

	SlEventObject& obj = eventActions_[keyword];
	obj.addAction(spritename, destination, manip->second, parameters);
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

