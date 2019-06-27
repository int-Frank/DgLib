//! @file DgR3QueryLinePlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYLINEPLANE_H
#define DGR3QUERYLINEPLANE_H

#include "../impl/DgQueryLinePlane.h"
#include "../DgR3Line.h"
#include "../DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TILinePlane = impl::TIQuery<Real, 3, Line<Real>, Plane<Real>>;
    
    template<typename Real>
    using FILinePlane = impl::FIQuery<Real, 3, Line<Real>, Plane<Real>>;
  }
}

#endif