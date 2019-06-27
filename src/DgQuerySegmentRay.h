//! @file DgQuerySegmentRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYSEGMENTRAY_H
#define DGQUERYSEGMENTRAY_H

#include "../query/DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgSegment_generic.h"
#include "DgRay_generic.h"
#include "DgQueryPointRay.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line Segment, Ray
    template <typename Real, int R>
    class CPQuery<Real, R, 
                  Segment_generic<Real, R>, 
                  Ray_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the ray origin to closest point to the line segment
        Real ur;

        //! Distance from the line segment origin to closest point to the ray
        Real us;

        //! Closest point on ray to the line segment
        Vector_generic<Real, R> cpr;

        //! Closest point on line segment to the ray
        Vector_generic<Real, R> cps;

        //! Return code. Codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, R> const &, Ray_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Segment_generic<Real, R>, Ray_generic<Real, R>>::Result
      CPQuery<Real, R, Segment_generic<Real, R>, Ray_generic<Real, R>>::operator()
      (Segment_generic<Real, R> const & a_seg, Ray_generic<Real, R> const & a_ray)
    {
      Result result;
      result.code = QueryCode::QC_Success;

      Vector_generic<Real, R> ds(a_seg.Direction());
      Vector_generic<Real, R> os(a_seg.Origin());
      Vector_generic<Real, R> or (a_ray.Origin());
      Vector_generic<Real, R> dr(a_ray.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(os - or );
      Real a = ds.Dot(ds);
      Real b = ds.Dot(dr);
      Real c = ds.Dot(w0);
      Real d = dr.Dot(w0);
      Real denom = a - b*b;

      Real sn, sd, tn, td;

      // if denom is zero, try finding closest point on segment1 to origin0
      if (Dg::IsZero(denom))
      {
        if (IsZero(a))
        {
          CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>> query;
          CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>>::Result res = query(os, a_ray);
          result.ur = res.u;
          result.us = static_cast<Real>(0);
          result.cpr = res.cp;
          result.cps = os;
          return result;
        }

        // clamp result.uls to 0
        sd = td = static_cast<Real>(1.0);
        sn = static_cast<Real>(0.0);
        tn = d;

        //Does the live segment overlap the ray?
        Vector_generic<Real, R> w1((os + ds) - or );
        if (!(d < static_cast<Real>(0.0) &&
          w1.Dot(dr) < static_cast<Real>(0.0)))
        {
          result.code = QueryCode::QC_Overlapping;
        }
      }
      else
      {
        // clamp result.uls within [0,1]
        sd = td = denom;
        sn = b*d - c;
        tn = a*d - b*c;

        // clamp result.uls to 0
        if (sn < static_cast<Real>(0.0))
        {
          sn = static_cast<Real>(0.0);
          tn = d;
          td = static_cast<Real>(1.0);
        }
        // clamp result.uls to 1
        else if (sn > sd)
        {
          sn = sd;
          tn = d + b;
          td = static_cast<Real>(1.0);
        }
      }

      // clamp result.ur within [0,+inf]
      // clamp result.ur to 0
      if (tn < static_cast<Real>(0.0))
      {
        result.ur = static_cast<Real>(0.0);
        // clamp result.uls to 0
        if (-c < static_cast<Real>(0.0))
        {
          result.us = static_cast<Real>(0.0);
        }
        // clamp result.uls to 1
        else if (-c > a)
        {
          result.us = static_cast<Real>(1.0);
        }
        else
        {
          result.us = -c / a;
        }
      }
      else
      {
        result.ur = tn / td;
        result.us = sn / sd;
      }

      result.cps = os + result.us*ds;
      result.cpr = or +result.ur*dr;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif