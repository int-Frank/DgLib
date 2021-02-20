//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYPOINTLINE_H
#define DGQUERYPOINTLINE_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgLine.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Line<Real, R>>
  {
  public:

    //! Query result data
    struct Result
    {
      Real u;             // Distance from line origin to closest point to point
      Vector<Real, R> cp; // Closest point on the line to the point.
    };

    Result operator()(Vector<Real, R> const &, Line<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3PointLine = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Line3<Real>>;

  template<typename Real>
  using CP2PointLine = Query<QueryType::ClosestPoint, Real, 2, Vector2<Real>, Line2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------
  
  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Line<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Line<Real, R>>::operator()
    (Vector<Real, R> const & a_point, Line<Real, R> const & a_line)
  {
    Result result;

    Vector<Real, R> w = a_point - a_line.Origin();
    result.u = Dot(w, a_line.Direction());
    result.cp = a_line.Origin() + result.u * a_line.Direction();
    return result;
  }
}

#endif