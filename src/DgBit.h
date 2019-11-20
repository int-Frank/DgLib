#ifndef DGBIT_H
#define DGBIT_H

#include <stdint.h>
#include <type_traits>

namespace Dg
{
  namespace impl
  {
    uint64_t const masks[65] =
    {
      0x0, 0x1, 0x3, 0x7, 0xF,
      0x1F, 0x3F, 0x7F, 0xFF,
      0x1FF, 0x3FF, 0x7FF, 0xFFF,
      0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF,
      0x1FFFF, 0x3FFFF, 0x7FFFF, 0xFFFFF,
      0x1FFFFF, 0x3FFFFF, 0x7FFFFF, 0xFFFFFF,
      0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFF,
      0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
      0x1FFFFFFFF, 0x3FFFFFFFF, 0x7FFFFFFFF, 0xFFFFFFFFF,
      0x1FFFFFFFFF, 0x3FFFFFFFFF, 0x7FFFFFFFFF, 0xFFFFFFFFFF,
      0x1FFFFFFFFFF, 0x3FFFFFFFFFF, 0x7FFFFFFFFFF, 0xFFFFFFFFFFF,
      0x1FFFFFFFFFFF, 0x3FFFFFFFFFFF, 0x7FFFFFFFFFFF, 0xFFFFFFFFFFFF,
      0x1FFFFFFFFFFFF, 0x3FFFFFFFFFFFF, 0x7FFFFFFFFFFFF, 0xFFFFFFFFFFFFF,
      0x1FFFFFFFFFFFFF, 0x3FFFFFFFFFFFFF, 0x7FFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF,
      0x1FFFFFFFFFFFFFF, 0x3FFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFF,
      0x1FFFFFFFFFFFFFFF, 0x3FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    };
  }

  template<typename INT, INT SHIFT>
  struct BIT
  {
    static_assert(SHIFT < (sizeof(INT) * CHAR_BIT), "Bit shift cannot be more than the size of the int!");
    static INT const value = static_cast<INT>(1) << SHIFT;
  };

  template<typename INT>
  INT Bit(INT a_val)
  {
    return static_cast<INT>(1) << a_val;
  }

  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  struct Mask
  {
    static INT const value = BIT<INT, (BEGIN + COUNT - 1)>::value | Mask<INT, BEGIN, COUNT - 1>::value;
  };

  template<typename INT, int BEGIN>
  struct Mask<INT, BEGIN, 1>
  {
    static INT const value = BIT<INT, BEGIN>::value;
  };

  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT GetSubInt(INT a_val)
  {
    return (a_val & Mask<INT, BEGIN, COUNT>::value) >> BEGIN;
  };

  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT SetSubInt(INT a_in, INT a_val)
  {
    INT cleared = a_in & ~Mask<INT, BEGIN, COUNT>::value;
    INT val = (a_val << BEGIN) & Mask<INT, BEGIN, COUNT>::value;
    return cleared | val;
  };

  template<typename INT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT GetMask(INT a_bitBegin, INT a_bitCount)
  {
    return static_cast<INT>(impl::masks[a_bitCount]) << a_bitBegin;
  }

  template<typename INT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT GetSubInt(INT a_in, INT a_bitBegin, INT a_bitCount)
  {
    return (a_in & GetMask<INT>(a_bitBegin, a_bitCount)) >> a_bitBegin; 
  }

  template<typename INT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT SetSubInt(INT a_in, INT a_val, INT a_bitBegin, INT a_bitCount)
  {
    INT mask = GetMask<INT>(a_bitBegin, a_bitCount);
    INT cleared = a_in & ~mask;
    INT val = (a_val << a_bitBegin) & mask;
    return cleared | val;
  }

  //! Finds the highest bit
  template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  T HighestBit(T val)
  {
    if (!val)
      return 0;

    T ret = 1;

    while (val >>= 1)
      ret++;

    return ret;
  }

}

#endif