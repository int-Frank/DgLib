//@group Misc/impl

#include "../DgTypes.h"

namespace Dg
{
  char const* ErrorCodeToString(ErrorCode a_code)
  {
#undef ITEM
#define ITEM(x) #x,
    static char const * s_ErrorCodeStrings[]
    {
      ERRORCODES
    };
#undef ITEM
    return s_ErrorCodeStrings[static_cast<ErrorCodeType>(a_code)];
  }

}