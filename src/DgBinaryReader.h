//@group Misc/io

#ifndef DGBINARYREADER_H
#define DGBINARYREADER_H

#include <stdint.h>
#include <string>

#include "DgEndian.h"
#include "DgStream.h"
#include "DgIO_Common.h"
#include "DgBinaryIO.h"

namespace Dg
{
  //Bytes will be interpreted in little endian format by default.
  //The BinaryReader does not own the input stream.
  class BinaryReader : public BinaryIO
  {
  public:

    BinaryReader(EndianConverter const = EndianConverter());
    BinaryReader(Stream *, EndianConverter const = EndianConverter());
    ~BinaryReader();

    BinaryReader(BinaryReader &) = delete;
    BinaryReader & operator=(BinaryReader &) = delete;

    BinaryReader(BinaryReader &&) noexcept;
    BinaryReader & operator=(BinaryReader &&) noexcept;

    ErrorCode Open(Stream *) override;

    template<typename T>
    IO::ReturnType Read(T * a_buffer, IO::myInt const a_count = 1);
    IO::ReturnType ReadRaw(void * a_buffer, IO::myInt const a_count);
    IO::ReturnType Read_string(std::string * a_out, IO::myInt const a_count);
  };

  template<typename T>
  IO::ReturnType BinaryReader::Read(T * a_buffer, IO::myInt const a_count)
  {
    ENSURE_VALID_FUNDAMENTAL_TYPE;

    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    IO::ReturnType rt;
    rt.error = ErrorCode::None;
    rt.value = 0;

    for (IO::myInt i = 0; i < a_count; i++)
    {
      T temp(static_cast<T>(0));
      IO::ReturnType result = m_pStream->Read(static_cast<void *>(&temp), sizeof(T));
      if (result.error != ErrorCode::None)
      {
        rt.error = result.error;
        break;
      }

      if (result.value != static_cast<IO::myInt>(sizeof(T)))
      {
        rt.error = ErrorCode::Failure;
        break;
      }
      a_buffer[i] = m_endianConverter.Convert<T>(temp);
      rt.value++;
    }
    return rt;
  }
}

#endif