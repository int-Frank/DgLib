//! @file DgR3QuerySegmentBall.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTHYPERSPHERE_H
#define DGR3QUERYSEGMENTHYPERSPHERE_H

#include "../query/DgQueryCommon.h"
#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "DgSegment_generic.h"
#include "DgHypersphere_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a sphere.
    template<typename Real, int R>
    class TIQuery<Real, R,
                  Segment_generic<Real, R>, 
                  Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects sphere.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, R> const &, Hypersphere_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename TIQuery<Real, R, Segment_generic<Real, R>, Hypersphere_generic<Real, R>>::Result
      TIQuery<Real, R, Segment_generic<Real, R>, Hypersphere_generic<Real, R>>::operator()
      (Segment_generic<Real, R> const & a_seg, Hypersphere_generic<Real, R> const & a_sphere)
    {
      Result result;
      Real a = a_seg.Direction().LengthSquared();

      //Zero-length segment case.
      if (IsZero(a))
      {
        result.isIntersecting = !a_sphere.IsOutside(a_seg.Origin());
        return result;
      }

      Vector_generic<Real, R> w0(a_seg.Origin() - a_sphere.Center());
      Real inv_a = static_cast<Real>(1.0) / a;
      Real b = inv_a * w0.Dot(a_seg.Direction());
      Real c = inv_a * (w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius());
      Real discriminant = b * b + c;

      if (discriminant < static_cast<Real>(0.0))
      {
        result.isIntersecting = false;
        return result;
      }

      //TODO Not complete
      Real irr = sqrt(b * b - static_cast<Real>(4.0) * a * c);
      Real u0 = (-b - irr) / (static_cast<Real>(2.0) * a);
      Real u1 = (-b + irr) / (static_cast<Real>(2.0) * a);
      result.isIntersecting = (u0 > static_cast<Real>(0.0) && u0 < static_cast<Real>(1.0))
        || (u1 > static_cast<Real>(0.0) && u1 < static_cast<Real>(1.0))
        || (u0 * u1 < static_cast<Real>(0.0))
        return result;
    } //End: CPQuery::operator()
  }
}

#endif