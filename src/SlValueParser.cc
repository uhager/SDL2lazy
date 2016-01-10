// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.cc

  SlValueParser implementation
*/

#include <iostream>

#include "SlRenderOptions.h" 
#include "SlValueParser.h"


SlValueParser::SlValueParser(int width, int height)
  : screen_width_(width)
  , screen_height_(height)
{
}



SlValueParser::SlValueParser(const SlValueParser& toCopy)
{
  screen_width_ = toCopy.screen_width_;
  screen_height_ = toCopy.screen_height_;
}



SlValueParser&
SlValueParser::operator=(const SlValueParser& rhs)
{
  screen_width_ = rhs.screen_width_;
  screen_height_ = rhs.screen_height_;
  return *this;
}



bool
SlValueParser::stringsToInts(const std::vector<std::string>& stringValues, int *values, unsigned int valueSize )
{
    bool validValues = false;
  
  if ( stringValues.size() < valueSize ) {
#ifdef DEBUG
    std::cerr << "[SlManager::determineColors] Need 4 values, found " << stringValues.size() << std::endl;
#endif
    return validValues;
  }

  *values = 0;
  
  for ( unsigned int i = 0 ; i != stringValues.size() ; ++i){
    if ( stringValues.at(i) == "SCREEN_WIDTH" ) {
      (*values) = screen_width_;
      ++values;
    }
    else if ( stringValues.at(i) == "SCREEN_HEIGHT" ) {
      (*values) = screen_height_;
      ++values;
    }
    else if ( stringValues.at(i) == "default" ) {
      (*values) |= SL_RENDER_DEFAULT;
      }
    else if ( stringValues.at(i) == "alpha" ) {
      (*values) |= SL_RENDER_ALPHAMOD;
      }
    else if ( stringValues.at(i) == "colour" ||  stringValues.at(i) == "color" ) {
      (*values) |= SL_RENDER_COLORMOD;
      }
    else {
      try {
	(*values) = std::stoi( stringValues.at(i) );
	++values;
      }
      catch (std::invalid_argument) {
	return validValues;
      }
    }
  }

  validValues = true;
  return validValues;
}
