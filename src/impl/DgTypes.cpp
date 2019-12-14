//@group Misc/impl

#include "../DgTypes.h"

namespace Dg
{
#undef ITEM
#define ITEM(x) #x,
  char const * ErrorCodeStrings[]
  {
    ERRORCODES
  };
#undef ITEM
}