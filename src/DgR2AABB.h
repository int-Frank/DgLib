//! @file DgR2AABB.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: AABB

#ifndef DGR2AABB_H
#define DGR2AABB_H

#include "impl/DgAABB_generic.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using AABB = impl::AABB_generic<Real, 2>;
  }
}
#endif