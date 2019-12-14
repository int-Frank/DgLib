//@group Misc/io

#ifndef DGBINARYWRITER_H
#define DGBINARYWRITER_H

#include <stdint.h>
#include <string>

#include "DgEndian.h"
#include "DgStream.h"
#include "DgBinaryIO.h"

namespace Dg
{
  //Bytes will be interpreted in little endian format by default.
  //The BinaryWriter does not own the input stream.
  class BinaryWriter : public BinaryIO
  {
  public:

    BinaryWriter(EndianConverter const = EndianConverter());
    BinaryWriter(Stream *, EndianConverter const = EndianConverter());
    ~BinaryWriter();

    BinaryWriter(BinaryWriter &) = delete;
    BinaryWriter & operator=(BinaryWriter &) = delete;

    BinaryWriter(BinaryWriter &&) noexcept;
    BinaryWriter & operator=(BinaryWriter &&) noexcept;

    ErrorCode::Type Open(Stream *) override;

    template<typename T>
    IO::ReturnType Write(T const * a_val, IO::myInt const a_count = 1);
    IO::ReturnType Write_string(std::string const &);
    IO::ReturnType WriteRaw(void const *, IO::myInt const count);
  };

  template<typename T>
  IO::ReturnType BinaryWriter::Write(T const * a_val, IO::myInt const a_count)
  {
    ENSURE_VALID_FUNDAMENTAL_TYPE;

    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    IO::ReturnType result{ErrorCode::None, 0};
    for (IO::myInt i = 0; i < a_count; i++)
    {
      T temp = m_endianConverter.Convert<T>(a_val[i]);
      IO::ReturnType rt = m_pStream->Write(&temp, static_cast<IO::myInt>(sizeof(T)));

      if (rt.error != ErrorCode::None || rt.value != static_cast<IO::myInt>(sizeof(T)))
      {
        result.error = rt.error;
        break;
      }
      result.value++;
    }
    return result;
  }
}

#endif