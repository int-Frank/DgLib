#ifndef DGIO_COMMON_H
#define DGIO_COMMON_H

#include <stdint.h>
#include "DgTypes.h"

#define ENSURE_VALID_FUNDAMENTAL_TYPE static_assert(IO::IsValidType<T>::value, "Invalid type.")

namespace Dg
{
  namespace IO
  {
    typedef uint8_t byte;
    typedef int64_t myInt;

    myInt const INVALID_VALUE = -1;

    struct ReturnType
    {
      operator myInt(){return value;}
      operator ::Dg::ErrorCode(){return error;}

      ::Dg::ErrorCode error;
      myInt           value;
    };

    template<typename T> 
    struct IsValidType {static bool const value = false;};

    template<> struct IsValidType<char>     {static bool const value = true;};
    template<> struct IsValidType<int8_t>   {static bool const value = true;};
    template<> struct IsValidType<int16_t>  {static bool const value = true;};
    template<> struct IsValidType<int32_t>  {static bool const value = true;};
    template<> struct IsValidType<int64_t>  {static bool const value = true;};
    template<> struct IsValidType<uint8_t>  {static bool const value = true;};
    template<> struct IsValidType<uint16_t> {static bool const value = true;};
    template<> struct IsValidType<uint32_t> {static bool const value = true;};
    template<> struct IsValidType<uint64_t> {static bool const value = true;};
    template<> struct IsValidType<float>    {static bool const value = true;};
    template<> struct IsValidType<double>   {static bool const value = true;};
  }
}

#endif