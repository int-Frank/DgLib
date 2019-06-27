//! @file DgR2QueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYPOINTRAY_H
#define DGR2QUERYPOINTRAY_H

#include "..\impl\DgQueryPointRay.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPPointRay = impl::CPQuery<Real, 2, impl::Vector_generic<Real, 2>, impl::Ray_generic<Real, 2>>;
  }
}

#endif