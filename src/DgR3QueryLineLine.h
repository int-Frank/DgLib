//! @file DgR3QueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYLINELINE_H
#define DGR3QUERYLINELINE_H

#include "..\impl\DgQueryLineLine.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPLineLine = impl::CPQuery<Real, 3, impl::Line_generic<Real, 3>, impl::Line_generic<Real, 3>>;
  }
}

#endif