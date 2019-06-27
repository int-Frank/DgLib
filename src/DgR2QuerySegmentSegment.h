//! @file DgR2QueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYSEGMENTSEGMENT_H
#define DGR2QUERYSEGMENTSEGMENT_H

#include "..\impl\DgQuerySegmentSegment.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPSegmentSegment = impl::CPQuery<Real, 2, impl::Segment_generic<Real, 2>, impl::Segment_generic<Real, 2>>;
    
    template<typename Real>
    using TISegmentSegment = impl::TIQuery<Real, 2, impl::Segment_generic<Real, 2>, impl::Segment_generic<Real, 2>>;
  }
}

#endif