//! @file DgR3QueryRayLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYRAYLINE_H
#define DGR3QUERYRAYLINE_H

#include "..\impl\DgQueryRayLine.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPRayLine = impl::CPQuery<Real, 3, impl::Ray_generic<Real, 3>, impl::Line_generic<Real, 3>>;
  }
}

#endif