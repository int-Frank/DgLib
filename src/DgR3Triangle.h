//! @file DgR3Triangle.h
//!
//! @author: Frank B. Hart
//! @date 
//!
//! Class declaration: Triangle

#ifndef DGR3TRIANGLE_H
#define DGR3TRIANGLE_H

#include "impl/DgTriangle_generic.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Triangle = Dg::impl::Triangle_generic<Real, 3>;
  }
}
#endif