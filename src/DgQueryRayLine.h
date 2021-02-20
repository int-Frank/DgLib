//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYRAYLINE_H
#define DGQUERYRAYLINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgRay.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>
  {
  public:

    struct Result
    {
      Real ul; // Distance from the line origin to closest point to the ray
      Real ur; // Distance from the ray origin to closest point to the line

      Vector<Real, R> cpl; // Closest point on line to the ray
      Vector<Real, R> cpr; // Closest point on ray to the line

      QueryCode code; // Success, Parallel
    };

    Result operator()(Ray<Real, R> const &, Line<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2RayLine = Query<QueryType::ClosestPoint, Real, 2, Ray2<Real>, Line2<Real>>;

  template<typename Real>
  using CP3RayLine = Query<QueryType::ClosestPoint, Real, 3, Ray3<Real>, Line3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------
  
  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>::operator()
    (Ray<Real, R> const & a_ray, Line<Real, R> const & a_line)
  {
    Vector<Real, R> or (a_ray.Origin());
    Vector<Real, R> ol(a_line.Origin());
    Vector<Real, R> dr(a_ray.Direction());
    Vector<Real, R> dl(a_line.Direction());

    //compute intermediate parameters
    Vector<Real, R> diff(or -ol);
    Real a = Dot(dr, dl);
    Real b = Dot(dl, diff);
    Real c = static_cast<Real>(1) - a * a;

    Result result;
    result.code = QueryCode::Success;

    //if c is zero, try finding closest point on the ray to the line origin
    if (Dg::IsZero(c))
    {
      //compute closest points
      result.ur = static_cast<Real>(0);
      result.ul = b;
      result.code = QueryCode::Parallel;
    }
    else
    {
      // clamp ur within [0,+inf]
      Real d = Dot(dr, diff);
      Real sn = a * b - d;

      // clamp ur to 0
      if (sn < static_cast<Real>(0))
      {
        result.ur = static_cast<Real>(0);
        result.ul = b;
      }
      else
      {
        Real denom = static_cast<Real>(1) / c;
        result.ur = sn * denom;
        result.ul = (b - a*d) * denom;
      }
    }

    result.cpr = or +result.ur*dr;
    result.cpl = ol + result.ul*dl;
    return result;
  }
}

#endif