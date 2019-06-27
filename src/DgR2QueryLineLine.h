//! @file DgR2QueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYLINELINE_H
#define DGR2QUERYLINELINE_H

#include "..\impl\DgQueryLineLine.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPLineLine = impl::CPQuery<Real, 2, impl::Vector_generic<Real, 2>, impl::Line_generic<Real, 2>>;
  }
}

#endif