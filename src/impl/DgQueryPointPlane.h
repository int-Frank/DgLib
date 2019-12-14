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

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgCPQuery.h"
#include "DgVector_generic.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Plane.
    template <typename Real>
    class CPQuery<Real, 3, 
                  Vector_generic<Real, 3>, 
                  R3::Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance between point and line
        Real distance;

        //! Signed distance between point and line
        Real sDistance;

        //! Closest point on the plane to the point.
        Vector_generic<Real, 3> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, 3> const &, R3::Plane<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, 3, Vector_generic<Real, 3>, R3::Plane<Real>>::Result
      CPQuery<Real, 3, Vector_generic<Real, 3>, R3::Plane<Real>>::operator()
      (Vector_generic<Real, 3> const & a_point, R3::Plane<Real> const & a_plane)
    {
      Result result;

      result.sDistance = a_plane.SignedDistance(a_point);
      result.distance = abs(result.sDistance);
      result.cp = a_point - result.sDistance * a_plane.Normal();

      return result;
    } //End: CPQuery::operator()
  }
}

#endif