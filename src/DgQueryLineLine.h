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

#ifndef DGQUERYLINELINE_H
#define DGQUERYLINELINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgVector.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>
  {
  public:

    struct Result
    {
      Real u0;            // Distance from line 0 origin to closest point to line 1
      Real u1;            // Distance from line 1 origin to closest point to line 0

      Vector3<Real> cp0;  // Closest point on line 0 to line 1
      Vector3<Real> cp1;  // Closest point on line 1 to line 0

      QueryCode code;     // Success, Parallel
    };

    Result operator()(Line3<Real> const &, Line3<Real> const &);
  };

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 2, Line2<Real>, Line2<Real>>
  {
  public:

    struct Result
    {
      Real u0;            // Distance from line 0 origin to closest point to line 1
      Real u1;            // Distance from line 1 origin to closest point to line 0

      Vector3<Real> pt;   // Intersection point

      QueryCode code;     // Success, Parallel, Overlapping
    };

    Result operator()(Line2<Real> const &, Line2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3LineLine = Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>;

  template<typename Real>
  using FI2LineLine = Query<QueryType::FindIntersection, Real, 2, Line2<Real>, Line2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Line3<Real>, Line3<Real>>::operator()
    (Line3<Real> const & a_line0, Line3<Real> const & a_line1)
  {
    Result result;

    Vector3<Real> o0(a_line0.Origin());
    Vector3<Real> o1(a_line1.Origin());
    Vector3<Real> d0(a_line0.Direction());
    Vector3<Real> d1(a_line1.Direction());

    //compute intermediate parameters
    Vector3<Real> w0(o0 - o1);
    Real a = Dot(d0, d1);
    Real b = Dot(d0, w0);
    Real c = Dot(d1, w0);
    Real d = static_cast<Real>(1) - a*a;

    if (Dg::IsZero(d))
    {
      result.u0 = static_cast<Real>(0);
      result.u1 = c;
      result.code = QueryCode::Parallel;
    }
    else
    {
      result.u0 = ((a*c - b) / d);
      result.u1 = ((c - a*b) / d);
      result.code = QueryCode::Success;
    }

    result.cp0 = o0 + result.u0 * d0;
    result.cp1 = o1 + result.u1 * d1;
    return result;
  }
}

#endif