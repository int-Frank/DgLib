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
    static float const SQRTPI;
    static float const INVPI;
    static float const EPSILON;
    static float const SQRT2;
    static float const INVSQRT2;
  };

  template<>
  struct Constants<double>
  {
    static double const PI;
    static double const SQRTPI;
    static double const INVPI;
    static double const EPSILON;
    static double const SQRT2;
    static double const INVSQRT2;
  };


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
      if (a_val % i == 0 || a_val % (i + 2) == 0)
        return false;
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
      val |= (val >> (1 << i));
    val ^= (val >> 1);
    return val;
  }

  //! Returns the next power of 2.
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
  T NextPower2(T val)
  {
    val--;
    for (size_t i = 0; i <= sizeof(T); i++)
      val |= (val >> (1 << i));
    val++;
    return val;
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

  //! Inverse error function
  template<typename Real>
  Real erfinv(Real a_x)
  {
    Real const erfinv_a3 = static_cast<Real>(-0.140543331);
    Real const erfinv_a2 = static_cast<Real>(0.914624893);
    Real const erfinv_a1 = static_cast<Real>(-1.645349621);
    Real const erfinv_a0 = static_cast<Real>(0.886226899);

    Real const erfinv_b4 = static_cast<Real>(0.012229801);
    Real const erfinv_b3 = static_cast<Real>(-0.329097515);
    Real const erfinv_b2 = static_cast<Real>(1.442710462);
    Real const erfinv_b1 = static_cast<Real>(-2.118377725);
    Real const erfinv_b0 = static_cast<Real>(1);

    Real const erfinv_c3 = static_cast<Real>(1.641345311);
    Real const erfinv_c2 = static_cast<Real>(3.429567803);
    Real const erfinv_c1 = static_cast<Real>(-1.62490649);
    Real const erfinv_c0 = static_cast<Real>(-1.970840454);

    Real const erfinv_d2 = static_cast<Real>(1.637067800);
    Real const erfinv_d1 = static_cast<Real>(3.543889200);
    Real const erfinv_d0 = static_cast<Real>(1);

    Real x2, r, y;
    Real sign_x;

    if (a_x < static_cast<Real>(-1) || a_x > static_cast<Real>(1))
      return std::numeric_limits<Real>::quiet_NaN();
  
    if (a_x == static_cast<Real>(0))
      return static_cast<Real>(0);
  
    if (a_x > static_cast<Real>(0))
      sign_x = static_cast<Real>(1);
    else 
    {
      sign_x = static_cast<Real>(-1);
      a_x = -a_x;
    }
    
    if (a_x <= static_cast<Real>(0.7)) 
    {
      x2 = a_x * a_x;
      r = a_x * (((erfinv_a3 * x2 + erfinv_a2) * x2 + erfinv_a1) * x2 + erfinv_a0);
      r /= (((erfinv_b4 * x2 + erfinv_b3) * x2 + erfinv_b2) * x2 + erfinv_b1) * x2 + erfinv_b0;
    }
    else 
    {
      y = sqrt (-log ((static_cast<Real>(1) - a_x) / static_cast<Real>(2)));
      r = (((erfinv_c3 * y + erfinv_c2) * y + erfinv_c1) * y + erfinv_c0);
      r /= ((erfinv_d2 * y + erfinv_d1) * y + erfinv_d0);
    }
    
    r = r * sign_x;
    a_x = a_x * sign_x;
    
    r -= (erf (r) - a_x) / (2 / Constants<Real>::SQRTPI * exp (-r * r));
    r -= (erf (r) - a_x) / (2 / Constants<Real>::SQRTPI * exp (-r * r));
    
    return r;
  }

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