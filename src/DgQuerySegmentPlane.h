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

#ifndef DGQUERYSEGMENTPLANE_H
#define DGQUERYSEGMENTPLANE_H

#include "DgQuery.h"
#include "DgSegment.h"
#include "DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line segment and a plane.
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 3, Segment3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Segment3<Real> const &, Plane<Real> const &);
  };

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 3, Segment3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      Vector3<Real> point;  // Point of intersection. Set to origin if line segment is parallel to plane.
      Real u;               // Distance from the line segment origin to the point of intersection. Set to 0 if line segment is parallel to the plane.
      QueryCode code;       // Intersecting, Overlapping, NotIntersecting
    };

    //! Perform query
    Result operator()(Segment3<Real> const &, Plane<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI3SegmentPlane = Query<QueryType::TestForIntersection, Real, 3, Segment3<Real>, Plane<Real>>;

  template<typename Real>
  using FI3SegmentPlane = Query<QueryType::FindIntersection, Real, 3, Segment3<Real>, Plane<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 3, Segment3<Real>, Plane<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 3, Segment3<Real>, Plane<Real>>::operator()
    (Segment3<Real> const & a_seg, Plane<Real> const & a_plane)
  {
    Result result;

    Vector3<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector3<Real> so(a_seg.Origin());
    Vector3<Real> sd(a_seg.Direction());

    Real denom = Dot(pn, sd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if ray is on the plane
      if (Dg::IsZero(a_plane.SignedDistance(so)))
        result.isIntersecting = true;
      else
        result.isIntersecting = false;
    }
    else
    {
      Real u = (-(Dot(so, pn) + po) / denom);

      if (u < static_cast<Real>(0) || u > static_cast<Real>(1))
        result.isIntersecting = false;
      else
        result.isIntersecting = true;
    }

    return result;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 3, Segment3<Real>, Plane<Real>>::Result
    Query<QueryType::FindIntersection, Real, 3, Segment3<Real>, Plane<Real>>::operator()
    (Segment3<Real> const & a_seg, Plane<Real> const & a_plane)
  {
    Result result;

    Vector3<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector3<Real> so(a_seg.Origin());
    Vector3<Real> sd(a_seg.Direction());

    Real denom = Dot(pn, sd);

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0);

      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(so)))
        result.code = QueryCode::Overlapping;
      else
        result.code = QueryCode::NotIntersecting;
    }
    else
    {
      result.u = (-(Dot(so, pn) + po) / denom);
      if (result.u < static_cast<Real>(0))
      {
        result.u = static_cast<Real>(0);
        result.code = QueryCode::NotIntersecting;
      }
      else if (result.u > static_cast<Real>(1))
      {
        result.u = static_cast<Real>(1);
        result.code = QueryCode::NotIntersecting;
      }
      else
      {
        result.code = QueryCode::Intersecting;
      }
    }

    result.point = so + result.u * sd;
    return result;
  } //End: FIQuery::operator()
}

#endif