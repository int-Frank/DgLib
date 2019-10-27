#ifndef DGENDIAN_H
#define DGENDIAN_H

#include <stdint.h>

namespace Dg
{ 
  //We won't worry about anything exotic
  enum class Endian
  {
    Big,
    Little,
    Unknown
  };

  //Determine Endianness of this machine during runtime.
  Endian Endianness();

  template<typename T>
  T ReverseBytes(T const & a_val)
  {
    T result(static_cast<T>(0));
    for (size_t i = 0; i < sizeof(T); i++)
    {
      reinterpret_cast<uint8_t*>(&result)[sizeof(T) - i - 1] =
        reinterpret_cast<uint8_t const *>(&a_val)[i];
    }
    return result;
  }

  class EndianConverter
  {
  public:

    //The default converter will not convert, but simply return the value;
    EndianConverter();

    void SetConversion(Endian const input, Endian const output);

    int8_t    Convert_s8(int8_t input)  const;
    uint8_t   Convert_u8(uint8_t input)  const;
    int16_t   Convert_s16(int16_t input) const;
    uint16_t  Convert_u16(uint16_t input) const;
    int32_t   Convert_s32(int32_t input) const;
    uint32_t  Convert_u32(uint32_t input) const;
    int64_t   Convert_s64(int64_t input) const;
    uint64_t  Convert_u64(uint64_t input) const;
    float     Convert_f32(float input) const;
    double    Convert_f64(double input) const;

  private:
  
    bool m_reverse;
  };

  
}

#endif