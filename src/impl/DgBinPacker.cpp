//@group Misc/impl

#include "../DgBinPacker.h"

namespace Dg
{
  namespace BinPackerCommon
  {
    Error GetError(ItemID a_id)
    {
      return static_cast<Error>(a_id & 0xF000'0000'0000'0000);
    }
  }
}