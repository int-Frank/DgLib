#ifndef DGBIT_H
#define DGBIT_H

#include <stdint.h>
#include <type_traits>

namespace Dg
{
  namespace impl
  {
    extern uint64_t const masks[65];
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
  template<typename T, typename = std::enable_if<std::is_integral<T>::value>::type>
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