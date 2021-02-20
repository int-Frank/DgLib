//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYPOINTSEGMENT_H
#define DGQUERYPOINTSEGMENT_H

#include "DgQuery.h"
#include "DgSegment.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>>
  {
  public:

    struct Result
    {
      Real u; // Distance from line segment origin to closest point to point
      Vector<Real, R> cp; // Closest point on the line segment to the point.
    };

    Result operator()(Vector<Real, R> const &, Segment<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2PointSegment = Query<QueryType::ClosestPoint, Real, 2, Vector2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP3PointSegment = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Segment3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>>::operator()
    (Vector<Real, R> const & a_point, Segment<Real, R> const & a_seg)
  {
    Result result;
    Vector<Real, R> w = a_point - a_seg.Origin();

    Real proj = Dot(w, a_seg.Direction());

    if (proj < static_cast<Real>(0))
    {
      result.u = static_cast<Real>(0);
    }
    else
    {
      Real vsq = a_seg.LengthSquared();

      if (proj >= vsq)
        result.u = static_cast<Real>(1);
      else
        result.u = (proj / vsq);
    }

    result.cp = a_seg.Origin() + result.u * a_seg.Direction();
    return result;
  }
}

#endif