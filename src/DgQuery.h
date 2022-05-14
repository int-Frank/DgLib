//@group Math/Geometry/Query

#ifndef DGQUERY_H
#define DGQUERY_H

namespace Dg
{
  enum class QueryCode
  {
    Fail = 0,
    Success,
    Parallel,
    Overlapping,
    Intersecting,
    NotIntersecting,
    CompletelyInside,
    CompletelyOutside,
    OnBoundary,
    A_in_B,
    B_in_A,
  };

  enum class QueryType
  {
    TestForIntersection,
    ClosestPoint,
    FindIntersection,
    ClosestPointApproach
  };

  template <QueryType qType, typename Real, int R, typename Type0, typename Type1>
  class Query
  {
  public:

    struct Result { };
    Result operator()(Type0 const & a_primitive0, Type1 const & a_primitive1);
  };
}

#endif