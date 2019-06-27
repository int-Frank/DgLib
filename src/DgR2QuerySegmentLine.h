//! @file DgR2QuerySegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYSEGMENTLINE_H
#define DGR2QUERYSEGMENTLINE_H

#include "..\impl\DgQuerySegmentLine.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using CPSegmentLine = impl::CPQuery<Real, 2, impl::Segment_generic<Real, 2>, impl::Line_generic<Real, 2>>;

    template<typename Real>
    using FISegmentLine = impl::FIQuery<Real, 2, impl::Segment_generic<Real, 2>, impl::Line_generic<Real, 2>>;
  }
}

#endif