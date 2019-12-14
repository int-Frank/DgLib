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

#ifndef DGQUERYLINEHYPERSPHERE_H
#define DGQUERYLINEHYPERSPHERE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "../DgQueryCommon.h"
#include "DgLine_generic.h"
#include "DgHypersphere_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a sphere.
    template <typename Real, int R>
    class TIQuery<Real, R,
                  Line_generic<Real, R>, 
                  Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects sphere.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Line_generic<Real, R> const &, Hypersphere_generic<Real, R> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line and a sphere.
    template<typename Real, int R>
    class FIQuery<Real, R,
                  Line_generic<Real, R>, 
                  Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersection from u0. Not set if line does not intersect sphere.
        Vector_generic<Real, R> p0;

        //! Point of intersection from u1. Not set if line does not intersect sphere.
        Vector_generic<Real, R> p1;

        //! Distance from the line origin to the first point of intersection. Not set if line does not intersect sphere.
        Real u0;

        //! Distance from the line origin to the second point of intersection. Not set if line does not intersect sphere.
        Real u1;

        //! Return code. Codes include:
        //! Intersecting, NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(Line_generic<Real, R> const &, Hypersphere_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename TIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::Result
      TIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line, Hypersphere_generic<Real, R> const & a_sphere)
    {
      Result result;
      Vector_generic<Real, R> w0(a_line.Origin() - a_sphere.Center());
      Real a = a_line.Direction().LengthSquared();
      Real b = w0.Dot(a_line.Direction());
      Real c = w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius();
      result.isIntersecting = (b*b >= a*c);
      return result;
    } //End: CPQuery::operator()


      //--------------------------------------------------------------------------------
      //	@	CPQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename FIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::Result
      FIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line, Hypersphere_generic<Real, R> const & a_sphere)
    {
      Result result;
      Vector_generic<Real, R> w0(a_line.Origin() - a_sphere.Center());
      Real a = a_line.Direction().LengthSquared();
      Real b = static_cast<Real>(2.0) * w0.Dot(a_line.Direction());
      Real c = w0.LengthSquared() - a_sphere.Radius() * a_sphere.Radius();

      Real discr = b*b - static_cast<Real>(4.0)*a*c;
      if (discr < static_cast<Real>(0.0))
      {
        result.code = QueryCode::NotIntersecting;
      }
      else
      {
        Real d = sqrt(discr);
        Real e = static_cast<Real>(1.0) / (static_cast<Real>(2.0) * a);
        result.u0 = (-b - d) * e;
        result.u1 = (-b + d) * e;
        result.p0 = a_line.Origin() + result.u0 * a_line.Direction();
        result.p1 = a_line.Origin() + result.u1 * a_line.Direction();
        result.code = QueryCode::Intersecting;
      }

      return result;
    } //End: CPQuery::operator()
  }
}

#endif