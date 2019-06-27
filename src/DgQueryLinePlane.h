//! @file DgR3QueryLinePlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINEPLANE_H
#define DGQUERYLINEPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "../query/DgQueryCommon.h"
#include "../DgR3Line.h"
#include "../DgR3Plane.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a plane.
    template <typename Real>
    class TIQuery<Real, 3,
                  R3::Line<Real>,
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
      Result operator()(R3::Line<Real> const &, R3::Plane<Real> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line and a plane.
    template <typename Real>
    class FIQuery<Real, 3,
                  R3::Line<Real>,
                  R3::Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersectLine<Real>ion. Set to origin if line is parallel to plane.
        Vector_generic<Real, 3> point;

        //! Distance from the line origin to the point of intersection. Set to 0 if line is parallel to the plane.
        Real u;

        //! Return code. Codes include:
        //! Intersecting, Overlapping, NoIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(R3::Line<Real> const &, R3::Plane<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Line<Real>, R3::Plane<Real>>::Result
      TIQuery<Real, 3, R3::Line<Real>, R3::Plane<Real>>::operator()
      (R3::Line<Real> const & a_line, R3::Plane<Real> const & a_plane)
    {
      Result result;
      result.isIntersecting = !Dg::IsZero(a_plane.NormalDot(a_line.Direction())) ||
                               Dg::IsZero(a_plane.SignedDistance(a_line.Origin()));
      return result;
    } //End: CPQuery::operator()


      //--------------------------------------------------------------------------------
      //	@	CPQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, 3, R3::Line<Real>, R3::Plane<Real>>::Result
      FIQuery<Real, 3, R3::Line<Real>, R3::Plane<Real>>::operator()
      (R3::Line<Real> const & a_line, R3::Plane<Real> const & a_plane)
    {
      Vector_generic<Real, 3> pn(a_plane.Normal());
      Real                    po(a_plane.Offset());
      Vector_generic<Real, 3> lo(a_line.Origin());
      Vector_generic<Real, 3> ld(a_line.Direction());

      Real denom = pn.Dot(ld);
      Result result;

      //check if line is parallel to plane
      if (Dg::IsZero(denom))
      {
        result.u = static_cast<Real>(0.0);
        result.point = lo;

        //check if line is on the plane
        result.code = Dg::IsZero(a_plane.SignedDistance(lo)) ? QueryCode::QC_Overlapping : QueryCode::QC_NotIntersecting;
      }
      else
      {
        result.u = -(lo.Dot(pn) + po) / denom;
        result.point = lo + result.u * ld;
        result.code = QueryCode::QC_Intersecting;
      }

      return result;
    } //End: CPQuery::operator()
  }
}

#endif