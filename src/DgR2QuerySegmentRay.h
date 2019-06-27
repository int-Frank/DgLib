//! @file DgR2QuerySegmentRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYSEGMENTRAY_H
#define DGR2QUERYSEGMENTRAY_H

#include "..\impl\DgQuerySegmentRay.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPSegmentRay = impl::CPQuery<Real, 2, impl::Segment_generic<Real, 2>, impl::Ray_generic<Real, 2>>;
  }
}

#endif