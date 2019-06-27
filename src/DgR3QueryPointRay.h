//! @file DgR3QueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTRAY_H
#define DGR3QUERYPOINTRAY_H

#include "..\impl\DgQueryPointRay.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPPointRay = impl::CPQuery<Real, 3, impl::Vector_generic<Real, 3>, impl::Ray_generic<Real, 3>>;
  }
}

#endif