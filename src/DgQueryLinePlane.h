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

#ifndef DGQUERYLINEPLANE_H
#define DGQUERYLINEPLANE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a plane.
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 3, Line3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      bool isIntersecting; // True if line intersects plane.
    };

    Result operator()(Line3<Real> const &, Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a plane.
  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 3, Line3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      Vector3<Real> point; // Point of intersectLine<Real>ion. Set to origin if line is parallel to plane.
      Real u;              // Distance from the line origin to the point of intersection. Set to 0 if line is parallel to the plane.
      QueryCode code;      // Intersecting, Overlapping, NoIntersecting
    };

    Result operator()(Line3<Real> const &, Plane<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real> using TI3LinePlane = Query<QueryType::TestForIntersection, Real, 3, Line3<Real>, Plane<Real>>;
  template<typename Real> using FI3LinePlane = Query<QueryType::FindIntersection, Real, 3, Line3<Real>, Plane<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 3, Line3<Real>, Plane<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 3, Line3<Real>, Plane<Real>>::operator()
    (Line3<Real> const & a_line, Plane<Real> const & a_plane)
  {
    Result result;
    result.isIntersecting = !Dg::IsZero(a_plane.NormalDot(a_line.Direction())) ||
      Dg::IsZero(a_plane.SignedDistance(a_line.Origin()));
    return result;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 3, Line3<Real>, Plane<Real>>::Result
    Query<QueryType::FindIntersection, Real, 3, Line3<Real>, Plane<Real>>::operator()
    (Line3<Real> const & a_line, Plane<Real> const & a_plane)
  {
    Vector3<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector3<Real> lo(a_line.Origin());
    Vector3<Real> ld(a_line.Direction());

    Real denom = Dot(pn, ld);
    Result result;

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0);
      result.point = lo;

      //check if line is on the plane
      result.code = Dg::IsZero(a_plane.SignedDistance(lo)) ? QueryCode::Overlapping : QueryCode::NotIntersecting;
    }
    else
    {
      result.u = -(Dot(lo, pn) + po) / denom;
      result.point = lo + result.u * ld;
      result.code = QueryCode::Intersecting;
    }

    return result;
  }
}
#endif