// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlFormulaItem.h
 \brief SlFormulaItem helper object to translate file input into values
*/

#ifndef SLFORMULAITEM_H
#define SLFORMULAITEM_H

#include <string>

/*! \class SlFormulaItem.
*/
class SlFormulaItem
{
 public:
  /*! Default constructor.
   */
  SlFormulaItem(){}
  /*! Constructor for operators.
   */
  SlFormulaItem(char c);
  /*! Constructor for numbers.
   */
  SlFormulaItem(double value) ;
  /*! Default constructor, nothing to see here.
   */
  ~SlFormulaItem(){}
  /*! Addition is only possible if both are numbers. #data will be summed.
    \throws std::runtime_error when one of the summed items is not a number.
   */
  SlFormulaItem operator+(const SlFormulaItem& rhs);
  /*! Addition is only possible if both are numbers. #data will be summed.
    \throws std::runtime_error when one of the summed items is not a number.
   */
  SlFormulaItem operator-(const SlFormulaItem& rhs);
  /*! Addition is only possible if both are numbers. #data will be summed.
    \throws std::runtime_error when one of the summed items is not a number.
   */
  SlFormulaItem operator*(const SlFormulaItem& rhs);
  /*! Addition is only possible if both are numbers. #data will be summed.
    \throws std::runtime_error when one of the summed items is not a number.
   */
  SlFormulaItem operator/(const SlFormulaItem& rhs);
  /*! Returns #what for operators and #data for numbers.
   */
  std::string print();
  /*! The operator symbol or 'n' if number.
   */
  char what;
  /*! Precedence is 2 for +, i, 3 for *, / .
   */
  int precedence;
  /*! If the item is a number, data holds the value.
   */
  double data = 0;
};


#endif  /* SLFORMULAITEM_H */
