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

#ifndef DGQUERYPOINTRAY_H
#define DGQUERYPOINTRAY_H

#include "DgCPQuery.h"
#include "DgVector_generic.h"
#include "DgRay_generic.h"
#include "../DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Ray.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Ray_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from ray origin to closest point to point
        Real u;

        //! Closest point on the ray to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &, Ray_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Ray_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Ray_generic<Real, R> const & a_ray)
    {
      Result result;

      Vector_generic<Real, R> w = a_point - a_ray.Origin();
      result.u = w.Dot(a_ray.Direction());

      if (result.u < static_cast<Real>(0.0))
      {
        result.u = static_cast<Real>(0.0);
      }

      result.cp = a_ray.Origin() + result.u * a_ray.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif