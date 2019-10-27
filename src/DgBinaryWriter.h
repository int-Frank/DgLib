#ifndef DGBINARYWRITER_H
#define DGBINARYWRITER_H

#include <stdint.h>
#include <string>

#include "DgEndian.h"
#include "DgStream.h"

namespace Dg
{
  //Bytes will be interpreted in little endian format by default.
  //The BinaryWriter does not own the input stream.
  class BinaryWriter
  {
    typedef Stream::byte byte;
    typedef Stream::myInt myInt;

  public:

    BinaryWriter(EndianConverter const = EndianConverter());
    BinaryWriter(Stream *, EndianConverter const = EndianConverter());
    ~BinaryWriter();

    BinaryWriter(BinaryWriter &) = delete;
    BinaryWriter & operator=(BinaryWriter &) = delete;

    BinaryWriter(BinaryWriter &&);
    BinaryWriter & operator=(BinaryWriter &&);

    void SetEndianConverter(EndianConverter const);

    ErrorCode Open(Stream *);
    bool IsOpen() const;
    void Close();

    myInt Write_s8(int8_t const);
    myInt Write_u8(uint8_t const);
    myInt Write_s16(int16_t const);
    myInt Write_u16(uint16_t const);
    myInt Write_s32(int32_t const);
    myInt Write_u32(uint32_t const);
    myInt Write_s64(int64_t const);
    myInt Write_u64(uint64_t const);
    myInt Write_f32(float const);
    myInt Write_f64(double const);
    myInt Write_string(std::string const &);
    myInt Write(void *, myInt const count);

    // Returns a new position or a negative value on error. On 
    // success, the position will be clamped to [0, size]
    myInt Seek(myInt const offset, StreamSeekOrigin const origin);

    // Skips a number of bytes forward if count is positive or
    // backward otherwise.
    // Returns a negative value on error.
    myInt Skip(myInt const);

    // Returns a current position or a negative value on error.
    myInt GetPosition();

    // Moves stream's pointer to a new position.
    // Returns position on success.
    // Returns a negative value on error.
    myInt SetPosition(myInt const position);

  private:

    template<typename T>
    myInt Write(T const a_val)
    {
      if (m_pStream == nullptr)
        return -Err_NullObject;

      return m_pStream->Write(&a_val, static_cast<Stream::myInt>(sizeof(T)));
    }

  private:

    EndianConverter m_endianConverter;
    Stream *        m_pStream;
  };


}

#endif