//@group Math/impl

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

#ifndef DGQUERYSEGMENTSEGMENT_H
#define DGQUERYSEGMENTSEGMENT_H

#include "DgQuery.h"
#include "DgQueryPointSegment.h"
#include "DgVector.h"
#include "DgSegment.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>
  {
  public:

    struct Result
    {
      Real u0; // Fraction along segment 0 to closest point to the segment 1
      Real u1; // Fraction along segment 1 to closest point to the segment 0

      Vector<Real, R> cp0; // Closest point on segment 0 to segment 1
      Vector<Real, R> cp1; // Closest point on segment 1 to segment 0

      QueryCode code; // Success, Overlapping
    };

    Result operator()(Segment<Real, R> const &, Segment<Real, R> const &);
  };

  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Segment2<Real> const &, Segment2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2SegmentSegment = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using TI2SegmentSegment = Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP3SegmentSegment = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Segment3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>::operator()
    (Segment<Real, R> const & a_seg0, Segment<Real, R> const & a_seg1)
  {
    Result result;
    result.code = QueryCode::Success;

    Vector<Real, R> o0(a_seg0.Origin());
    Vector<Real, R> o1(a_seg1.Origin());
    Vector<Real, R> d0(a_seg0.Direction());
    Vector<Real, R> d1(a_seg1.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(o0 - o1);
    Real a = Dot(d0, d0);
    Real b = Dot(d0, d1);
    Real c = Dot(d1, d1);
    Real d = Dot(d0, w0);
    Real e = Dot(d1, w0);
    Real denom = a*c - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      //Either or both segment lengths are zero
      if (IsZero(a))
      {
        if (IsZero(c))
        {
          result.u0 = static_cast<Real>(0);
          result.u1 = static_cast<Real>(0);
          result.cp0 = o0;
          result.cp1 = o1;
        }
        else
        {
          Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>> query;
          Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>>::Result qRes = query(o0, a_seg1);
          result.u0 = static_cast<Real>(0);
          result.u1 = qRes.u;
          result.cp0 = o0;
          result.cp1 = qRes.cp;
        }
        return result;
      }
      else if (IsZero(c))
      {
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>> query;
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Segment<Real, R>>::Result qRes = query(o1, a_seg0);
        result.u0 = qRes.u;
        result.u1 = static_cast<Real>(0);
        result.cp0 = qRes.cp;
        result.cp1 = o0;
        return result;
      }

      // clamp result.u0 to 0
      sd = td = c;
      sn = static_cast<Real>(0);
      tn = e;

      //Do the line segments overlap?
      Vector<Real, R> w1((o0 + d0) - o1);
      Vector<Real, R> w2(o0 - (o1 + d1));
      Vector<Real, R> w3((o0 + d0) - (o1 + d1));
      bool bse = (e < static_cast<Real>(0));
      if (!(
        bse == (Dot(w1, d1) < static_cast<Real>(0)) &&
        bse == (Dot(w2, d1) < static_cast<Real>(0)) &&
        bse == (Dot(w3, d1) < static_cast<Real>(0))
        ))
      {
        result.code = QueryCode::Overlapping;
      }
    }
    else
    {
      // clamp result.u0 within [0,1]
      sd = td = denom;
      sn = b*e - c*d;
      tn = a*e - b*d;

      // clamp result.u0 to 0
      if (sn < static_cast<Real>(0))
      {
        sn = static_cast<Real>(0);
        tn = e;
        td = c;
      }
      // clamp result.u0 to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = e + b;
        td = c;
      }
    }

    // clamp result.u1 within [0,1]
    // clamp result.u1 to 0
    if (tn < static_cast<Real>(0))
    {
      result.u1 = static_cast<Real>(0);
     
      if (-d < static_cast<Real>(0))    // clamp result.u0 to 0
        result.u0 = static_cast<Real>(0);
      else if (-d > a)                  // clamp result.u0 to 1
        result.u0 = static_cast<Real>(1);
      else
        result.u0 = -d / a;
    }
    // clamp result.u1 to 1
    else if (tn > td)
    {
      result.u1 = static_cast<Real>(1.0);
     
      if ((-d + b) < static_cast<Real>(0))  // clamp result.u0 to 0
        result.u0 = static_cast<Real>(0);
      else if ((-d + b) > a)                // clamp result.u0 to 1
        result.u0 = static_cast<Real>(1);
      else
        result.u0 = (-d + b) / a;
    }
    else
    {
      result.u1 = tn / td;
      result.u0 = sn / sd;
    }

    result.cp0 = o0 + result.u0*d0;
    result.cp1 = o1 + result.u1*d1;
    return result;
  }

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::operator()
    (Segment<Real, 2> const & a_seg0, Segment<Real, 2> const & a_seg1)
  {
    Result result;

    Vector<Real, 2> const & dir0 = a_seg0.Direction();
    Vector<Real, 2> const & dir1 = a_seg1.Direction();

    Vector<Real, 2> w = a_seg0.GetP0() - a_seg1.GetP0();
    Real denom = PerpDot(a_seg0.Direction(), a_seg1.Direction());
    Real u0_numerator = PerpDot(a_seg1.Direction(), w);
    Real u1_numerator = PerpDot(a_seg0.Direction(), w);

    if (Dg::IsZero(denom))
    {
      //Parallel
      if (!Dg::IsZero(u0_numerator))
        result.isIntersecting = false;

      //Segments lie on the same line
      else
      {
        Vector2<Real> v0 = a_seg0.Direction() / MagSq(a_seg0.Direction());
        Vector2<Real> v1 = a_seg1.Direction() / MagSq(a_seg1.Direction());
        Vector2<Real> q = a_seg1.GetP0() - a_seg1.GetP0();

        result.isIntersecting = (IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(w, v1))
          || IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(q, v1))
          || IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(-w, v0)));
      }
    }
    else
    {
      Real u0 = u0_numerator / denom;
      Real u1 = u1_numerator / denom;
      result.isIntersecting = (IsInRange(static_cast<Real>(0), static_cast<Real>(1), u0)
        && IsInRange(static_cast<Real>(0), static_cast<Real>(1), u1));
    }
    return result;
  }
}

#endif