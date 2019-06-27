//! @file DgBoundedSND.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: BoundedNormal

#ifndef DGBOUNDEDSND_H
#define DGBOUNDEDSND_H

#include <math.h>

#include "DgTypes.h"
#include "DgMath.h"

//TODO remove dependancy to RNG. Get() should be Get(size_t index), where
//     index is a number between 0 and a_nValues. Better yet, this should 
//     just be a function which fills a pre-allocated table with values.
namespace Dg
{
  //! Initiate the table of values.
  //! @param a_mean Mean
  //! @param a_sd Standard deviation
  //! @param a_lower Lower bound on the normal distribution
  //! @param a_upper Upper bound on the normal distribution
  //! @param a_nValues Number of values in the table to generate.
  //! @return Err_None on success.
  template<typename Real>
  ErrorCode Init(Real a_mean,
                 Real a_sd,
                 Real a_lower,
                 Real a_upper,
                 unsigned int a_nValues,
                 Real * a_out)
  {
    Clean();

    //Check input
    if (a_lower >= a_upper || Dg::IsZero(a_sd) || a_nValues == 0)
      return Err_OutOfBounds;

    Real zLower = 0.5 * (1.0 + std::erf((a_lower - a_mean) / (a_sd * Dg::Constants<Real>::SQRT2)));
    Real zUpper = 0.5 * (1.0 + std::erf((a_upper - a_mean) / (a_sd * Dg::Constants<Real>::SQRT2)));

    m_nValues = a_nValues;
    m_values = new Real[m_nValues];

    for (unsigned i = 0; i < a_nValues; i++)
    {
      double c = static_cast<double>(zLower + (zUpper - zLower) * static_cast<double>(i) / static_cast<double>(m_nValues - 1));
      c = 2.0 * c - 1.0;
      Real inverfResult = static_cast<Real>(inverf<double, N_C_INVERF>(c));
      a_out[i] = a_sd * Dg::Constants<Real>::SQRT2 * inverfResult + a_mean;
    }

    return Err_None;
  }
}

#endif