//! @file DgR3QueryRayTriangle.h
//!
//! @author: Wikipedia
//! @date 29/05/2016

#ifndef DGQUERYRAYTRIANGLE_H
#define DGQUERYRAYTRIANGLE_H

#include "DgTIQuery.h"
#include "../query/DgQueryCommon.h"
#include "../DgR3Ray.h"
#include "../DgR3Triangle.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a plane.
    template <typename Real>
    class TIQuery<Real, 3,
                  R3::Ray<Real>,
                  R3::Triangle<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects plane.
        bool isIntersecting;

        //! Distance along the ray to the intersection
        Real u;
      };

      //! Perform query.
      Result operator()(R3::Ray<Real> const &, R3::Triangle<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Ray<Real>, R3::Triangle<Real>>::Result
      TIQuery<Real, 3, R3::Ray<Real>, R3::Triangle<Real>>::operator()
      (R3::Ray<Real> const & a_ray, R3::Triangle<Real> const & a_tri)
    {
      Result result;
      result.isIntersecting = false;

      //Find vectors for two edges sharing V1
      Vector_generic<Real, 3> e1(a_tri.P1() - a_tri.P0());
      Vector_generic<Real, 3> e2(a_tri.P2() - a_tri.P0());

      //Begin calculating determinant - also used to calculate u parameter
      Vector_generic<Real, 3> P(a_ray.Direction().Cross(e2));

      //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
      Real det = e1.Dot(P);
      if (IsZero(det)) return result;

      Real inv_det = static_cast<Real>(1) / det;
      Vector_generic<Real, 3> T(a_ray.Origin() - a_tri.P0());

      //Calculate u parameter and test bound
      Real u = T.Dot(P) * inv_det;

      //The intersection lies outside of the triangle
      if (u < static_cast<Real>(0) || u > static_cast<Real>(1)) return result;

      //Prepare to test v parameter
      Vector_generic<Real, 3> Q(T.Cross(e1));

      //Calculate V parameter and test bound
      Real v = a_ray.Direction().Dot(Q) * inv_det;

      //The intersection lies outside of the triangle
      if (v < static_cast<Real>(0) || u + v  > static_cast<Real>(1)) return result;

      Real t = e2.Dot(Q) * inv_det;

      if (t > static_cast<Real>(0)) 
      { 
        //ray intersection
        result.u = t;
        result.isIntersecting = true;
      }

      return result;
    } //End: TIQuery::operator()
  }
}

#endif