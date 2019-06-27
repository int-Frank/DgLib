//! @file DgR3QueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTSEGMENT_H
#define DGR3QUERYPOINTSEGMENT_H

#include "..\impl\DgQueryPointSegment.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPPointSegment = impl::CPQuery<Real, 3, impl::Vector_generic<Real, 3>, impl::Segment_generic<Real, 3>>;
  }
}

#endif