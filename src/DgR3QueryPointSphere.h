//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYPOINTSPHERE_H
#define DGR3QUERYPOINTSPHERE_H

#include "..\impl\DgFPCQueryPointHypersphere.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using FPCPointSphere = impl::FPCQuery<Real, 3, impl::Hypersphere_generic<Real, 3>, impl::Vector_generic<Real, 3>>;
  }
}

#endif