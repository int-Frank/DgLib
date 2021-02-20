//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYPOINTRAY_H
#define DGQUERYPOINTRAY_H

#include "DgVector.h"
#include "DgRay.h"
#include "DgQuery.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>>
  {
  public:

    //! Query result data
    struct Result
    {
      Real u;             // Distance from ray origin to closest point to point
      Vector<Real, R> cp; // Closest point on the ray to the point.
    };

    Result operator()(Vector<Real, R> const &, Ray<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2PointRay = Query<QueryType::ClosestPoint, Real, 2, Vector2<Real>, Ray2<Real>>;

  template<typename Real>
  using CP3PointRay = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Ray3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------
  
  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>>::operator()
    (Vector<Real, R> const & a_point, Ray<Real, R> const & a_ray)
  {
    Result result;

    Vector<Real, R> w = a_point - a_ray.Origin();
    result.u = Dot(w, a_ray.Direction());

    if (result.u < static_cast<Real>(0))
      result.u = static_cast<Real>(0);

    result.cp = a_ray.Origin() + result.u * a_ray.Direction();
    return result;
  }
}

#endif