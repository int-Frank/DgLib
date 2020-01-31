//@group Misc/io

#ifndef DGBINARYIO_H
#define DGBINARYIO_H

#include "DgEndian.h"
#include "DgStream.h"

namespace Dg
{
  class BinaryIO
  {
  public:

    BinaryIO(EndianConverter const = EndianConverter());
    virtual ~BinaryIO();

    BinaryIO(BinaryIO &) = delete;
    BinaryIO & operator=(BinaryIO &) = delete;

    BinaryIO(BinaryIO &&) noexcept;
    BinaryIO & operator=(BinaryIO &&) noexcept;

    void SetEndianConverter(EndianConverter const);

    virtual ErrorCode Open(Stream *) = 0;
    bool IsOpen() const;
    void Close();

    // Returns a new position.
    IO::ReturnType Seek(IO::myInt const offset, StreamSeekOrigin const origin);

    // Skips a number of bytes forward if count is positive or
    // backward otherwise.
    IO::ReturnType Skip(IO::myInt const);

    // Returns a current position or a negative value on error.
    IO::ReturnType GetPosition();

    // Moves stream's pointer to a new position.
    // Returns position on success.
    IO::ReturnType SetPosition(IO::myInt const position);

  protected:

    EndianConverter m_endianConverter;
    Stream * m_pStream;
  };
}

#endif