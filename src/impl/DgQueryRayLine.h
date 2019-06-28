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

#include "../DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgLine_generic.h"
#include "DgRay_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Ray, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Ray_generic<Real, R>, 
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the line origin to closest point to the ray
        Real ul;

        //! Distance from the ray origin to closest point to the line
        Real ur;

        //! Closest point on line to the ray
        Vector_generic<Real, R> cpl;

        //! Closest point on ray to the line
        Vector_generic<Real, R> cpr;

        //! Return code. Codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Ray_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Ray_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Ray_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Ray_generic<Real, R> const & a_ray, Line_generic<Real, R> const & a_line)
    {
      Vector_generic<Real, R> or (a_ray.Origin());
      Vector_generic<Real, R> ol(a_line.Origin());
      Vector_generic<Real, R> dr(a_ray.Direction());
      Vector_generic<Real, R> dl(a_line.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> diff(or -ol);
      Real a = dr.Dot(dl);
      Real b = dl.Dot(diff);
      Real c = static_cast<Real>(1.0) - a * a;

      Result result;

      //if c is zero, try finding closest point on the ray to the line origin
      if (Dg::IsZero(c))
      {
        //compute closest points
        result.ur = static_cast<Real>(0.0);
        result.ul = b;
        result.code = QueryCode::QC_Parallel;
      }
      else
      {
        // clamp ur within [0,+inf]
        Real d = dr.Dot(diff);
        Real sn = a * b - d;

        // clamp ur to 0
        if (sn < static_cast<Real>(0.0))
        {
          result.ur = static_cast<Real>(0.0);
          result.ul = b;
        }
        else
        {
          Real denom = static_cast<Real>(1.0) / c;
          result.ur = sn * denom;
          result.ul = (b - a*d) * denom;
        }

        result.code = QC_Success;
      }

      result.cpr = or +result.ur*dr;
      result.cpl = ol + result.ul*dl;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif