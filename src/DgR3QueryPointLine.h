//! @file DgR3QueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTLINE_H
#define DGR3QUERYPOINTLINE_H

#include "..\impl\DgQueryPointLine.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPPointLine = impl::CPQuery<Real, 3, impl::Vector_generic<Real, 3>, impl::Line_generic<Real, 3>>;
  }
}

#endif