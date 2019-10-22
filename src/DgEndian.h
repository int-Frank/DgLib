#ifndef DGENDIAN_H
#define DGENDIAN_H

namespace Dg
{
  enum class Endian
  {
    ENDIAN_UNKNOWN,
    ENDIAN_BIG,
    ENDIAN_LITTLE,
    ENDIAN_BIG_WORD,   /* Middle-endian, Honeywell 316 style */
    ENDIAN_LITTLE_WORD /* Middle-endian, PDP-11 style */
  };

  Endian Endianness();
}

#endif