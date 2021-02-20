//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYRAYPLANE_H
#define DGQUERYRAYPLANE_H

#include "DgQuery.h"
#include "DgRay.h"
#include "DgPlane.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 3, Ray<Real, 3>,  Plane<Real>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Ray3<Real> const &, Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a plane.
  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 3, Ray<Real, 3>, Plane<Real>>
  {
  public:

    struct Result
    {
      Vector3<Real> point;  // Point of intersectRay<Real>ion. Set to origin if line is parallel to plane.
      Real u;               // Distance from the line origin to the point of intersection. Set to 0 if line is parallel to the plane.
      QueryCode code;       // Intersecting, Overlapping, NoIntersecting
    };

    Result operator()(Ray3<Real> const &, Plane<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI3RayPlane = Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Plane<Real>>;

  template<typename Real>
  using FI3RayPlane = Query<QueryType::FindIntersection, Real, 3, Ray3<Real>, Plane<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Plane<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Plane<Real>>::operator()
    (Ray3<Real> const & a_ray, Plane<Real> const & a_plane)
  {
    Result result;

    Vector3<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector3<Real> ro(a_ray.Origin());
    Vector3<Real> rd(a_ray.Direction());

    Real denom = Dot(pn, rd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if ray is on the plane
      if (Dg::IsZero(a_plane.Distance(ro)))
        result.isIntersecting = true;
      else
        result.isIntersecting = false;
    }
    else
    {
      Real u = (-(Dot(ro, pn) + po) / denom);

      //ray points away from plane
      if (u < static_cast<Real>(0))
        result.isIntersecting = false;
      else
        result.isIntersecting = true;
    }

    return result;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 3, Ray<Real, 3>, Plane<Real>>::Result
    Query<QueryType::FindIntersection, Real, 3, Ray<Real, 3>, Plane<Real>>::operator()
    (Ray3<Real> const & a_ray, Plane<Real> const & a_plane)
  {
    Result result;

    Vector3<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector3<Real> ro(a_ray.Origin());
    Vector3<Real> rd(a_ray.Direction());

    Real denom = Dot(pn, rd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0);
      result.point = ro;

      //check if ray is on the plane
      if (Dg::IsZero(a_plane.Distance(ro)))
        result.code = QueryCode::Overlapping;
      else
        result.code = QueryCode::NotIntersecting;
    }
    else
    {
      result.u = (-(Dot(ro, pn) + po) / denom);

      //ray points away from plane
      if (result.u < static_cast<Real>(0))
      {
        result.u = static_cast<Real>(0);
        result.point = ro;
        result.code = QueryCode::NotIntersecting;
      }
      else
      {
        result.point = ro + result.u * rd;
        result.code = QueryCode::Intersecting;
      }
    }

    return result;
  }
}

#endif