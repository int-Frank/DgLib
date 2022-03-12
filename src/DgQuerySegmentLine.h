//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYSEGMENTLINE_H
#define DGQUERYSEGMENTLINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgSegment.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>
  {
  public:

    struct Result
    {
      Real ul; // Distance from the line origin to closest point to the line segment
      Real us; // Distance from the line segment origin to closest point to the line

      Vector<Real, R> cpl; // Closest point on line to the line segment
      Vector<Real, R> cps; // Closest point on line segment to the line

      QueryCode code; // Success, Parallel.
    };

    Result operator()(Segment<Real, R> const &, Line<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2SegmentLine = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Line2<Real>>;

  template<typename Real>
  using CP3SegmentLine = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Line3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>::operator()
    (Segment<Real, R> const & a_seg, Line<Real, R> const & a_line)
  {
    Result result;

    Vector<Real, R> os(a_seg.Origin());
    Vector<Real, R> ol(a_line.Origin());
    Vector<Real, R> ds(a_seg.Vect());
    Vector<Real, R> dl(a_line.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(os - ol);
    Real a = Dot(ds, ds);
    Real b = Dot(ds, dl);
    Real c = Dot(ds, w0);
    Real d = Dot(dl, w0);
    Real denom = a - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if (Dg::IsZero(denom))
    {
      result.us = static_cast<Real>(0);
      result.ul = d;

      if (IsZero(a))
        result.code = QueryCode::Success;
      else
        result.code = QueryCode::Parallel;
    }
    else
    {
      result.code = QueryCode::Success;

      // clamp result.uls within [0,1]
      Real sn = b*d - c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0))
      {
        result.us = static_cast<Real>(0);
        result.ul = d;
      }
      // clamp result.uls to 1
      else if (sn > denom)
      {
        result.us = static_cast<Real>(1);
        result.ul = (d + b);
      }
      else
      {
        result.us = sn / denom;
        result.ul = (a*d - b*c) / denom;
      }
    }

    result.cps = os + result.us*ds;
    result.cpl = ol + result.ul*dl;
    return result;
  }
}

#endif