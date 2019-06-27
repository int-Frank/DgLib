//! @file DgR2QueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYPOINTLINE_H
#define DGR2QUERYPOINTLINE_H

#include "..\impl\DgQueryPointLine.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPPointLine = impl::CPQuery<Real, 2, impl::Vector_generic<Real, 2>, impl::Line_generic<Real, 2>>;
  }
}

#endif