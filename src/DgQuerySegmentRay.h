//@group Math/Geometry/Query

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

#include "DgQuery.h"
#include "DgSegment.h"
#include "DgRay.h"
#include "DgQueryPointRay.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>
  {
  public:

    struct Result
    {
      Real ur; // Distance from the ray origin to closest point to the line segment
      Real us; // Distance from the line segment origin to closest point to the ray

      Vector<Real, R> cpr; // Closest point on ray to the line segment
      Vector<Real, R> cps; // Closest point on line segment to the ray

      QueryCode code; // Success, Parallel
    };

    Result operator()(Segment<Real, R> const &, Ray<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2SegmentRay = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Ray2<Real>>;

  template<typename Real>
  using CP3SegmentRay = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Ray3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>::operator()
    (Segment<Real, R> const & a_seg, Ray<Real, R> const & a_ray)
  {
    Result result;
    result.code = QueryCode::Success;

    Vector<Real, R> ds(a_seg.Direction());
    Vector<Real, R> os(a_seg.Origin());
    Vector<Real, R> or (a_ray.Origin());
    Vector<Real, R> dr(a_ray.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(os - or);
    Real a = Dot(ds, ds);
    Real b = Dot(ds, dr);
    Real c = Dot(ds, w0);
    Real d = Dot(dr, w0);
    Real denom = a - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      if (IsZero(a))
      {
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>> query;
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>>::Result res = query(os, a_ray);
        result.ur = res.u;
        result.us = static_cast<Real>(0);
        result.cpr = res.cp;
        result.cps = os;
        return result;
      }

      // clamp result.uls to 0
      sd = td = static_cast<Real>(1);
      sn = static_cast<Real>(0);
      tn = d;

      //Does the live segment overlap the ray?
      Vector<Real, R> w1((os + ds) - or);
      if (!(d < static_cast<Real>(0) && Dot(w1, dr) < static_cast<Real>(0)))
        result.code = QueryCode::Overlapping;
    }
    else
    {
      // clamp result.uls within [0,1]
      sd = td = denom;
      sn = b*d - c;
      tn = a*d - b*c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0))
      {
        sn = static_cast<Real>(0);
        tn = d;
        td = static_cast<Real>(1);
      }
      // clamp result.uls to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = d + b;
        td = static_cast<Real>(1);
      }
    }

    // clamp result.ur within [0,+inf]
    // clamp result.ur to 0
    if (tn < static_cast<Real>(0))
    {
      result.ur = static_cast<Real>(0);
     
      if (-c < static_cast<Real>(0))  // clamp result.uls to 0
        result.us = static_cast<Real>(0);
      else if (-c > a)                // clamp result.uls to 1
        result.us = static_cast<Real>(1);
      else
        result.us = -c / a;
    }
    else
    {
      result.ur = tn / td;
      result.us = sn / sd;
    }

    result.cps = os + result.us*ds;
    result.cpr = or +result.ur*dr;
    return result;
  }
}

#endif