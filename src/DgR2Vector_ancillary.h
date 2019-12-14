//@group Math/R2

//! @file DgR2Vector_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGR2VECTOR3_ANCILLARY_H
#define DGR2VECTOR3_ANCILLARY_H

#include "DgR2Vector.h"
#include "impl/DgRNG_Base.h"

namespace Dg
{
  namespace R2
  {
    //-------------------------------------------------------------------------------
    //		@	GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomVector(RNG_Base & a_rRNG)
    {
      Real theta = a_rRNG.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * Dg::Constants<Real>::PI);

      Real x = cos(theta);
      Real y = sin(theta);

      return Vector<Real>({ x, y, static_cast<Real>(0.0) });
    }	//End: GetRandomVector()
  }
}

#endif