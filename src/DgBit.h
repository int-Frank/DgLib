#ifndef DGBIT_H
#define DGBIT_H

#include <type_traits>

#define BIT(x) (1 << (x))

namespace Dg
{
  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  struct Mask
  {
    static INT const value = BIT(BEGIN + COUNT - 1) | Mask<INT, BEGIN, COUNT - 1>::value;
  };

  template<typename INT, int BEGIN>
  struct Mask<INT, BEGIN, 1>
  {
    static INT const value = BIT(BEGIN);
  };

  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT GetSubInt(INT a_val)
  {
    return (a_val & Mask<INT, BEGIN, COUNT>::value) >> BEGIN;
  };

  template<typename INT, unsigned BEGIN, unsigned COUNT, typename = std::enable_if_t<std::is_integral<INT>::value>>
  INT SetSubInt(INT a_int, INT a_val)
  {
    INT cleared = a_int & ~Mask<INT, BEGIN, COUNT>::value;
    INT val = (a_val << BEGIN) & Mask<INT, BEGIN, COUNT>::value;
    return cleared | val;
  };

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