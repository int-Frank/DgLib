//! @file DgR3QueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTSEGMENT_H
#define DGR3QUERYSEGMENTSEGMENT_H

#include "..\impl\DgQuerySegmentSegment.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPSegmentSegment = impl::CPQuery<Real, 3, impl::Segment_generic<Real, 3>, impl::Segment_generic<Real, 3>>;
  }
}

#endif