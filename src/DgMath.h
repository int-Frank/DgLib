//! @file DgMath.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! This files contains a variety of useful math functions and constants.
#ifndef DGMATH_H
#define DGMATH_H

#include <cmath>
#include <stdint.h>
#include <limits>

#include "DgTypes.h"
#include "impl/DgMath_impl.h"

namespace Dg
{
	
  //! @ingroup DgMath_constants
  //! @{

  enum class EulerOrder
  {
    XYZ,
    XZY,
    YXZ,
    YZX,
    ZYX,
    ZXY,
    XYX,
    XZX,
    YXY,
    YZY,
    ZYZ,
    ZXZ
  };

  enum class Orientation
  {
    CW,
    CCW,
    Colinear,
    Coplanar
  };

  template<typename Real>
  struct Constants
  {};

  template<>
  struct Constants<float>
  {
    static float const PI;
    static float const INVPI;
    static float const EPSILON;
    static float const SQRT2;
    static float const INVSQRT2;
  };

  template<>
  struct Constants<double>
  {
    static double const PI;
    static double const INVPI;
    static double const EPSILON;
    static double const SQRT2;
    static double const INVSQRT2;
  };

  unsigned const N_C_INVERF   = 512;

  //! @}
  
  //! @ingroup DgMath_functions
  //! @{

  //! Test to see if a number is prime
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  bool IsPrime(T a_val)
  {
    if (a_val <= 1) return false;
    if (a_val <= 3) return true;
    if (a_val % 2 == 0 || a_val % 3 == 0) return false;

    T maxi = (T(1) << ((CHAR_BIT * sizeof(T)) / 2)) - 1; //Root of the highest square T can hold before overflow.
    for (T i = 5; i*i <= a_val && i <= maxi; i += 6)
    {
      if (a_val % i == 0 || a_val % (i + 2) == 0)
      {
        return false;
      }
    }
    return true;
  }

  //! Finds log base 2 of a 32-bit integer.
  uint32_t Log2_32(uint32_t);

  //! Finds log base 2 of a 64-bit integer.
  uint64_t Log2_64(uint64_t);

  //! Floors a number to the nearest power of 2.
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  T FloorPower2(T val)
  {
    for (size_t i = 0; i <= sizeof(T); i++)
    {
      val |= (val >> (1 << i));
    }
    val ^= (val >> 1);
    return val;
  }

  //! Returns the next power of 2.
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  T NextPower2(T val)
  {
    val--;
    for (size_t i = 0; i <= sizeof(T); i++)
    {
      val |= (val >> (1 << i));
    }
    val++;
    return val;
  }

  //! Finds the highest bit
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  T HighestBit(T val)
  {
    if (!val)
      return 0;

    T ret = 1;

    while (val >>= 1)
      ret++;

    return ret;
  }

  //! Computes 2 numbers which are closest to sprt(val) but when
  //! multiplied together are >= val
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  void ClosestSquare(T val, T & lower,  T & upper)
  {
    lower = static_cast<T>(round(sqrt(static_cast<double>(val))));

    if (lower * lower >= val)
      upper = lower;
    else
      upper = lower + 1;
  }

  //! Inverse error function. Uses Mclaurin series expansion approximation.
  //! The template param nTerms represents the number of terms in the series expansion to use.
  //!
  //! @return Result
  //!
  //! @param a_x Input: -1 < x < 1
  template<typename Real, unsigned nTerms = 16>
  Real inverf(Real a_x)
  {
	  static_assert(nTerms <= N_C_INVERF, "Max terms for the inverf is 512");

	  if (a_x < static_cast<Real>(-1.0) || a_x > static_cast<Real>(1.0))
		  return static_cast<Real>(0.0);

	  Real x0Sq = a_x * a_x;
	  Real x = a_x;
	  Real result = static_cast<Real>(0.0);
	  
	  for (unsigned i = 0; i < nTerms; i++)
	  {
		  result += x * static_cast<Real>(impl::C_INVERF[i]);
		  x *= x0Sq;
	  }

	  return result;
  }

  //! Set bits within an integer type.
  template<typename T, unsigned Position, unsigned Length, typename = std::enable_if<std::is_integral<T>::value>>
  T SetBitSet(T a_input, T a_value)
  {
    T mask = (T(1) << (Length + 1)) - 1;
    a_value &= mask;
    mask <<= Position;
    mask = ~mask;
    a_input &= mask;
    a_input |= (a_value << Position);
    return a_input;
  }// End: SetBitSet()


  //! Wrap a number to a range.
  template<typename Real>
  void WrapNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower || val > upper)
    {
      Real range = upper - lower;
      val = val - range * Real(floor((val - lower) / range));
    }

  }	//End: WrapNum()


  //! Tests number to see if in range
  template<typename Real>
  bool IsInRange(Real lower, Real upper, Real val)
  {
    return (val >= lower && val <= upper);
  }	//End: WrapNum()


  //! Wraps val to [-PI, PI].
  template<typename Real>
  void WrapAngle(Real& val)
  {
    val = val - static_cast<Real>(Constants<Real>::PI * 2.0)*floor((val + Constants<Real>::PI) * (Constants<Real>::INVPI * 0.5));
  }	//End: WrapAngle()


  //! Clamp a number to a range.
  template<typename Real>
  void ClampNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower)
      val = lower;
    else if (val > upper)
      val = upper;

  }	//End: ClampNumber()


  //! Is the argument close to zero?
  template<typename Real>
  bool IsZero(Real a)
  {
    return (a > -Dg::Constants<Real>::EPSILON && a < Dg::Constants<Real>::EPSILON);
  }	//End: IsZero()


  //! Are a and b close to being equal?
  template<typename Real>
  bool AreEqual(Real a, Real b)
  {
    return (Dg::IsZero(a - b));
  }	//End: AreEqual()

  //! @}

}

#endif