//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYDISKLINE_H
#define DGR3QUERYDISKLINE_H

#include "..\impl\DgFPCQueryHypersphereLine.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using FPCDiskLine = impl::FPCQuery<Real, 2, impl::Hypersphere_generic<Real, 2>, impl::Line_generic<Real, 2>>;
  }
}

#endif