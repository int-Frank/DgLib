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

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPlane.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>
  {
  public:

    struct Result
    {
      Real distance;
      Real signedDistance;
      Vector3<Real> cp;
    };

    Result operator()(Vector3<Real> const &, Plane<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3PointPlane = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, Plane<Real>>::operator()
    (Vector3<Real> const & a_point, Plane<Real> const & a_plane)
  {
    Result result;

    result.signedDistance = a_plane.SignedDistance(a_point);
    result.distance = abs(result.signedDistance);
    result.cp = a_point - result.signedDistance * a_plane.Normal();

    return result;
  }
}

#endif