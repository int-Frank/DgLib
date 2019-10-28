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

    // Returns a new position or a negative value on error. On 
    // success, the position will be clamped to [0, size]
    IO::ReturnType Seek(IO::myInt const offset, StreamSeekOrigin const origin);

    // Skips a number of bytes forward if count is positive or
    // backward otherwise.
    // Returns a negative value on error.
    IO::ReturnType Skip(IO::myInt const);

    // Returns a current position or a negative value on error.
    IO::ReturnType GetPosition();

    // Moves stream's pointer to a new position.
    // Returns position on success.
    // Returns a negative value on error.
    IO::ReturnType SetPosition(IO::myInt const position);

  protected:

    EndianConverter m_endianConverter;
    Stream * m_pStream;
  };
}

#endif