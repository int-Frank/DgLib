//! @file DgR3QueryPointPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTPLANE_H
#define DGR3QUERYPOINTPLANE_H

#include "..\impl\DgQueryPointPlane.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPPointPlane = impl::CPQuery<Real, 3, impl::Vector_generic<Real, 3>, Plane<Real>>;
  }
}

#endif