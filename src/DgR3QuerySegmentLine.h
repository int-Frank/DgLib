//! @file DgR3QuerySegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTLINE_H
#define DGR3QUERYSEGMENTLINE_H

#include "..\impl\DgQuerySegmentLine.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPSegmentLine = impl::CPQuery<Real, 3, impl::Segment_generic<Real, 3>, impl::Line_generic<Real, 3>>;
  }
}

#endif