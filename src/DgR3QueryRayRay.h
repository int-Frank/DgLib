//! @file DgR3QueryRayRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYRAYRAY_H
#define DGR3QUERYRAYRAY_H

#include "..\impl\DgQueryRayRay.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPRayRay = impl::CPQuery<Real, 3, impl::Ray_generic<Real, 3>, impl::Ray_generic<Real, 3>>;
  }
}

#endif