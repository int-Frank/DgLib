//! @file DgR3QueryRayPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYRAYPLANE_H
#define DGR3QUERYRAYPLANE_H

#include "../impl/DgQueryRayPlane.h"
#include "../DgR3Ray.h"
#include "../DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TIRayPlane = impl::TIQuery<Real, 3, Ray<Real>, Plane<Real>>;

    template<typename Real>
    using FIRayPlane = impl::FIQuery<Real, 3, Ray<Real>, Plane<Real>>;
  }
}

#endif