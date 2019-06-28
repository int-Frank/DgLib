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

#include "DgCPQuery.h"
#include "../DgQueryCommon.h"
#include "DgLine_generic.h"
#include "DgVector_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Line_generic<Real, R>,
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from line 0 origin to closest point to line 1
        Real u0;

        //! Distance from line 1 origin to closest point to line 0
        Real u1;

        //! Closest point on line 0 to line 1
        Vector_generic<Real, R> cp0;

        //! Closest point on line 1 to line 0
        Vector_generic<Real, R> cp1;

        //! Return codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Line_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Line_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Line_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line0, Line_generic<Real, R> const & a_line1)
    {
      Result result;

      Vector_generic<Real, R> o0(a_line0.Origin());
      Vector_generic<Real, R> o1(a_line1.Origin());
      Vector_generic<Real, R> d0(a_line0.Direction());
      Vector_generic<Real, R> d1(a_line1.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(o0 - o1);
      Real a = d0.Dot(d1);
      Real b = d0.Dot(w0);
      Real c = d1.Dot(w0);
      Real d = static_cast<Real>(1.0) - a*a;

      if (Dg::IsZero(d))
      {
        result.u0 = static_cast<Real>(0.0);
        result.u1 = c;
        result.code = QueryCode::QC_Parallel;
      }
      else
      {
        result.u0 = ((a*c - b) / d);
        result.u1 = ((c - a*b) / d);
        result.code = QueryCode::QC_Success;
      }

      result.cp0 = o0 + result.u0 * d0;
      result.cp1 = o1 + result.u1 * d1;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif