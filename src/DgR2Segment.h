//! @file DgR2Segment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment

#ifndef DGR2SEGMENT_H
#define DGR2SEGMENT_H

#include "impl/DgSegment_generic.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Segment = Dg::impl::Segment_generic<Real, 2>;
  }
}
#endif