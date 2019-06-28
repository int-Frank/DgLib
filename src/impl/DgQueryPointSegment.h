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

#ifndef DGQUERYPOINTSEGMENT_H
#define DGQUERYPOINTSEGMENT_H

#include "DgCPQuery.h"
#include "DgTIQuery.h"
#include "..\DgQueryCommon.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Segment.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Segment_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line segment origin to closest point to point
        Real u;

        //! Closest point on the line segment to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &, Segment_generic<Real, R> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Intersection test: Segment, Segment
    template <typename Real>
    class TIQuery<Real, 2,
                  Vector_generic<Real, 2>,
                  Segment_generic<Real, 2>>
    {
    public:

      //! Query return data
      struct Result
      {
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Vector_generic<Real, 2> const &,
                        Segment_generic<Real, 2> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::Result
      TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::operator()
      (Vector_generic<Real, 2> const & a_point, Segment_generic<Real, 2> const & a_seg)
    {
      Result result;
      result.isIntersecting = false;

      Vector_generic<Real, 2> w = a_point - a_seg.GetP0();

      Real proj = w.Dot(a_seg.Direction());

      if (proj > static_cast<Real>(0) && proj < static_cast<Real>(1))
      {
        Real perpDot = w.PerpDot(a_seg.Direction());
        if (IsZero(w.PerpDot(a_seg.Direction())))
        {
          result.isIntersecting = true;
        }
      }
      return result;
    }

    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Segment_generic<Real, R> const & a_seg)
    {
      Result result;
      Vector_generic<Real, R> w = a_point - a_seg.Origin();

      Real proj = w.Dot(a_seg.Direction());

      if (proj < Dg::Constants<Real>::EPSILON)
      {
        result.u = static_cast<Real>(0.0);
      }
      else
      {
        Real vsq = a_seg.LengthSquared();

        if (proj >= vsq)
        {
          result.u = static_cast<Real>(1.0);
        }
        else
        {
          result.u = (proj / vsq);
        }
      }

      result.cp = a_seg.Origin() + result.u * a_seg.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif