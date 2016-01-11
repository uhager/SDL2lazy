// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.cc

  SlValueParser implementation
*/

#include <iostream>
#include <algorithm>

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
SlValueParser::parseFormula(const std::vector<std::string>& stringValues, unsigned int& i, int& value)
{
  bool parsed = false;
  std::vector<int> numbers;
  std::vector<std::string> operators;
  operators.push_back("+");   //! <- The first number is always added to the result. 
  std::string formula;
  formula = stringValues.at(i).substr(1); //! <- already determined position 0 is '"'
  ++i;
  
  while ( ( formula.back() != '\"' ) && ( i < stringValues.size() ) ){
    formula += stringValues.at(i);
    ++i;
  }
  if ( formula.back() != '\"' ) {
#ifdef DEBUG
    std::cerr << "[SlManager::parseFormula] Error: couldn't find end of formula." << std::endl;
#endif
    return parsed;
  }

  formula.pop_back();    //! <- Remove trailing '"'
  --i;                   //! <- This is where stringsToInts will continue
  auto iter = std::remove_if(formula.begin(), formula.end(), ::isspace);
  formula.erase( iter, formula.end() );

#ifdef DEBUG
  std::cout << "[SlValueParser::parseFormula] formula " << formula << std::endl;
#endif

  
  decltype( formula.find_first_of("+") ) pos = 0;
  while ( pos != std::string::npos ) {
    pos = formula.find_first_of("+-");
    std::string number = formula.substr(0, pos);
    numbers.push_back(0);
    valueFromString( number, numbers.back() );
    if ( pos != std::string::npos ) {
      operators.push_back( formula.substr(pos, 1) ); 
      formula = formula.substr(pos+1);
    }
  }

  if (operators.size() != numbers.size()) {
#ifdef DEBUG
      std::cerr << "[SlManager::parseFormula] Error: Wrong number of operators to combine numbers." << std::endl;
#endif
      return parsed;
  }
  value = 0;
  for ( decltype(numbers.size()) j = 0 ; j < numbers.size() ; ++j ) {
    if ( operators.at(j) == "+" )
      value += numbers.at(j);
    else if  ( operators.at(j) == "-" )
      value -= numbers.at(j);
    else {
#ifdef DEBUG
      std::cerr << "[SlManager::parseFormula] Error: Unknown operator " << operators.at(j) << std::endl;
#endif
    }

  }
  return true;
}



bool
SlValueParser::stringsToInts(const std::vector<std::string>& stringValues, int* values, unsigned int valueSize )
{
    bool validValues = false;
  
  if ( stringValues.size() < valueSize ) {
#ifdef DEBUG
    std::cerr << "[SlManager::determineColors] Need " << valueSize << " values, found " << stringValues.size() << std::endl;
#endif
    return validValues;
  }

  *values = 0;
  for ( unsigned int i = 0 ; i != stringValues.size() ; ++i) {
    if ( stringValues.at(i) == "default" ) {
      (*values) |= SL_RENDER_DEFAULT;
    }
    else if ( stringValues.at(i) == "alpha" ) {
      (*values) |= SL_RENDER_ALPHAMOD;
    }
    else if ( stringValues.at(i) == "colour" ||  stringValues.at(i) == "color" ) {
      (*values) |= SL_RENDER_COLORMOD;
    }
    else if ( stringValues.at(i)[0] == '\"' ) {
      parseFormula(stringValues, i, *values);
      ++values;
    }
    else {
      bool check = valueFromString( stringValues.at(i), *values ) ;
      if (check) ++values;
    }
  }
  
  return true;
}



bool
SlValueParser::valueFromString(const std::string& value, int& i)
{
  bool isInt = false;
  if ( value == "SCREEN_WIDTH" ) {
    i = screen_width_;
  }
  else if ( value == "SCREEN_HEIGHT" ) {
    i = screen_height_;
  }
  else {
    if (isdigit(value[0])) {
      try {
	i = std::stoi( value );
      }
      catch (std::invalid_argument) {
	return isInt;
      }
    }
    else
      return isInt; 
  }
    
  return true;
}
