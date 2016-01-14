// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.h
 \brief SlValueParser helper object to translate file input into values
*/

#ifndef SLVALUEPARSER_H
#define SLVALUEPARSER_H

#include <string>
#include <vector>

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
   */
  bool doubleFromString(const std::string& value, double& i);
  /*! Calculates the results of a formula (only + and - implemented).
   */
  bool parseFormula(const std::vector<std::string>& stringValues, unsigned int& i, double& value);
  /*! Takes a vector of strings, converts the strings to ints and returns them as an array.
   */
  bool stringsToDoubles(const std::vector<std::string>& stringValues, double *values, unsigned int length );
  /*! Takes a vector of strings, converts the strings to type T (floating point or integer types) and returns them as an array.
   */
  template<typename T>
    bool stringsToNumbers(const std::vector<std::string>& stringValues, T *array, unsigned int length );
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

};

/*! Include template implementation so the compiler can generate the required functions.
 */
#include "SlValueParser.hpp"

#endif /* SLVALUEPARSER_H */
