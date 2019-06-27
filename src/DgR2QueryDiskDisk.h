//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYDISKDISK_H
#define DGR3QUERYDISKDISK_H

#include "..\impl\DgFPCQueryHypersphereHypersphere.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using FPCDiskDisk = impl::FPCQuery<Real, 2, impl::Hypersphere_generic<Real, 2>, impl::Hypersphere_generic<Real, 2>>;
  }
}

#endif