//@group Math/R2/Queries

//! @file DgR3QueryLineSphere.h
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYPOINTAABB_H
#define DGR3QUERYPOINTAABB_H

#include "impl\DgQueryPointAABB.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using TIPointAABB = impl::TIQuery<Real, 2, impl::Vector_generic<Real, 2>, impl::AABB_generic<Real, 2>>;
  }
}

#endif