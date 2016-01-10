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
  /*! Takes a vector of strings, converts the strings to ints and returns them as an array.
   */
  bool stringsToInts(const std::vector<std::string>& stringValues, int *values, unsigned int valueSize );
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


#endif /* SLVALUEPARSER_H */
