// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.cc

  SlValueParser implementation
*/

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <queue>

#include "SlRenderOptions.h" 
#include "SlFormulaItem.h"

#include "SlValueParser.h"


/*! \class SlValueParser
 */
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



std::string
SlValueParser::assembleFormula(const std::vector<std::string>& stringValues, unsigned int& i)
{
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
  std::cout << "[SlValueParser::assembleFormula] formula " << formula << std::endl;
#endif
  return formula;
}



double
SlValueParser::calculateFormula(std::queue<SlFormulaItem>& outputQueue)
{
  std::stack<SlFormulaItem> tempStorage; 

  for ( ; !outputQueue.empty() ; outputQueue.pop() ) {
    SlFormulaItem item = outputQueue.front();
    if ( item.what == 'n' ) {   //! <- number
      tempStorage.push(item);
    }
    else {
      if ( tempStorage.size() < 2 )   //! <- need two numbers to use operator
	throw std::runtime_error("Bad formula");
      
      SlFormulaItem operants[2] ;
      for ( int j=0 ; j<2 ; j++ ){
	operants[j] = tempStorage.top();
	tempStorage.pop();
      }
      switch ( item.what ){
      case '+':
	tempStorage.push( operants[1] + operants[0] );
	break;
      case '-':
	tempStorage.push( operants[1] - operants[0] );
	break;
      case '*':
	tempStorage.push( operants[1] * operants[0] );
	break;
      case '/':
	tempStorage.push( operants[1] / operants[0] );
	break;
      default:
	throw std::runtime_error("Unknown operator: " + item.what );
      }
    }
  }

  if ( tempStorage.size() != 1 )
    throw std::runtime_error("Bad formula.");

  return tempStorage.top().data;
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
      throw std::runtime_error("Invalid conversion to double of string: \"" + svalue + "\"");
  }
}



void
SlValueParser::parseFormula(const std::vector<std::string>& stringValues, unsigned int& i, double& value)
{
  std::string formula = assembleFormula(stringValues, i);
  std::queue<SlFormulaItem> outputQueue = shuntFormula(formula);

  value = calculateFormula( outputQueue );
}



std::queue<SlFormulaItem>
SlValueParser::shuntFormula(std::string& formula)
{
  std::queue<SlFormulaItem> outputQueue;
  std::stack<SlFormulaItem> operatorStack; 


  decltype( formula.find_first_of("+") ) pos = 0;
  while ( pos != std::string::npos ) {
    pos = formula.find_first_of("+-*/");
    std::string number = formula.substr(0, pos);
    double data;
    doubleFromString( number, data );
    outputQueue.push( SlFormulaItem(data) );
    if ( pos != std::string::npos ) {
      char op = formula.substr(pos, 1)[0];
      SlFormulaItem opItem(op);
      while ( !operatorStack.empty() && operatorStack.top().precedence > opItem.precedence ) {
	outputQueue.push( operatorStack.top() );
	operatorStack.pop();
      }
      operatorStack.push(opItem);
      formula = formula.substr(pos+1);
    }
  }
  while ( !operatorStack.empty() ) {
	outputQueue.push( operatorStack.top() );
	operatorStack.pop();
  }
    return outputQueue;
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
    //! Skip trailing whitespace at end of line otherwise causes exception.
    if ( stringValues.at(i).empty() ) continue;
    else if ( stringValues.at(i)[0] == '\"' ) {
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




