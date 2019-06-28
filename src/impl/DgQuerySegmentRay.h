//Copyright (c) 2015, James M. Van Verth and Lars M. Bishop.
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//* Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//* Neither the name of Essential Mathematics for Games and Interactive
//Applications nor the names of its contributors may be used to endorse
//or promote products derived from this software without specific prior
//written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL JAMES M. VAN VERTH OR LARS M. BISHOP BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DGQUERYSEGMENTRAY_H
#define DGQUERYSEGMENTRAY_H

#include "../DgQueryCommon.h"
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