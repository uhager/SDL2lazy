// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.cc

  SlValueParser implementation
*/

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdexcept>
	
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



void
SlValueParser::parseFormula(const std::vector<std::string>& stringValues, unsigned int& i, double& value)
{
  std::vector<double> numbers;
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
    throw std::invalid_argument("Couldn't find end of formula.");
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
    doubleFromString( number, numbers.back() );
    if ( pos != std::string::npos ) {
      operators.push_back( formula.substr(pos, 1) ); 
      formula = formula.substr(pos+1);
    }
  }

  if (operators.size() != numbers.size()) {
#ifdef DEBUG
      std::cerr << "[SlManager::parseFormula] Error: Wrong number of operators to combine numbers." << std::endl;
#endif
      throw std::invalid_argument("Formula has wrong number of values / operators.");
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
      throw std::invalid_argument("Unknown operator: " + operators.at(j) );   
    }

  }
}



void
SlValueParser::stringsToDoubles(const std::vector<std::string>& stringValues, double* values, unsigned int length )
{
  if ( stringValues.size() < length ) {
#ifdef DEBUG
    std::cerr << "[SlManager::stringsToDoubles] Need " << length << " values, found " << stringValues.size() << std::endl;
#endif
    throw std::invalid_argument("Too few values.");
  }

  for ( unsigned int i = 0 ; i != stringValues.size() ; ++i) {
    if ( stringValues.at(i)[0] == '\"' ) {
      parseFormula(stringValues, i, *values);
    }
    else {
      doubleFromString( stringValues.at(i), *values ) ;
    }
    ++values;
  }
}



bool
SlValueParser::stringsToRenderOptions(const std::vector<std::string>& stringValues, int& options )
{
  options = 0;
  for ( unsigned int i = 0 ; i != stringValues.size() ; ++i) {
    if ( stringValues.at(i) == "default" ) {
      (options) |= SL_RENDER_DEFAULT;
    }
    else if ( stringValues.at(i) == "alpha" ) {
      (options) |= SL_RENDER_ALPHAMOD;
    }
    else if ( stringValues.at(i) == "colour" ||  stringValues.at(i) == "color" ) {
      (options) |= SL_RENDER_COLORMOD;
    }
    else {
#ifdef DEBUG
    std::cerr << "[SlManager::stringsToRenderOptions] Unknown token " << stringValues.at(i) << std::endl;
#endif
    return false;
    }
  }
      return true;
}




void
SlValueParser::doubleFromString(const std::string& svalue, double& dvalue)
{
  if ( svalue == "SCREEN_WIDTH" ) {
    dvalue = screen_width_;
  }
  else if ( svalue == "SCREEN_HEIGHT" ) {
    dvalue = screen_height_;
  }
  else {
    std::istringstream is(svalue);
    if ( !(is >> dvalue) ) 
      throw std::runtime_error("Invalid conversion to double of string " + svalue);
  }
}
