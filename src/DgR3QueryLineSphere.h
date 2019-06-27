//! @file DgR3QueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGR3QUERYLINESPHERE_H
#define DGR3QUERYLINESPHERE_H

#include "..\impl\DgQueryLineHypersphere.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using TILineSphere = impl::TIQuery<Real, 3, impl::Line_generic<Real, 3>, impl::Hypersphere_generic<Real, 3>>;

    template<typename Real>
    using FILineSphere = impl::FIQuery<Real, 3, impl::Line_generic<Real, 3>, impl::Hypersphere_generic<Real, 3>>;
  }
}

#endif