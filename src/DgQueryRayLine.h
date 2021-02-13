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

#ifndef DGQUERYRAYLINE_H
#define DGQUERYRAYLINE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgRay.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Ray, Line
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>
  {
  public:

    struct Result
    {
      Real ul; // Distance from the line origin to closest point to the ray
      Real ur; // Distance from the ray origin to closest point to the line

      Vector<Real, R> cpl; // Closest point on line to the ray
      Vector<Real, R> cpr; // Closest point on ray to the line

      QueryCode code; // Success, Parallel
    };

    Result operator()(Ray<Real, R> const &, Line<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2RayLine = Query<QueryType::ClosestPoint, Real, 2, Ray2<Real>, Line2<Real>>;

  template<typename Real>
  using CP3RayLine = Query<QueryType::ClosestPoint, Real, 3, Ray3<Real>, Line3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------
  
  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Line<Real, R>>::operator()
    (Ray<Real, R> const & a_ray, Line<Real, R> const & a_line)
  {
    Vector<Real, R> or (a_ray.Origin());
    Vector<Real, R> ol(a_line.Origin());
    Vector<Real, R> dr(a_ray.Direction());
    Vector<Real, R> dl(a_line.Direction());

    //compute intermediate parameters
    Vector<Real, R> diff(or -ol);
    Real a = Dot(dr, dl);
    Real b = Dot(dl, diff);
    Real c = static_cast<Real>(1) - a * a;

    Result result;
    result.code = QueryCode::Success;

    //if c is zero, try finding closest point on the ray to the line origin
    if (Dg::IsZero(c))
    {
      //compute closest points
      result.ur = static_cast<Real>(0);
      result.ul = b;
      result.code = QueryCode::Parallel;
    }
    else
    {
      // clamp ur within [0,+inf]
      Real d = Dot(dr, diff);
      Real sn = a * b - d;

      // clamp ur to 0
      if (sn < static_cast<Real>(0))
      {
        result.ur = static_cast<Real>(0);
        result.ul = b;
      }
      else
      {
        Real denom = static_cast<Real>(1) / c;
        result.ur = sn * denom;
        result.ul = (b - a*d) * denom;
      }
    }

    result.cpr = or +result.ur*dr;
    result.cpl = ol + result.ul*dl;
    return result;
  }
}

#endif