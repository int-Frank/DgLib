//! @file DgR2Triangle.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Triangle

#ifndef DGR2TRIANGLE_H
#define DGR2TRIANGLE_H

#include "impl/DgTriangle_generic.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Triangle = Dg::impl::Triangle_generic<Real, 2>;
  }
}
#endif