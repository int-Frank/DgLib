//! @file DgFixedPoint.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: FixedPoint

/*******************************************************************************/
/*                                                                             */
/*  Copyright (c) 2007-2009: Peter Schregle,                                   */
/*  All rights reserved.                                                       */
/*                                                                             */
/*  This file is part of the Fixed Point Math Library.                        */
/*                                                                             */
/*  Redistribution of the Fixed Point Math Library and use in source and      */
/*  binary forms, with or without modification, are permitted provided that    */
/*  the following conditions are met:                                          */
/*  1. Redistributions of source code must retain the above copyright notice,  */
/*     this list of conditions and the following disclaimer.                   */
/*  2. Redistributions in binary form must reproduce the above copyright       */
/*     notice, this list of conditions and the following disclaimer in the     */
/*     documentation and/or other materials provided with the distribution.    */
/*  3. Neither the name of Peter Schregle nor the names of other contributors  */
/*     may be used to endorse or promote products derived from this software   */
/*     without specific prior written permission.                              */
/*                                                                             */
/*  THIS SOFTWARE IS PROVIDED BY PETER SCHREGLE AND CONTRIBUTORS 'AS IS' AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE */
/*  ARE DISCLAIMED. IN NO EVENT SHALL PETER SCHREGLE OR CONTRIBUTORS BE LIABLE */
/*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    */
/*  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      */
/*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,        */
/*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN   */
/*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            */
/*  POSSIBILITY OF SUCH DAMAGE.                                                */
/*                                                                             */
/*******************************************************************************/

#ifndef DGFIXEDPOINT_H
#define DGFIXEDPOINT_H

#include <stdint.h>
#include <limits>

namespace Dg
{
  template<typename I, uint8_t F>
  class FixedPoint;
}

