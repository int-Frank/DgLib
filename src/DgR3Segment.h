//! @file DgR3Segment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment_general

#ifndef DGR3SEGMENT_H
#define DGR3SEGMENT_H

#include "impl/DgSegment_generic.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Segment = Dg::impl::Segment_generic<Real, 3>;
  }
}
#endif