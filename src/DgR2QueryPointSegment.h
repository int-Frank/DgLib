//! @file DgR2QueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYPOINTSEGMENT_H
#define DGR2QUERYPOINTSEGMENT_H

#include "..\impl\DgQueryPointSegment.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPPointSegment = impl::CPQuery<Real, 2, impl::Vector_generic<Real, 2>, impl::Segment_generic<Real, 2>>;
  }
}

#endif