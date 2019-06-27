//! @file DgR3QueryFrustumSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYFRUSTUMSPHERE_H
#define DGR3QUERYFRUSTUMSPHERE_H

#include "..\impl\DgQueryFrustumSphere.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TIFrustumSphere = impl::TIQuery<Real, 3, FrustumData<Real>, Sphere<Real>>;
  }
}

#endif