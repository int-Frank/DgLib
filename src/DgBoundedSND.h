//@group Math

//! @file DgBoundedSND.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: BoundedNormal

#ifndef DGBOUNDEDSND_H
#define DGBOUNDEDSND_H

#include <math.h>

#include "DgError.h"
#include "DgMath.h"

namespace Dg
{
  //! Initiate the table of values.
  //! @param a_mean Mean
  //! @param a_sd Standard deviation
  //! @param a_lower Lower bound on the normal distribution
  //! @param a_upper Upper bound on the normal distribution
  //! @param a_nValues Number of values in the table to generate.
  //! @return ErrorCode::None on success.
  template<typename Real>
  ErrorCode GetSNDValues(Real a_mean,
                         Real a_sd,
                         Real a_lower,
                         Real a_upper,
                         size_t a_nValues,
                         Real * a_out)
  {
    if (a_lower >= a_upper || Dg::IsZero(a_sd) || a_nValues == 0)
      return ErrorCode::OutOfBounds;

    Real two = static_cast<Real>(2);
    Real one = static_cast<Real>(1);
    Real point5 = static_cast<Real>(0.5);

    Real zLower = point5 * (one + std::erf((a_lower - a_mean) / (a_sd * Dg::Constants<Real>::SQRT2)));
    Real zUpper = point5 * (one + std::erf((a_upper - a_mean) / (a_sd * Dg::Constants<Real>::SQRT2)));

    for (size_t i = 0; i < a_nValues; i++)
    {
      Real c = static_cast<Real>(zLower + (zUpper - zLower) * static_cast<Real>(i) / static_cast<Real>(a_nValues - 1));
      c = two * c - one;
      Real inverfResult = static_cast<Real>(erfinv<Real>(c));
      a_out[i] = a_sd * Dg::Constants<Real>::SQRT2 * inverfResult + a_mean;
    }

    return ErrorCode::None;
  }
}

#endif