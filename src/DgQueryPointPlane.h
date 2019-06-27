//! @file DgQueryPointPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgCPQuery.h"
#include "DgVector_generic.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Plane.
    template <typename Real>
    class CPQuery<Real, 3, 
                  Vector_generic<Real, 3>, 
                  R3::Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance between point and line
        Real distance;

        //! Signed distance between point and line
        Real sDistance;

        //! Closest point on the plane to the point.
        Vector_generic<Real, 3> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, 3> const &, R3::Plane<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, 3, Vector_generic<Real, 3>, R3::Plane<Real>>::Result
      CPQuery<Real, 3, Vector_generic<Real, 3>, R3::Plane<Real>>::operator()
      (Vector_generic<Real, 3> const & a_point, R3::Plane<Real> const & a_plane)
    {
      Result result;

      result.sDistance = a_plane.SignedDistance(a_point);
      result.distance = abs(result.sDistance);
      result.cp = a_point - result.sDistance * a_plane.Normal();

      return result;
    } //End: CPQuery::operator()
  }
}

#endif