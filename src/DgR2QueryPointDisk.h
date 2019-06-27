//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYPOINTDISK_H
#define DGR3QUERYPOINTDISK_H

#include "..\impl\DgFPCQueryPointHypersphere.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using FPCPointDisk = impl::FPCQuery<Real, 2, impl::Hypersphere_generic<Real, 2>, impl::Vector_generic<Real, 2>>;
  }
}

#endif