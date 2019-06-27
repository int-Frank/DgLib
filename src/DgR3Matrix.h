//! @file DgR3Matrix.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGR3MATRIX_H
#define DGR3MATRIX_H

#include "impl/DgR3Matrix_generic.h"
#include "DgR3Vector.h"
#include "DgR3Quaternion.h"

//--------------------------------------------------------------------------------
//	@	Matrix
//--------------------------------------------------------------------------------
namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Matrix = Dg::impl::Matrix_generic<Real, 3>;
  }
}
#endif