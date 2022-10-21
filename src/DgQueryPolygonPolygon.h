//@group Math/Geometry/Query

#ifndef DGQUERYPOLYGONPOLYGON_H
#define DGQUERYPOLYGONPOLYGON_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPolygon.h"
#include "DgQueryPointPolygon.h"
#include "DgQuerySegmentSegment.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;
    };

    Result operator()(Polygon2<Real> const &, Polygon2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2PolygonPolygon = Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::operator()
    (Polygon2<Real>  const &poly_a, Polygon2<Real> const &poly_b)
  {
    Result result;
    for (auto ea = poly_a.cEdgesBegin(); ea != poly_a.cEdgesEnd(); ea++)
    {
      Segment2<Real> sa = *(ea);
      for (auto eb = poly_b.cEdgesBegin(); eb != poly_b.cEdgesEnd(); eb++)
      {
        Segment2<Real> sb = *(eb);

        TI2SegmentSegment<Real> query;
        TI2SegmentSegment<Real>::Result tempResult = query(sa, sb);

        if (tempResult.code != QueryCode::NotIntersecting)
        {
          result.code = Dg::QueryCode::Intersecting;
          goto epilogue;
        }
      }
    }

    TI2PointPolygon<Real> query;
    TI2PointPolygon<Real>::Result tempResult;
      
    tempResult = query(*poly_b.cPointsBegin(), poly_a);
    if (tempResult.code != Dg::QueryCode::CompletelyOutside)
    {
      result.code = Dg::QueryCode::B_in_A;
      goto epilogue;
    }
    
    tempResult = query(*poly_a.cPointsBegin(), poly_b);
    if (tempResult.code != Dg::QueryCode::CompletelyOutside)
    {
      result.code = Dg::QueryCode::A_in_B;
      goto epilogue;
    }

    result.code = Dg::QueryCode::NotIntersecting;
  epilogue:
    return result;
  }
}

#endif