//! @file DgR3QueryRayPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYRAYPLANE_H
#define DGQUERYRAYPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "../query/DgQueryCommon.h"
#include "../DgR3Ray.h"
#include "../DgR3Plane.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a plane.
    template <typename Real>
    class TIQuery<Real, 3,
                  R3::Ray<Real>,
                  R3::Plane<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects plane.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(R3::Ray<Real> const &, R3::Plane<Real> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line and a plane.
    template <typename Real>
    class FIQuery<Real, 3,
                  R3::Ray<Real>,
                  R3::Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersectRay<Real>ion. Set to origin if line is parallel to plane.
        Vector_generic<Real, 3> point;

        //! Distance from the line origin to the point of intersection. Set to 0 if line is parallel to the plane.
        Real u;

        //! Return code. Codes include:
        //! Intersecting, Overlapping, NoIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(R3::Ray<Real> const &, R3::Plane<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Ray<Real>, R3::Plane<Real>>::Result
      TIQuery<Real, 3, R3::Ray<Real>, R3::Plane<Real>>::operator()
      (R3::Ray<Real> const & a_ray, R3::Plane<Real> const & a_plane)
    {
      Result result;

      Vector_generic<Real, 3> pn(a_plane.Normal());
      Real                    po(a_plane.Offset());
      Vector_generic<Real, 3> ro(a_ray.Origin());
      Vector_generic<Real, 3> rd(a_ray.Direction());

      Real denom = pn.Dot(rd);

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        //check if ray is on the plane
        if (Dg::IsZero(a_plane.Distance(ro)))
        {
          result.isIntersecting = true;
        }
        else
        {
          result.isIntersecting = false;
        }
      }
      else
      {
        Real u = (-(ro.Dot(pn) + po) / denom);

        //ray points away from plane
        if (u < static_cast<Real>(0.0))
        {
          result.isIntersecting = false;
        }
        else
        {
          result.isIntersecting = true;
        }
      }

      return result;
    } //End: TIQuery::operator()


      //--------------------------------------------------------------------------------
      //	@	CPQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, 3, R3::Ray<Real>, R3::Plane<Real>>::Result
      FIQuery<Real, 3, R3::Ray<Real>, R3::Plane<Real>>::operator()
      (R3::Ray<Real> const & a_ray, R3::Plane<Real> const & a_plane)
    {
      Result result;

      Vector_generic<Real, 3> pn(a_plane.Normal());
      Real                    po(a_plane.Offset());
      Vector_generic<Real, 3> ro(a_ray.Origin());
      Vector_generic<Real, 3> rd(a_ray.Direction());

      Real denom = pn.Dot(rd);

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        result.u = static_cast<Real>(0.0);
        result.point = ro;

        //check if ray is on the plane
        if (Dg::IsZero(a_plane.Distance(ro)))
        {
          result.code = QueryCode::QC_Overlapping;
        }
        else
        {
          result.code = QueryCode::QC_NotIntersecting;
        }
      }
      else
      {
        result.u = (-(ro.Dot(pn) + po) / denom);

        //ray points away from plane
        if (result.u < static_cast<Real>(0.0))
        {
          result.u = static_cast<Real>(0.0);
          result.point = ro;
          result.code = QueryCode::QC_NotIntersecting;
        }
        else
        {
          result.point = ro + result.u * rd;
          result.code = QueryCode::QC_Intersecting;
        }
      }

      return result;
    } //End: FIQuery::operator()
  }
}

#endif