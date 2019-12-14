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

#ifndef DGQUERYRAYRAY_H
#define DGQUERYRAYRAY_H

#include "../DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgRay_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Ray, Ray
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Ray_generic<Real, R>, 
                  Ray_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from ray 0 origin to closest point to ray 1
        Real u0;

        //! Distance from ray 1 origin to closest point to ray 0
        Real u1;

        //! Closest point on ray 0 to ray 1
        Vector_generic<Real, R> cp0;

        //! Closest point on ray 1 to ray 0
        Vector_generic<Real, R> cp1;

        //! Return code. Codes include:
        //! Success, Overlapping
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Ray_generic<Real, R> const &, Ray_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Ray_generic<Real, R>, Ray_generic<Real, R>>::Result
      CPQuery<Real, R, Ray_generic<Real, R>, Ray_generic<Real, R>>::operator()
      (Ray_generic<Real, R> const & a_ray0, Ray_generic<Real, R> const & a_ray1)
    {
      Result result;
      result.code = QueryCode::QC_Success;

      Vector_generic<Real, R> o0(a_ray0.Origin());
      Vector_generic<Real, R> o1(a_ray1.Origin());
      Vector_generic<Real, R> d0(a_ray0.Direction());
      Vector_generic<Real, R> d1(a_ray1.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(o0 - o1);
      Real a = d0.Dot(d1);
      Real b = d0.Dot(w0);
      Real c = d1.Dot(w0);
      Real d = static_cast<Real>(1.0) - a * a;

      //parameters to compute result.u0 and result.u1
      Real sn, sd, tn, td;

      //if denom is zero, try finding closest point on ray1 to origin of ray0
      if (Dg::IsZero(d))
      {
        //clamp result.u0 to 0
        sd = td = static_cast<Real>(1.0);
        sn = static_cast<Real>(0.0);
        tn = c;

        //Do the rays overlap?
        if (!(c < static_cast<Real>(0.0) &&
          b > static_cast<Real>(0.0)))
        {
          result.code = QueryCode::QC_Overlapping;
        }
      }
      else
      {
        //clamp result.u0 within[0, +inf]
        sd = td = d;
        sn = a*c - b;
        tn = c - a*b;

        //clamp result.u0 to 0
        if (sn < static_cast<Real>(0.0))
        {
          sn = static_cast<Real>(0.0);
          tn = c;
          td = static_cast<Real>(1.0);
        }
      }

      //clamp result.u1 within [0, +inf]
      //clamp result.u1 to 0
      if (tn < static_cast<Real>(0.0))
      {
        result.u1 = static_cast<Real>(0.0);

        //clamp result.u0 to 0
        if (-b < static_cast<Real>(0.0))
        {
          result.u0 = static_cast<Real>(0.0);
        }
        else
        {
          result.u0 = -b;
        }
      }
      else
      {
        result.u1 = tn / td;
        result.u0 = sn / sd;
      }

      result.cp0 = o0 + result.u0*d0;
      result.cp1 = o1 + result.u1*d1;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif