//! @file DgR2Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGVECTOR2D_H
#define DGVECTOR2D_H

#include "impl/DgR2Vector_generic.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Vector = Dg::impl::Vector_generic<Real, 2>;


    //! Squared distance between two points.
    template<typename Real>
    Real SquaredDistance(Vector<Real> const & a_p0, Vector<Real> const & a_p1)
    {
      return (a_p0 - a_p1).LengthSquared();
    } // End: SquaredDistance()


    //! Distance between two points.
    template<typename Real>
    Real Distance(Vector<Real> const & a_p0, Vector<Real> const & a_p1)
    {
      return (a_p0 - a_p1).Length();
    } // End: SquaredDistance()
  }
}

#endif