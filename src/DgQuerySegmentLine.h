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

#ifndef DGQUERYSEGMENTLINE_H
#define DGQUERYSEGMENTLINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgSegment.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>
  {
  public:

    struct Result
    {
      Real ul; // Distance from the line origin to closest point to the line segment
      Real us; // Distance from the line segment origin to closest point to the line

      Vector<Real, R> cpl; // Closest point on line to the line segment
      Vector<Real, R> cps; // Closest point on line segment to the line

      QueryCode code; // Success, Parallel.
    };

    Result operator()(Segment<Real, R> const &, Line<Real, R> const &);
  };

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Line2<Real>>
  {
  public:

	  struct Result
	  {
		  Real us; // Distance from the line origin to closest point to the line segment
		  Real ul; // Distance from the line segment origin to closest point to the line

		  Vector2<Real> p; // Intersection point

		  QueryCode code; // NotIntersecting, Intersecting, Overlapping
	  };

	  Result operator()(Segment2<Real> const &, Line2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using FI2SegmentLine = Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Line2<Real>>;

  template<typename Real>
  using CP2SegmentLine = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Line2<Real>>;

  template<typename Real>
  using CP3SegmentLine = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Line3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Line<Real, R>>::operator()
    (Segment<Real, R> const & a_seg, Line<Real, R> const & a_line)
  {
    Result result;

    Vector<Real, R> os(a_seg.Origin());
    Vector<Real, R> ol(a_line.Origin());
    Vector<Real, R> ds(a_seg.Direction());
    Vector<Real, R> dl(a_line.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(os - ol);
    Real a = Dot(ds, ds);
    Real b = Dot(ds, dl);
    Real c = Dot(ds, w0);
    Real d = Dot(dl, w0);
    Real denom = a - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if (Dg::IsZero(denom))
    {
      result.us = static_cast<Real>(0);
      result.ul = d;

      if (IsZero(a))
        result.code = QueryCode::Success;
      else
        result.code = QueryCode::Parallel;
    }
    else
    {
      result.code = QueryCode::Success;

      // clamp result.uls within [0,1]
      Real sn = b*d - c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0))
      {
        result.us = static_cast<Real>(0);
        result.ul = d;
      }
      // clamp result.uls to 1
      else if (sn > denom)
      {
        result.us = static_cast<Real>(1);
        result.ul = (d + b);
      }
      else
      {
        result.us = sn / denom;
        result.ul = (a*d - b*c) / denom;
      }
    }

    result.cps = os + result.us*ds;
    result.cpl = ol + result.ul*dl;
    return result;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Line2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Line2<Real>>::operator()
	  (Segment2<Real> const & a_seg, Line2<Real> const & a_line)
  {
	  Result result;

	  Vector2<Real> const & dir_s = a_seg.Direction();
	  Vector2<Real> const & dir_l = a_line.Direction();

	  Vector2<Real> w = a_seg.GetP0() - a_line.Origin();
	  Real denom = PerpDot(dir_s, dir_l);
	  Real us_numerator = PerpDot(dir_l, w);
	  Real ul_numerator = PerpDot(dir_s, w);

	  if (Dg::IsZero(denom))
	  {
      //Parallel
		  if (!Dg::IsZero(us_numerator))
			  result.code = QueryCode::NotIntersecting;

      //Coincident
		  else
		  {
			  result.code = QueryCode::Overlapping;
			  result.p = a_seg.GetP0();
			  result.us = static_cast<Real>(0);
			  result.ul = Dot(dir_l, w);
		  }
	  }
	  else
	  {
		  Real us = us_numerator / denom;
		  Real ul = ul_numerator / denom;

		  if (IsInRange(static_cast<Real>(0), static_cast<Real>(1), us))
		  {
			  result.code = QueryCode::Intersecting;
			  result.us = us;
			  result.ul = ul;
			  result.p = a_seg.GetP0() + us * dir_s;
		  }
		  else
		  {
        result.code = QueryCode::NotIntersecting;
		  }
	  }
	  return result;
  }
}

#endif