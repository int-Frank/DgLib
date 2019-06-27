//! @file DgQueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTRAY_H
#define DGQUERYPOINTRAY_H

#include "DgCPQuery.h"
#include "DgVector_generic.h"
#include "DgRay_generic.h"
#include "../query/DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Ray.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Ray_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from ray origin to closest point to point
        Real u;

        //! Closest point on the ray to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &, Ray_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Ray_generic<Real, R> const & a_ray)
    {
      Result result;

      Vector_generic<Real, R> w = a_point - a_ray.Origin();
      result.u = w.Dot(a_ray.Direction());

      if (result.u < static_cast<Real>(0.0))
      {
        result.u = static_cast<Real>(0.0);
      }

      result.cp = a_ray.Origin() + result.u * a_ray.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif