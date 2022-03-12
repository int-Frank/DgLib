//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYLINELINE_H
#define DGQUERYLINELINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgVector.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>
  {
  public:

    struct Result
    {
      Real u0;            // Distance from line 0 origin to closest point to line 1
      Real u1;            // Distance from line 1 origin to closest point to line 0

      Vector3<Real> cp0;  // Closest point on line 0 to line 1
      Vector3<Real> cp1;  // Closest point on line 1 to line 0

      QueryCode code;     // Success, Parallel
    };

    Result operator()(Line3<Real> const &, Line3<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3LineLine = Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>;

  template<typename Real>
  using CP2LineLine = Query<QueryType::ClosestPoint, Real, 2, Line2<Real>, Line2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>::operator()
    (Line3<Real> const & a_line0, Line3<Real> const & a_line1)
  {
    Result result;

    Vector3<Real> o0(a_line0.Origin());
    Vector3<Real> o1(a_line1.Origin());
    Vector3<Real> d0(a_line0.Direction());
    Vector3<Real> d1(a_line1.Direction());

    //compute intermediate parameters
    Vector3<Real> w0(o0 - o1);
    Real a = Dot(d0, d1);
    Real b = Dot(d0, w0);
    Real c = Dot(d1, w0);
    Real d = static_cast<Real>(1) - a*a;

    if (Dg::IsZero(d))
    {
      result.u0 = static_cast<Real>(0);
      result.u1 = c;
      result.code = QueryCode::Parallel;
    }
    else
    {
      result.u0 = ((a*c - b) / d);
      result.u1 = ((c - a*b) / d);
      result.code = QueryCode::Success;
    }

    result.cp0 = o0 + result.u0 * d0;
    result.cp1 = o1 + result.u1 * d1;
    return result;
  }
}

#endif