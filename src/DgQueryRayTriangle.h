//@group Math/Geometry/Query

#ifndef DGQUERYRAYTRIANGLE_H
#define DGQUERYRAYTRIANGLE_H

#include "DgQuery.h"
#include "DgRay.h"
#include "DgTriangle.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Triangle3<Real>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
      Real u; // Distance along the ray to the intersection
    };

    Result operator()(Ray3<Real> const &, Triangle3<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI3RayTriangle = Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Triangle3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Triangle3<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 3, Ray3<Real>, Triangle3<Real>>::operator()
    (Ray3<Real> const & a_ray, Triangle3<Real> const & a_tri)
  {
    Result result;
    result.isIntersecting = false;

    //Find vectors for two edges sharing V1
    Vector3<Real> e1(a_tri.P1() - a_tri.P0());
    Vector3<Real> e2(a_tri.P2() - a_tri.P0());

    //Begin calculating determinant - also used to calculate u parameter
    Vector3<Real> P(Cross(a_ray.Direction(), e2));

    //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
    Real det = Dot(e1, P);
    if (IsZero(det))
      return result;

    Real inv_det = static_cast<Real>(1) / det;
    Vector3<Real> T(a_ray.Origin() - a_tri.P0());

    //Calculate u parameter and test bound
    Real u = Dot(T, P) * inv_det;

    //The intersection lies outside of the triangle
    if (u < static_cast<Real>(0) || u > static_cast<Real>(1)) 
      return result;

    //Prepare to test v parameter
    Vector3<Real> Q(Cross(T, e1));

    //Calculate V parameter and test bound
    Real v = Dot(a_ray.Direction(), Q) * inv_det;

    //The intersection lies outside of the triangle
    if ((v < static_cast<Real>(0)) || (u + v  > static_cast<Real>(1))) 
      return result;

    Real t = Dot(e2, Q) * inv_det;

    if (t > static_cast<Real>(0)) 
    { 
      //ray intersection
      result.u = t;
      result.isIntersecting = true;
    }

    return result;
  }
}

#endif