//@group Math/impl

#include "../DgGraph.h"

namespace Dg
{
  namespace Graph
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

    uint64_t GetUndirectedEdgeID(uint32_t a, uint32_t b)
    {
      if (a > b)
        return (((uint64_t)a) << 32) | (uint64_t)b;
      return (((uint64_t)b) << 32) | (uint64_t)a;
    }

    uint64_t GetDirectedEdgeID(uint32_t a, uint32_t b)
    {
      return (((uint64_t)a) << 32) | ((uint64_t)b);
    }
  }
}