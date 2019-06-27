//! @file DgR3QuerySegmentRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTRAY_H
#define DGR3QUERYSEGMENTRAY_H

#include "..\impl\DgQuerySegmentRay.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPSegmentRay = impl::CPQuery<Real, 3, impl::Segment_generic<Real, 3>, impl::Ray_generic<Real, 3>>;
  }
}

#endif