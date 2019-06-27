//! @file DgR3AABB.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: AABB

#ifndef DGR3AABB_H
#define DGR3AABB_H

#include "impl/DgAABB_generic.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using AABB = impl::AABB_generic<Real, 3>;
  }
}
#endif