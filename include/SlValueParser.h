// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.h
 \brief SlValueParser helper object to translate file input into values
*/

#ifndef SLVALUEPARSER_H
#define SLVALUEPARSER_H

#include <string>
#include <vector>
#include <queue>


class SlFormulaItem;

/*! \class SlValueParser
  Translate string input from configuration files to int output.
 */
class SlValueParser
{
 public:
  /*! #screen_width_ and screen_height_ are set in the constructor.
   */
  SlValueParser(int width, int height);
  /*! Default constructor
   */
  SlValueParser(){}
  /*! Default destructor
   */
  ~SlValueParser(){}
  /*! Assignment.
   */
  SlValueParser& operator=(const SlValueParser& rhs);
  /*! Copy constructor.
   */
  SlValueParser(const SlValueParser& toCopy);
  
  /*! Returns the double value for a string. Understands SCREEN_WIDTH and SCREEN_HEIGHT.
    \throws std::runtime_error if the string can't be converted to double.
   */
  void doubleFromString(const std::string& value, double& i);
  /*! Calculates the results of a formula (only + and - implemented).
    \throws std::invalid_argument if the formula can't be calculated (unknown operator).
   */
  void parseFormula(const std::vector<std::string>& stringValues, unsigned int& i, double& value);
  /*! Takes a vector of strings, converts the strings to ints and returns them as an array.
    \throws std::invalid_argument if the size of stringValues is less than length, i.e. to few parameters given. 
   */
  void stringsToDoubles(const std::vector<std::string>& stringValues, double* values, unsigned int length );
  /*! Takes a vector of strings, converts the strings to type T (floating point or integer types) and returns them as an array.
   */
  template<typename T>
    void stringsToNumbers(const std::vector<std::string>& stringValues, T *array, unsigned int length );
  /*! Translates strings into SlRenderOptions.
   */
  bool stringsToRenderOptions(const std::vector<std::string>& stringValues, int& options );
  /*! Window dimensions can be read but not set after construction.
   */
  int screenWidth() {return screen_width_;}
  /*! Window dimensions can be read but not set after construction.
   */
  int screenHeight() {return screen_height_;}

 private:
  /*! Screen dimensions are needed to define a texture as having the dimensions of the window.
   */
  int screen_width_;
  /*! Screen dimensions are needed to define a texture as having the dimensions of the window.
   */
  int screen_height_;

  /*! Generates the formula string from stringValues.
    \throws std::invalid_argument if the end of the formula can't be found.
   */
  std::string assembleFormula(const std::vector<std::string>& stringValues, unsigned int& i);
  /*! Uses the outputQueue from shuntFormula() to calculate the result of the equation.
    \throws std::runtime_error if the items in the provided queue do not match the expected Reverse Polish notation or if an unknown operator is encountered.
   */
  double calculateFormula(std::queue<SlFormulaItem>& outputQueue);
  /*! Extracts the next item from the given string, and cuts the corresponding part from the input string.
   */
  SlFormulaItem getNextItem(std::string& formula);
  /*! Parses equation from assembleFormula() in infix notation and sorts it into Reverse Polish notation.
   */
  std::queue<SlFormulaItem> shuntFormula(std::string& formula);

};

/*! Include template implementation so the compiler can generate the required functions.
 */
#include "SlValueParser.hpp"

#endif /* SLVALUEPARSER_H */
