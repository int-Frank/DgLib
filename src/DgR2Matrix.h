//! @file DgR2Matrix.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGR2MATRIX_H
#define DGR2MATRIX_H

#include "impl/DgR2Matrix_generic.h"

//--------------------------------------------------------------------------------
//	@	Matrix
//--------------------------------------------------------------------------------
namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Matrix = Dg::impl::Matrix_generic<Real, 2>;
  }
}
#endif