template<typename I, uint8_t F> Dg::FixedPoint<I, F> abs(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> ceil(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> floor(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> mod(Dg::FixedPoint<I, F>, Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> modf(Dg::FixedPoint<I, F>, Dg::FixedPoint<I, F> *);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> exp(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> log(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> cos(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> sin(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> tan(Dg::FixedPoint<I, F>);
template<typename I, uint8_t F> Dg::FixedPoint<I, F> sqrt(Dg::FixedPoint<I, F>);

namespace Dg
{
  namespace impl
  {
    struct Error_promote_type_not_specialized_for_this_type
    { 
    };

    //! When two 8 bit numbers are multiplied, a 16 bit result is produced.
    //! When two 16 bit numbers are multiplied, a 32 bit result is produced.
    //! When two 32 bit numbers are multiplied, a 64 bit result is produced.
    //! Since the fixed_point class internally relies on integer 
    //! multiplication, we need type promotion. After the multiplication we
    //! need to adjust the position of the decimal point by shifting the
    //! temporary result to the right an appropriate number of bits. 
    //! However, if the temporary multiplication result is not kept in a big
    //! enough variable, overflow errors will occur and lead to wrong 
    //! results. A similar promotion needs to be done to the divisor in the
    //! case of division, but here the divisor needs to be shifted to the
    //! left an appropriate number of bits.
    //!
    //! Unfortunately the integral_promotion class of the boost type_traits
    //! library could not be used, since it does not provide a promotion
    //! from int/unsigned int (32 bit) to long long/unsigned long long 
    //! (64 bit). However, this promotion is often needed, because it is 
    //! quite common to use a 32 bit base type for the fixed_point type.
    //!
    //! Therefore, the Fixed Point Math Library defines its own promotions 
    //! here in a set of private classes.
    template<typename I, uint8_t F>
    struct QueryFPType 
    { 
      static bool const valid = false;
      static int const nIntegerBits = 0;
      typedef Error_promote_type_not_specialized_for_this_type PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int8_t, F>
    {
      static bool const valid = (F <= 7);
      static int const nIntegerBits = 7 - F;
      typedef int16_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint8_t, F>
    {
      static bool const valid = (F <= 8);
      static int const nIntegerBits = 8 - F;
      typedef uint16_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int16_t, F>
    {
      static bool const valid = (F <= 15);
      static int const nIntegerBits = 15 - F;
      typedef int32_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint16_t, F>
    {
      static bool const valid = (F <= 16);
      static int const nIntegerBits = 16 - F;
      typedef uint32_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int32_t, F>
    {
      static bool const valid = (F <= 31);
      static int const nIntegerBits = 31 - F;
      typedef int64_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint32_t, F>
    {
      static bool const valid = (F <= 32);
      static int const nIntegerBits = 32 - F;
      typedef uint64_t PromoteType;
    };

    //! We use this table to get around the compiler throwing a warning 
    //! when bit shifting by a potentially negative number.
    uint32_t const Shifts[32] =
    {
      0,  1,  2,  3,  4,  5,  6,  7,
      8,  9,  10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23,
      24, 25, 26, 27, 28, 29, 30, 31
    };

    uint32_t const fBitMasks[33] =
    {
      0, 0x1, 0x3, 0x7, 0xF,
      0x1F, 0x3F, 0x7F, 0xFF,
      0x1FF, 0x3FF, 0x7FF, 0xFFF,
      0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF,
      0x1FFFF, 0x3FFFF, 0x7FFFF, 0xFFFFF,
      0x1FFFFF, 0x3FFFFF, 0x7FFFFF, 0xFFFFFF,
      0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFF,
      0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
    };
  }

  template<
    /// The base type. Must be an integer type. 
    //!
    //! If this is a signed type, the fixed_point number will behave signed too, 
    //! if this is an unsigned type, the fixed_point number will behave 
    //! unsigned.
    typename I, 
    /// Number of fraction bits. Must be between 0 and sizeof (I).
    uint8_t F>
  //! This type is designed to be a plug-in type to replace the floating point
  //! types, such as float, double and long double. While it doesn't offer the
  //! precision of these types, its operations are all implemented in integer
  //! math, and it is therefore hoped that these operations are faster on non-
  //! floating-point enabled hardware.
  //!
  //! The value uses 0/1 bits for the sign, I bits for the integer part and F bits
  //! for the fractional part.
  //!
  //! Here is an example: a signed 8 bit 5:2 fixed_point type would have the 
  //! following layout:
  //!
  //! fixed_point<signed char, 2>
  //!
  //!  sign           integer part \ / fractional part
  //!	  |                           |
  //! +----+----+----+----+----+----+----+----+
  //! | S  | I4 | I3 | I2 | I1 | I0 | F0 | F1 |
  //! +----+----+----+----+----+----+----+----+
  //!
  //! where S is the sign-bit, I0 to I4 is the integer part, and F0 to F1 is
  //! the fractional part. The range of this type is from -32 to +31.75, the 
  //! fractional part can encode multiples of 0.25.
  //!
  //! Note: This is a modified version of the fixed point library from Peter Schregle.
  //! Retrieved from http://www.codeproject.com/Articles/37636/Fixed-Point-Class
  class FixedPoint
  {
    static_assert(impl::QueryFPType<I, F>::valid, "Invalid template argument. Check input type and number of fractional bits.");
    
    /// Grant the fixed_point template access to private members. Types with
    /// different template parameters are different types and without this
    /// declaration they do not have access to private members.
    friend class Dg::FixedPoint;

    /// Grant the numeric_limits specialization for this fixed_point class 
    /// access to private members.
    friend class std::numeric_limits<Dg::FixedPoint<I, F> >;

  public:

    /// The base type of this fixed_point class.
    typedef I base_type;

    //! Just as with built-in types no initialization is done. The value is
    //! undetermined after executing this constructor.
    FixedPoint() {}
    ~FixedPoint() {}

    //! This constructor takes a numeric value of type float and converts it to 
    //! this fixed_point type.
    //!
    //! The conversion is done by multiplication with 2^F and rounding to the 
    //! next integer.
    FixedPoint(float a_val)       : m_val(static_cast<I>(a_val * static_cast<float>(Power2<F>::value) + (a_val >= 0.0f ? 0.5f : -0.5f))) {}
    
    //! This constructor takes a numeric value of type double and converts it to 
    //! this fixed_point type.
    //!
    //! The conversion is done by multiplication with 2^F and rounding to the 
    //! next integer.
    FixedPoint(double a_val)      : m_val(static_cast<I>(a_val * static_cast<double>(Power2<F>::value) + (a_val >= 0.0 ? 0.5 : -0.5))) {}
    
    //! This constructor takes a numeric value of type long double and converts it to 
    //! this fixed_point type.
    //!
    //! The conversion is done by multiplication with 2^F and rounding to the 
    //! next integer.
    FixedPoint(long double a_val) : m_val(static_cast<I>(a_val * static_cast<long double>(Power2<F>::value) + (a_val >= 0.0L ? 0.5L : -0.5L))) {}

    //! This constructor takes a numeric value of type bool and converts it to this fixed_point type.
    FixedPoint(bool a_val)     : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type int8_t and converts it to this fixed_point type.
    FixedPoint(int8_t a_val)   : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type uint8_t and converts it to this fixed_point type.
    FixedPoint(uint8_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type int16_t and converts it to this fixed_point type.
    FixedPoint(int16_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type uint16_t and converts it to this fixed_point type.
    FixedPoint(uint16_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type int32_t and converts it to this fixed_point type.
    FixedPoint(int32_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type uint32_t and converts it to this fixed_point type.
    FixedPoint(uint32_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type int64_t and converts it to this fixed_point type.
    FixedPoint(int64_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! This constructor takes a numeric value of type uint64_t and converts it to this fixed_point type.
    FixedPoint(uint64_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! Constructor from another fixed point type.
    template<typename I2, uint8_t F2>
    explicit operator FixedPoint<I2, F2>() const;

    //! Copy constructor
    FixedPoint(FixedPoint const &);

    //! Assignment
    FixedPoint & operator=(FixedPoint const &);

    //! @return The value converted to float
    operator float()        const { return static_cast<float>(m_val) / Power2<F>::value; }
    
    //! @return The value converted to double
    operator double()       const { return static_cast<double>(m_val) / Power2<F>::value; }
    
    //! @return The value converted to long double
    operator long double()  const { return static_cast<long double>(m_val) / Power2<F>::value; }

    //! @return The value converted to bool
    operator bool()         const { return m_val != 0; }
    
    //! @return The value converted to int8_t
    operator int8_t()       const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int8_t>(m_val >> F); }
    
    //! @return The value converted to uint8_t
    operator uint8_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint8_t>(m_val >> F); }
    
    //! @return The value converted to int16_t
    operator int16_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int16_t>(m_val >> F); }
    
    //! @return The value converted to uint16_t
    operator uint16_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint16_t>(m_val >> F); }
    
    //! @return The value converted to int32_t
    operator int32_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int32_t>(m_val >> F); }
    
    //! @return The value converted to uint32_t
    operator uint32_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint32_t>(m_val >> F); }
    
    //! @return The value converted to int64_t
    operator int64_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int64_t>(m_val >> F); }
    
    //! @return The value converted to uint64_t
    operator uint64_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint64_t>(m_val >> F); }

    bool operator==(FixedPoint const & a_other) const {return m_val == a_other.m_val;}
    bool operator!=(FixedPoint const & a_other) const {return m_val != a_other.m_val;}
    bool operator> (FixedPoint const & a_other) const {return m_val > a_other.m_val;}
    bool operator< (FixedPoint const & a_other) const {return m_val < a_other.m_val;}
    bool operator>=(FixedPoint const & a_other) const {return m_val >= a_other.m_val;}
    bool operator<=(FixedPoint const & a_other) const {return m_val <= a_other.m_val;}            

    bool operator !() const { return m_val == 0; }

    //! For signed fixed-point types you can apply the unary minus operator to 
    //! get the additive inverse. For unsigned fixed-point types, this operation 
    //! is undefined. Also, shared with the integer base type B, the minimum 
    //! value representable by the type cannot be inverted, since it would yield 
    //! a positive value that is out of range and cannot be represented.
    //!
    //! /return The negative value.
    FixedPoint operator-() const;

    FixedPoint operator++(int);
    FixedPoint & operator++();
    FixedPoint operator--(int);
    FixedPoint & operator--();

    FixedPoint operator<<(int) const;
    FixedPoint operator>>(int) const;
    FixedPoint & operator<<=(int);
    FixedPoint & operator>>=(int);

    FixedPoint operator+(FixedPoint) const;
    FixedPoint operator-(FixedPoint) const;
    FixedPoint operator*(FixedPoint) const;
    FixedPoint operator/(FixedPoint) const;

    FixedPoint & operator+=(FixedPoint);
    FixedPoint & operator-=(FixedPoint);
    FixedPoint & operator*=(FixedPoint);
    FixedPoint & operator/=(FixedPoint);

    friend FixedPoint (::abs<I, F>)(FixedPoint);
    friend FixedPoint (::ceil<I, F>)(FixedPoint);
    friend FixedPoint (::floor<I, F>)(FixedPoint);
    friend FixedPoint (::mod<I, F>)(FixedPoint, FixedPoint);
    friend FixedPoint (::modf<I, F>)(FixedPoint, FixedPoint *);
    friend FixedPoint (::exp<I, F>)(FixedPoint);
    friend FixedPoint (::log<I, F>)(FixedPoint);
    friend FixedPoint (::cos<I, F>)(FixedPoint);
    friend FixedPoint (::sin<I, F>)(FixedPoint);
    friend FixedPoint (::tan<I, F>)(FixedPoint);
    friend FixedPoint (::sqrt<I, F>)(FixedPoint);

  private:

    //! This constructor takes a value of type B and initializes the internal
    //! representation of fixed_point<B, I, F> with it.
    //!
    //! @param[in] a_val Value to initialize to.
    //! @param[in] DUMMY This value is not important, it's just here to differentiate from
    //!                  the other constructors that convert its values.
    FixedPoint(I a_val, bool DUMMY) : m_val(a_val) {}
    
    //! The fixed_point class needs 2 to the power of P in several locations in
    //! the code. However, the value depends on compile time constants only and
    //! can therefore be calculated at compile time using this template 
    //! trickery. There is no need to call the function pow(2., P) at runtime to
    //! calculate this value.
    //!
    //! The value is calculated by recursively instantiating the power2 template
    //! with successively decrementing P. Finally, 2 to the power of 0 is
    //! terminating the recursion and set to 1.
    template<
      /// The power.
      int E>
    struct Power2
    {
      static uint64_t const value = Power2<E - 1>::value * 2;
    };

    //! The fixed_point class needs 2 to the power of P in several locations in
    //! the code. However, the value depends on compile time constants only and
    //! can therefore be calculated at compile time using this template 
    //! trickery. There is no need to call the function pow(2., P) at runtime to
    //! calculate this value.
    //!
    //! The value is calculated by recursively instantiating the power2 template
    //! with successively decrementing P. Finally, 2 to the power of 0 is
    //! terminating the recursion and set to 1.
    template<>
    struct Power2<0>
    {
      static uint64_t const value = 1;
    };

  private:
    I       m_val;
  };

  template<typename I, uint8_t F>
  FixedPoint<I, F>::FixedPoint(FixedPoint<I, F> const & a_other)
    : m_val(a_other.m_val)
  {

  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator=(FixedPoint<I, F> const & a_other)
  {
    m_val = a_other.m_val;
    return *this;
  }

  
  template<typename I, uint8_t F>
  template<typename I2, uint8_t F2>
  FixedPoint<I, F>::operator FixedPoint<I2, F2>() const
  {
    if (sizeof(I2) >= sizeof(I))
    {
      if (F2 > F)
      {
        int const diff = F2 - F;
        return FixedPoint<I2, F2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : static_cast<I2>(m_val) << impl::Shifts[diff], true);
      }
      else
      {
        int const diff = F - F2;
        return FixedPoint<I2, F2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : static_cast<I2>(m_val) >> impl::Shifts[diff], true);
      }
    }
    else
    {
      if (F2 > F)
      {
        int const diff = F2 - F;
        return FixedPoint<I2, F2>(static_cast<I2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : m_val << impl::Shifts[diff]), true);
      }
      else
      {
        int const diff = F - F2;
        return FixedPoint<I2, F2>(static_cast<I2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : m_val >> impl::Shifts[diff]), true);
      }
    }
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-() const
  {
    //Negating an unsigned FixedPoint has undefined behaviour.
    return FixedPoint<I, F>(-m_val, true);
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator++()
  {
    m_val += Power2<F>::value;
    return *this;
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator++(int)
  {
    FixedPoint<I, F> result(m_val, true);
    m_val += Power2<F>::value;
    return result;
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator--()
  {
    m_val -= Power2<F>::value;
    return *this;
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator--(int)
  {
    FixedPoint<I, F> result(m_val, true);
    m_val -= Power2<F>::value;
    return result;
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator<<(int a_shft) const
  {
    return FixedPoint<I, F>(m_val << a_shft, true);
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator>>(int a_shft) const
  {
    return FixedPoint<I, F>(m_val >> a_shft, true);
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator<<=(int a_shft)
  {
    m_val <<= a_shft;
    return *this;
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator>>=(int a_shft)
  {
    m_val >>= a_shft;
    return *this;

  }
  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator+(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val + a_rhs.m_val, true);
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val - a_rhs.m_val, true);
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator*(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>
      (
        static_cast<I>
        (
          (
            static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) * a_rhs.m_val
          ) >> F
        ), true
      );
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator/(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>
      (
        static_cast<I>
        (
          (
            (static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) << F) / a_rhs.m_val
          )
        ), true
      );
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator+=(FixedPoint<I, F> a_rhs)
  {
    m_val += a_rhs.m_val;
    return *this;
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator-=(FixedPoint<I, F> a_rhs)
  {
    m_val -= a_rhs.m_val;
    return *this;
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator*=(FixedPoint<I, F> a_rhs)
  {
    m_val = static_cast<I>
            (
              (
                static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) * a_rhs.m_val
              ) >> F
            );
    return *this;
  }
  
  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator/=(FixedPoint<I, F> a_rhs)
  {
    m_val = static_cast<I>
            (
              (
                (static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) << F)/ a_rhs.m_val
              )
            );
    return *this;
  }
}

//! A value is first input to type double and then the read value is converted
//! to type fixed_point before it is returned.
//!
//! @return A reference to this input stream.
template<typename S, typename I, uint8_t F>
S & operator >> (S & a_stream, Dg::FixedPoint<I, F> & a_val)
{
  double d(0.0);
  a_stream >> d;
  if (a_stream)
  {
    a_val = d;
  }
  return a_stream;
}

//! The fixed_point value is first converted to type double and then the output
//! operator for type double is called.
//!
//! @return A reference to this output stream.
template<typename S, typename I, uint8_t F>
S & operator << (S & a_stream, Dg::FixedPoint<I, F> a_val)
{
  doubl d = static_cast<double>(a_val);
  a_stream << d;
  return a_stream;
}

//! The abs function computes the absolute value of its argument.
//!
//! @return The absolute value of the argument.
template<typename I, uint8_t F>
Dg::FixedPoint<I, F> abs(Dg::FixedPoint<I, F> a_val)
{
  return Dg::FixedPoint<I, F>(((a_val.m_val < 0) ? -a_val.m_val : a_val.m_val), true);
}


//! The ceil function computes the smallest integral value not less than 
//! its argument.
//!
//! @return The smallest integral value not less than the argument.
template<typename I, uint8_t F>
Dg::FixedPoint<I, F> ceil(Dg::FixedPoint<I, F> a_val)
{
  I val = (a_val.m_val & ~Dg::impl::fBitMasks[F])
  + ((a_val.m_val & Dg::impl::fBitMasks[F]) ? Dg::FixedPoint<I, F>::Power2<F>::value : 0);
  return Dg::FixedPoint<I, F>(val, true);
}

//! The floor function computes the largest integral value not greater than 
//! its argument.
//!
//! @return The largest integral value not greater than the argument.
template<typename I, uint8_t F>
Dg::FixedPoint<I, F> floor(Dg::FixedPoint<I, F> a_val)
{
  I val = (a_val.m_val & ~Dg::impl::fBitMasks[F]);
  return Dg::FixedPoint<I, F>(val, true);
}


//! The mod function computes the fixed point remainder of x/y.
//!
//! /return The fixed point remainder of x/y.
template<typename I, uint8_t F>
Dg::FixedPoint<I, F> mod(Dg::FixedPoint<I, F> a_x, Dg::FixedPoint<I, F> a_y)
{
  return Dg::FixedPoint<I, F>(a_x.m_val % a_y.m_val, true);
}

/// Numerical limits
namespace std
{
  template<typename I, uint8_t F>
  class numeric_limits<Dg::FixedPoint<I, F> >
  {
  public:
    /// The FixedPoint type. This numeric_limits specialization is specialized
    /// for this type.
    typedef Dg::FixedPoint<I, F> fp_type;

    /// Tests whether a type allows denormalized values.
    //!
    //! An enumeration value of type const float_denorm_style, indicating 
    //! whether the type allows denormalized values. The FixedPoint class does
    //! not have denormalized values.
    //!
    //! The member is always set to denorm_absent.
    static const float_denorm_style has_denorm = denorm_absent;

    /// Tests whether loss of accuracy is detected as a denormalization loss 
    /// rather than as an inexact result.
    //!
    //! The FixedPoint class does not have denormalized values.
    //!
    //! The member is always set to false.
    static const bool has_denorm_loss = false;

    /// Tests whether a type has a representation for positive infinity.
    //!
    //! The FixedPoint class does not have a representation for positive
    //! infinity.
    //!
    //! The member is always set to false.
    static const bool has_infinity = false;

    /// Tests whether a type has a representation for a quiet not a number 
    /// (NAN), which is nonsignaling.
    //!
    //! The FixedPoint class does not have a quiet NAN.
    //!
    //! The member is always set to false.
    static const bool has_quiet_NaN = false;

    /// Tests whether a type has a representation for signaling not a number 
    //! (NAN).
    //!
    //! The FixedPoint class does not have a signaling NAN.
    //!
    //! The member is always set to false.
    static const bool has_signaling_NaN = false;

    /// Tests if the set of values that a type may represent is finite.
    //!
    //! The FixedPoint type has a bounded set of representable values.
    //!
    //! The member is always set to true.
    static const bool is_bounded = true;

    /// Tests if the calculations done on a type are free of rounding errors.
    //!
    //! The FixedPoint type is considered exact.
    //!
    //! The member is always set to true.
    static const bool is_exact = true;

    /// Tests if a type conforms to IEC 559 standards.
    //!
    //! The FixedPoint type does not conform to IEC 559 standards.
    //!
    //! The member is always set to false.
    static const bool is_iec559 = false;

    /// Tests if a type has an integer representation.
    //!
    //! The FixedPoint type behaves like a real number and thus has not
    //! integer representation.
    //!
    //! The member is always set to false.
    static const bool is_integer = false;

    /// Tests if a type has a modulo representation.
    //!
    //! A modulo representation is a representation where all results are 
    //! reduced modulo some value. The FixedPoint class does not have a
    //! modulo representation.
    //!
    //! The member is always set to false.
    static const bool is_modulo = false;

    /// Tests if a type has a signed representation.
    //!
    //! The member stores true for a type that has a signed representation, 
    //! which is the case for all FixedPoint types with a signed base type.
    //! Otherwise it stores false.
    static const bool is_signed = std::numeric_limits<typename fp_type::base_type>::is_signed;

    /// Tests if a type has an explicit specialization defined in the template 
    /// class numeric_limits.
    //!
    //! The FixedPoint class has an explicit specialization.
    //!
    //! The member is always set to true.
    static const bool is_specialized = true;

    /// Tests whether a type can determine that a value is too small to 
    /// represent as a normalized value before rounding it.
    //!
    //! Types that can detect tinyness were included as an option with IEC 559 
    //! floating-point representations and its implementation can affect some 
    //! results.
    //!
    //! The member is always set to false.
    static const bool tinyness_before = false;

    /// Tests whether trapping that reports on arithmetic exceptions is 
    //! implemented for a type.
    //!
    //! The member is always set to false.
    static const bool traps = false;

    /// Returns a value that describes the various methods that an 
    /// implementation can choose for rounding a real value to an integer 
    /// value.
    //!
    //! The member is always set to round_toward_zero.
    static const float_round_style round_style = round_toward_zero;

    /// Returns the number of radix digits that the type can represent without 
    /// loss of precision.
    //!
    //! The member stores the number of radix digits that the type can represent 
    //! without change.
    //!
    //! The member is set to the template parameter I (number of integer bits).
    static const int digits = I;

    /// Returns the number of decimal digits that the type can represent without 
    /// loss of precision.
    //!
    //! The member is set to the number of decimal digits that the type can 
    //! represent.
    static const int digits10 = (int)(digits * 301. / 1000. + .5);

    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int radix = 0;

    /// The minimum value of this type.
    //!
    //! /return The minimum value representable with this type.
    static fp_type(min)()
    {
      fp_type minimum;
      minimum.m_val = (std::numeric_limits<typename fp_type::base_type>::min)();
      return minimum;
    }

    /// The maximum value of this type.
    //!
    //! /return The maximum value representable with this type.
    static fp_type(max)()
    {
      fp_type maximum;
      maximum.m_val = (std::numeric_limits<typename fp_type::base_type>::max)();
      return maximum;
    }

    /// The function returns the difference between 1 and the smallest value 
    /// greater than 1 that is representable for the data type.
    //!
    //! /return The smallest effective increment from 1.0.
    static fp_type epsilon()
    {
      fp_type epsilon;
      epsilon.m_val = 1;
      return epsilon;
    }

    /// Returns the maximum rounding error for the type.
    //!
    //! The maximum rounding error for the FixedPoint type is 0.5.
    //!
    //! /return Always returns 0.5.
    static fp_type round_error()
    {
      return (fp_type)(0.5);
    }

    static fp_type denorm_min()
    {
      return (fp_type)(0);
    }

    static fp_type infinity()
    {
      return (fp_type)(0);
    }

    static fp_type quiet_NaN()
    {
      return (fp_type)(0);
    }

    static fp_type signaling_NaN()
    {
      return (fp_type)(0);
    }
  };

}

#endif