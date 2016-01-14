// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.hpp

  SlValueParser template implementation, included in SlValueParser.h
*/

#include <iostream>
#include <sstream>
#include <algorithm>





template<typename T> bool 
SlValueParser::stringsToNumbers(const std::vector<std::string>& stringValues, T *values, unsigned int length )
{
  double dvalues[length];
  bool validValues = stringsToNumbers(stringValues, dvalues, length) ;
  if (validValues){
    for (unsigned int i = 0; i < length; ++i ) {
      values[i] = static_cast<T>(dvalues[0]);
    }
  }
  return validValues;
}


