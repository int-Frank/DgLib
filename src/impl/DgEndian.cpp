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
      case UINT32_C(0x00010203): return Endian::ENDIAN_BIG;
      case UINT32_C(0x03020100): return Endian::ENDIAN_LITTLE;
      case UINT32_C(0x02030001): return Endian::ENDIAN_BIG_WORD;
      case UINT32_C(0x01000302): return Endian::ENDIAN_LITTLE_WORD;
      default:                   return Endian::ENDIAN_UNKNOWN;
    }
  }
}