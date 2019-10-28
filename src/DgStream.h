
#ifndef DGSTREAM_H
#define DGSTREAM_H

#include <stdint.h>
#include <string>

#include "DgTypes.h"
#include "DgIO_Common.h"

namespace Dg
{
  enum class StreamSeekOrigin
  {
    begin,
    current,
    end, // 1 passed the last element
  };

  struct StreamOpenMode
  {
    enum uint32_t
    {
      read  = 0x1,
      write = 0x2,
      trunc = 0x4,
    };
  };

  class Stream
  {
  public:
  
    Stream();
    virtual ~Stream();

    Stream(Stream const &) = delete;
    Stream & operator=(Stream const &) = delete;

    Stream(Stream &&) noexcept;
    Stream & operator=(Stream &&) noexcept;

    virtual bool IsOpen() const = 0;
    virtual void Close();

    virtual IO::ReturnType GetSize() = 0;

    virtual IO::ReturnType Seek(IO::myInt const offset, StreamSeekOrigin const origin) = 0;

    //! Skips a number of bytes forward if count is positive or
    //! backward otherwise.
    virtual IO::ReturnType Skip(IO::myInt const);

    virtual IO::ReturnType GetPosition();

    //! Moves stream's pointer to a new position.
    virtual IO::ReturnType SetPosition(IO::myInt const position);

    // Attempts to read a specified number of bytes. Check 
    // RetrunType::value to query actual number of bytes read.
    virtual IO::ReturnType Read(void* buffer, IO::myInt const count) = 0;

    // Attempts to write a specified number of bytes to the stream.
    //Check RetrunType::value to query actual number of bytes written.
    virtual IO::ReturnType Write(void const * buffer, IO::myInt const count) = 0;

    virtual IO::ReturnType ReadByte(IO::byte &);

    virtual IO::ReturnType WriteByte(IO::byte const);

    // Writes a string without a terminator.
    virtual IO::ReturnType WriteString(std::string const &);

    virtual bool IsReadable() const = 0;
    virtual bool IsSeekable() const = 0;
    virtual bool IsWritable() const = 0;

    // Copies the stream from a current position to
    // an another stream using internal buffer.
    IO::ReturnType CopyTo(Stream * , IO::myInt const buffer_size = 0);

  private:

    static const IO::myInt s_defaultCopyBufSize = 4096;
  
  };
}

#endif