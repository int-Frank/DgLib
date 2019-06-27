//! @file DgR2QueryRayLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYRAYLINE_H
#define DGR2QUERYRAYLINE_H

#include "..\impl\DgQueryRayLine.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPLineLine = impl::CPQuery<Real, 2, impl::Ray_generic<Real, 2>, impl::Line_generic<Real, 2>>;
  }
}

#endif