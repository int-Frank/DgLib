//@group Math/Geometry/Query

#ifndef DGQUERYSEGMENTPOLYGON_H
#define DGQUERYSEGMENTPOLYGON_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPolygon.h"
#include "DgQuerySegmentSegment.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Segment<Real, 2>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;
    };

    Result operator()(Segment<Real, 2> const &, Polygon2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2SegmentPolygon = Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Polygon2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Segment<Real, 2>, Polygon2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Segment<Real, 2>, Polygon2<Real>>::operator()
    (Segment<Real, 2> const &a_seg, Polygon2<Real> const &a_polygon)
  {
    Result result;
    result.code = QueryCode::NotIntersecting;

    for (auto it = a_polygon.cEdgesBegin(); it != a_polygon.cEdgesEnd(); it++)
    {
      TI2SegmentSegment<Real> query;
      TI2SegmentSegment<Real>::Result tempResult = query(*it, a_seg);

      if (tempResult.code != QueryCode::NotIntersecting)
      {
        result.code = Dg::QueryCode::Intersecting;
        break;
      }
    }
    return result;
  }
}

#endif