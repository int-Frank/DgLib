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

  int8_t EndianConverter::Convert_s8(int8_t a_val) const
  {
    return a_val;
  }

  uint8_t EndianConverter::Convert_u8(uint8_t a_val) const
  {
    return a_val;
  }

  int16_t EndianConverter::Convert_s16(int16_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<int16_t>(a_val);
    return a_val;
  }

  uint16_t EndianConverter::Convert_u16(uint16_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<uint16_t>(a_val);
    return a_val;
  }

  int32_t EndianConverter::Convert_s32(int32_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<int32_t>(a_val);
    return a_val;
  }

  uint32_t EndianConverter::Convert_u32(uint32_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<uint32_t>(a_val);
    return a_val;
  }

  int64_t EndianConverter::Convert_s64(int64_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<int64_t>(a_val);
    return a_val;
  }

  uint64_t EndianConverter::Convert_u64(uint64_t a_val) const
  {
    if (m_reverse)
      return ReverseBytes<uint64_t>(a_val);
    return a_val;
  }

  float EndianConverter::Convert_f32(float a_val) const
  {
    if (m_reverse)
      return ReverseBytes<float>(a_val);
    return a_val;
  }

  double EndianConverter::Convert_f64(double a_val) const
  {
    if (m_reverse)
      return ReverseBytes<double>(a_val);
    return a_val;
  }
}