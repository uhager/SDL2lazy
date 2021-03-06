// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlValueParser.hpp

  SlValueParser template implementation, included in SlValueParser.h
*/





template<typename T> void
SlValueParser::stringsToNumbers(const std::vector<std::string>& stringValues, T *values, unsigned int length )
{
  double dvalues[length];
  stringsToDoubles(stringValues, dvalues, length) ;
    for (unsigned int i = 0; i < length; ++i ) {
      values[i] = static_cast<T>(dvalues[i]);
    }
}


