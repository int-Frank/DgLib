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

#ifndef DGQUERYSEGMENTLINE_H
#define DGQUERYSEGMENTLINE_H

#include "../DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgFIQuery.h"
#include "DgLine_generic.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line segment, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Segment_generic<Real, R>, 
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the line origin to closest point to the line segment
        Real ul;

        //! Distance from the line segment origin to closest point to the line
        Real us;

        //! Closest point on line to the line segment
        Vector_generic<Real, R> cpl;

        //! Closest point on line segment to the line
        Vector_generic<Real, R> cps;

        //! Return code. Codes include:
        //! Success, Parallel.
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


	//! @ingroup DgMath_geoQueries
	//! Distance and closest-point query: Line segment, Line
	template <typename Real>
	class FIQuery<Real, 2,
		          Segment_generic<Real, 2>,
		          Line_generic<Real, 2>>
	{
	public:

		//! Query return data
		struct Result
		{
			//! Distance from the line origin to closest point to the line segment
			Real us;

			//! Distance from the line segment origin to closest point to the line
			Real ul;

			//! Intersection point
			Vector_generic<Real, 2> p;

			//! Return code. Codes include:
			//! NotIntersecting, Intersecting, Overlapping
			QueryCode code;
		};

		//! Perform query.
		Result operator()(Segment_generic<Real, 2> const &, Line_generic<Real, 2> const &);
	};


	//--------------------------------------------------------------------------------
	//	@	FIQuery::operator()
	//--------------------------------------------------------------------------------
	template<typename Real>
	typename FIQuery<Real, 2, Segment_generic<Real, 2>, Line_generic<Real, 2>>::Result
		FIQuery<Real, 2, Segment_generic<Real, 2>, Line_generic<Real, 2>>::operator()
		(Segment_generic<Real, 2> const & a_seg, Line_generic<Real, 2> const & a_line)
	{
		Result result;

		Vector_generic<Real, 2> const & dir_s = a_seg.Direction();
		Vector_generic<Real, 2> const & dir_l = a_line.Direction();

		Vector_generic<Real, 2> w = a_seg.GetP0() - a_line.Origin();
		Real denom = dir_s.PerpDot(dir_l);
		Real us_numerator = dir_l.PerpDot(w);
		Real ul_numerator = dir_s.PerpDot(w);

		if (Dg::IsZero(denom))
		{
      //Parallel
			if (!Dg::IsZero(us_numerator))
			{
				result.code = QueryCode::QC_NotIntersecting;
			}

      //Coincident
			else
			{
				result.code = QueryCode::QC_Overlapping;
				result.p = a_seg.GetP0();
				result.us = static_cast<Real>(0);
				result.ul = a_line.Direction().Dot(w);
			}
		}
		else
		{
			Real us = us_numerator / denom;
			Real ul = ul_numerator / denom;

			if (IsInRange(static_cast<Real>(0), static_cast<Real>(1), us))
			{
				result.code = QueryCode::QC_Intersecting;
				result.us = us;
				result.ul = ul;
				result.p = a_seg.GetP0() + us * dir_s;
			}
			else
			{
        result.code = QueryCode::QC_NotIntersecting;
			}
		}
		return result;
	} //End: FIQuery::operator()


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Segment_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Segment_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Segment_generic<Real, R> const & a_seg, Line_generic<Real, R> const & a_line)
    {
      Result result;

      Vector_generic<Real, R> os(a_seg.Origin());
      Vector_generic<Real, R> ol(a_line.Origin());
      Vector_generic<Real, R> ds(a_seg.Direction());
      Vector_generic<Real, R> dl(a_line.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(os - ol);
      Real a = ds.Dot(ds);
      Real b = ds.Dot(dl);
      Real c = ds.Dot(w0);
      Real d = dl.Dot(w0);
      Real denom = a - b*b;

      // if denom is zero, try finding closest point on line to segment origin
      if (Dg::IsZero(denom))
      {
        result.us = static_cast<Real>(0.0);
        result.ul = d;

        if (IsZero(a))
        {
          result.code = QueryCode::QC_Success;
        }
        else
        {
          result.code = QueryCode::QC_Parallel;
        }
      }
      else
      {
        result.code = QueryCode::QC_Success;

        // clamp result.uls within [0,1]
        Real sn = b*d - c;

        // clamp result.uls to 0
        if (sn < static_cast<Real>(0.0))
        {
          result.us = static_cast<Real>(0.0);
          result.ul = d;
        }
        // clamp result.uls to 1
        else if (sn > denom)
        {
          result.us = static_cast<Real>(1.0);
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
    } //End: CPQuery::operator()
  }
}

#endif