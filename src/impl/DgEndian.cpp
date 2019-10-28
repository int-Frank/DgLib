#include <stdint.h>

#include "../DgEndian.h"

namespace Dg
{
  Endian Endianness(void)
  {
    union
    {
      uint32_t value;
      uint8_t data[sizeof(uint32_t)];
    } number;

    number.data[0] = 0x00;
    number.data[1] = 0x01;
    number.data[2] = 0x02;
    number.data[3] = 0x03;

    switch (number.value)
    {
      case UINT32_C(0x00010203): return Endian::Big;
      case UINT32_C(0x03020100): return Endian::Little;
      default:                   return Endian::Unknown;
    }
  }

  EndianConverter::EndianConverter()
    : m_reverse(false)
  {
  
  }

  void EndianConverter::SetConversion(Endian const a_input, Endian const a_output)
  {
    if (a_input == Endian::Unknown
     || a_output == Endian::Unknown
     || a_input == a_output)
      m_reverse = false;
    else
      m_reverse = true;
  }
}