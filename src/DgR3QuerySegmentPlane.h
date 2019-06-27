//! @file DgR3QuerySegmentPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYSEGMENTPLANE_H
#define DGR3QUERYSEGMENTPLANE_H

#include "../impl/DgQuerySegmentPlane.h"
#include "../DgR3Segment.h"
#include "../DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TISegmentPlane = impl::TIQuery<Real, 3, Segment<Real>, Plane<Real>>;

    template<typename Real>
    using FISegmentPlane = impl::FIQuery<Real, 3, Segment<Real>, Plane<Real>>;
  }
}

#endif