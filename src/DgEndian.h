#ifndef DGENDIAN_H
#define DGENDIAN_H

#include <stdint.h>
#include "DgIO_Common.h"

namespace Dg
{ 
  //We won't worry about anything exotic
  enum class Endian
  {
    Big,
    Little,
    Unknown
  };

  //Determine Endianness of this machine during runtime.
  Endian Endianness();

  template<typename T>
  T ReverseBytes(T const & a_val)
  {
    T result(static_cast<T>(0));
    for (size_t i = 0; i < sizeof(T); i++)
    {
      reinterpret_cast<uint8_t*>(&result)[sizeof(T) - i - 1] =
        reinterpret_cast<uint8_t const *>(&a_val)[i];
    }
    return result;
  }

  class EndianConverter
  {
  public:

    //The default converter will not convert, but simply return the value;
    EndianConverter();

    void SetConversion(Endian const input, Endian const output);

    template<typename T>
    T Convert(T const a_val) const;

  private:
  
    bool m_reverse;
  };

  template<typename T>
  T EndianConverter::Convert(T const a_val) const
  {
    ENSURE_VALID_FUNDAMENTAL_TYPE;
    if (m_reverse)
      return ReverseBytes<T>(a_val);
    return a_val;
  }
}

#endif