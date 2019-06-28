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

#ifndef DGQUERYPOINTLINE_H
#define DGQUERYPOINTLINE_H

#include "DgCPQuery.h"
#include "../DgQueryCommon.h"
#include "DgLine_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Line.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Line_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line origin to closest point to point
        Real u;

        //! Closest point on the line to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &,
                        Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Line_generic<Real, R> const & a_line)
    {
      Result result;

      Vector_generic<Real, R> w = a_point - a_line.Origin();
      result.u = w.Dot(a_line.Direction());
      result.cp = a_line.Origin() + result.u * a_line.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif