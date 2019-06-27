//! @file DgR2QueryRayRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYRAYRAY_H
#define DGR2QUERYRAYRAY_H

#include "..\impl\DgQueryRayRay.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPRayRay = impl::CPQuery<Real, 2, impl::Ray_generic<Real, 2>, impl::Ray_generic<Real, 2>>;
  }
}

#endif