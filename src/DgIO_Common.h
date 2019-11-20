#ifndef DGIO_COMMON_H
#define DGIO_COMMON_H

#include <stdint.h>
#include <type_traits>
#include "DgTypes.h"

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
      ErrorCode::Type error;
      myInt           value;
    };
  }
}

#endif