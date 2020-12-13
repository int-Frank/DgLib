//@group Misc/io

#ifndef DGIO_COMMON_H
#define DGIO_COMMON_H

#include <stdint.h>
#include <type_traits>
#include "DgError.h"

#define ENSURE_VALID_FUNDAMENTAL_TYPE static_assert(std::is_fundamental<T>::value, "Invalid type.")

namespace Dg
{
  namespace IO
  {
    typedef uint8_t byte;
    typedef int64_t myInt;

    myInt const INVALID_VALUE = -1;

    struct ReturnType
    {
      ErrorCode error;
      myInt     value;
    };
  }
}

#endif