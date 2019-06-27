//! @file DgR2Disk.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Disk

#ifndef DGR2DISK_H
#define DGR2DISK_H

#include "impl/DgHypersphere_generic.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Disk = Dg::impl::Hypersphere_generic<Real, 2>;
  }
}
#endif