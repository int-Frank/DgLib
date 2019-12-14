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

#ifndef DGQUERYLINEPLANE_H
#define DGQUERYLINEPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "../DgQueryCommon.h"
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