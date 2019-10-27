#ifndef DGBINARYREADER_H
#define DGBINARYREADER_H

#include <stdint.h>
#include <string>

#include "DgEndian.h"
#include "DgStream.h"

namespace Dg
{
  //Bytes will be interpreted in little endian format by default.
  //The BinaryReader does not own the input stream.
  class BinaryReader
  {
    typedef Stream::byte byte;
    typedef Stream::myInt myInt;

  public:

    BinaryReader(EndianConverter const = EndianConverter());
    BinaryReader(Stream *, EndianConverter const = EndianConverter());
    ~BinaryReader();

    BinaryReader(BinaryReader &) = delete;
    BinaryReader & operator=(BinaryReader &) = delete;

    BinaryReader(BinaryReader &&) noexcept;
    BinaryReader & operator=(BinaryReader &&) noexcept;

    void SetEndianConverter(EndianConverter const);

    ErrorCode Open(Stream *);
    bool IsOpen() const;
    void Close();

    ErrorCode Read_s8(int8_t &);
    ErrorCode Read_u8(uint8_t &);
    ErrorCode Read_s16(int16_t &);
    ErrorCode Read_u16(uint16_t &);
    ErrorCode Read_s32(int32_t &);
    ErrorCode Read_u32(uint32_t &);
    ErrorCode Read_s64(int64_t &);
    ErrorCode Read_u64(uint64_t &);
    ErrorCode Read_f32(float &);
    ErrorCode Read_f64(double &);
    myInt     Read_string(myInt const length, std::string &);
    myInt     Read(void *, myInt const count);

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
    ErrorCode Read(T & a_out)
    {
      if (m_pStream == nullptr)
        return Err_NullObject;

      myInt result = m_pStream->Read(static_cast<void *>(&a_out), sizeof(T));
      if (result < 0)
        return Stream::GetErrorCode(result);

      if (result != sizeof(T))
        return Err_Failure;
      return Err_None;
    }

  private:

    EndianConverter m_endianConverter;
    Stream *        m_pStream;
  };


}

#endif