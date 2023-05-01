//@group Math/Geometry/Query/impl

#include "../DgQueryPolygonPolygon.h"

namespace Dg
{
  namespace impl_FI2PolygonPolygon
  {
    EdgePair::EdgePair(uint64_t e0, uint64_t e1)
      : edge0(e0)
      , edge1(e1)
    {
      if (e0 > e1)
      {
        edge0 = e1;
        edge1 = e0;
      }
    }

    bool EdgePairLess(EdgePair const &p0, EdgePair const &p1)
    {
      if (p0.edge0 < p1.edge0)
        return true;
      if (p0.edge0 > p1.edge0)
        return false;
      if (p0.edge1 < p1.edge1)
        return true;
      if (p0.edge1 > p1.edge1)
        return false;
      return false; // equal
    }
  }
}