//! @file DgR3Sphere.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Sphere

#ifndef DGR3SPHERE_H
#define DGR3SPHERE_H

#include "impl/DgHypersphere_generic.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Sphere = Dg::impl::Hypersphere_generic<Real, 3>;
  }
}
#endif