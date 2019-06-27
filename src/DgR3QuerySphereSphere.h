//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYSPHERESPHERE_H
#define DGR3QUERYSPHERESPHERE_H

#include "..\impl\DgFPCQueryHypersphereHypersphere.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using FPCSphereSphere = impl::FPCQuery<Real, 3, impl::Hypersphere_generic<Real, 3>, impl::Hypersphere_generic<Real, 3>>;
  }
}

#endif