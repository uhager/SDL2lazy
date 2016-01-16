// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlFormulaItem.cc

  SlFormulaItem implementation
*/

/*! \class SlFormulaItem
 */

#include <stdexcept>
#include <sstream>

#include "SlFormulaItem.h"


SlFormulaItem::SlFormulaItem(char c) 
{
    switch (c) {
    case '+': case '-':
      precedence = 2;
      break;
    case '*': case '/':
      precedence = 3;
      break;
    default:
      throw std::invalid_argument("Unknown operator " + c);
    }
    what = c;
}



SlFormulaItem::SlFormulaItem(double value) 
{
  data = value;
  what = 'n';
  precedence = 0;
}



SlFormulaItem 
SlFormulaItem::operator+(const SlFormulaItem& rhs)
{
  if ( precedence != 0 || rhs.precedence != 0 ) 
    throw std::runtime_error("Cannot add SlFormulaItems that aren't numbers.");
  SlFormulaItem result = *this;
  result.data += rhs.data;
  return result;
}



SlFormulaItem 
SlFormulaItem::operator-(const SlFormulaItem& rhs)
{
  if ( precedence != 0 || rhs.precedence != 0 ) 
    throw std::runtime_error("Cannot add SlFormulaItems that aren't numbers.");
  SlFormulaItem result = *this;
  result.data -= rhs.data;
  return result;
}



SlFormulaItem 
SlFormulaItem::operator*(const SlFormulaItem& rhs)
{
  if ( precedence != 0 || rhs.precedence != 0 ) 
    throw std::runtime_error("Cannot add SlFormulaItems that aren't numbers.");
  SlFormulaItem result = *this;
  result.data *= rhs.data;
  return result;
}



SlFormulaItem 
SlFormulaItem::operator/(const SlFormulaItem& rhs)
{
  if ( precedence != 0 || rhs.precedence != 0 ) 
    throw std::runtime_error("Cannot add SlFormulaItems that aren't numbers.");
  SlFormulaItem result = *this;
  result.data /= rhs.data;
  return result;
}



std::string
SlFormulaItem::print()
{
  std::string result;
  std::ostringstream stream;
  stream << " ";
  if ( what == 'n' )
    stream << data ; 
  else 
    stream << what;
  stream << " ";
  result = stream.str();
  return result;
}
