//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPlane.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      Real distance;
      Real signedDistance;
      Vector3<Real> cp;
    };

    Result operator()(Vector3<Real> const &, Plane<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3PointPlane = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>::operator()
    (Vector3<Real> const & a_point, Plane<Real> const & a_plane)
  {
    Result result;

    result.signedDistance = a_plane.SignedDistance(a_point);
    result.distance = abs(result.signedDistance);
    result.cp = a_point - result.signedDistance * a_plane.Normal();

    return result;
  }
}

#endif