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

#ifndef DGQUERYSEGMENTPLANE_H
#define DGQUERYSEGMENTPLANE_H

#include "../DgQueryCommon.h"
#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgR3Segment.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line segment and a plane.
    template <typename Real>
    class TIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects plane.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(R3::Segment<Real> const &, R3::Plane<Real> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line segment and a plane.
    template <typename Real>
    class FIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersection. Set to origin if line segment is parallel to plane.
        Vector_generic<Real, 3> point;

        //! Distance from the line segment origin to the point of intersection. Set to 0 if line segment is parallel to the plane.
        Real u;

        //! Return code. Codes include:
        //! Intersecting, Overlapping, NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(R3::Segment<Real> const &, R3::Plane<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>::Result
      TIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>::operator()
      (R3::Segment<Real> const & a_seg, R3::Plane<Real> const & a_plane)
    {
      Result result;

      Vector_generic<Real, 3> pn(a_plane.Normal());
      Real                    po(a_plane.Offset());
      Vector_generic<Real, 3> so(a_seg.Origin());
      Vector_generic<Real, 3> sd(a_seg.Direction());

      Real denom = pn.Dot(sd);

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        //check if ray is on the plane
        if (Dg::IsZero(a_plane.SignedDistance(so)))
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
        Real u = (-(so.Dot(pn) + po) / denom);

        if (u < static_cast<Real>(0.0) ||
          u > static_cast<Real>(1.0))
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
      //	@	FIQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>::Result
      FIQuery<Real, 3, R3::Segment<Real>, R3::Plane<Real>>::operator()
      (R3::Segment<Real> const & a_seg, R3::Plane<Real> const & a_plane)
    {
      Result result;

      Vector_generic<Real, 3> pn(a_plane.Normal());
      Real          po(a_plane.Offset());
      Vector_generic<Real, 3> so(a_seg.Origin());
      Vector_generic<Real, 3> sd(a_seg.Direction());

      Real denom = pn.Dot(sd);

      //check if line is parallel to plane
      if (Dg::IsZero(denom))
      {
        result.u = static_cast<Real>(0.0);

        //check if line is on the plane
        if (Dg::IsZero(a_plane.Distance(so)))
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
        result.u = (-(so.Dot(pn) + po) / denom);
        if (result.u < static_cast<Real>(0.0))
        {
          result.u = static_cast<Real>(0.0);
          result.code = QueryCode::QC_NotIntersecting;
        }
        else if (result.u > static_cast<Real>(1.0))
        {
          result.u = static_cast<Real>(1.0);
          result.code = QueryCode::QC_NotIntersecting;
        }
        else
        {
          result.code = QueryCode::QC_Intersecting;
        }
      }

      result.point = so + result.u * sd;
      return result;
    } //End: FIQuery::operator()
  }
}

#endif