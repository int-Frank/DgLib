//! @file DgR3QueryRayTriangle.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYRAYTRIANGLE_H
#define DGR3QUERYRAYTRIANGLE_H

#include "../impl/DgQueryRayTriangle.h"
#include "../DgR3Ray.h"
#include "../DgR3Triangle.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TIRayTriangle = impl::TIQuery<Real, 3, Ray<Real>, Triangle<Real>>;
  }
}

#endif