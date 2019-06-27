//! @file DgR3QuerySegmentSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTLINE_H
#define DGR3QUERYSEGMENTLINE_H

#include "..\impl\DgQuerySegmentBall.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TISegmentSphere = impl::TIQuery<Real, 3, impl::Segment_generic<Real, 3>, impl::Ball_generic<Real, 3>>;
  }
}

#endif