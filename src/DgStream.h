
#ifndef DGSTREAM_H
#define DGSTREAM_H

#include <stdint.h>
#include <string>

#include "DgTypes.h"

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

    typedef uint8_t byte;
    typedef int64_t myInt;

    // The negative myInt returned by some methods as an error
    // can be converted to an ErrorCode with this method. 
    static ErrorCode GetErrorCode(myInt);

  public:
  
    Stream();
    virtual ~Stream();

    Stream(Stream const &) = delete;
    Stream & operator=(Stream const &) = delete;

    Stream(Stream &&) noexcept;
    Stream & operator=(Stream &&) noexcept;

    virtual bool IsOpen() const = 0;
    virtual void Close();

    // Returns a size of the stream or a negative value on error.
    virtual myInt GetSize() = 0;

    // Returns a new position or a negative value on error. On 
    virtual myInt Seek(myInt const offset, StreamSeekOrigin const origin) = 0;

    // Skips a number of bytes forward if count is positive or
    // backward otherwise.
    // Returns a negative value on error.
    virtual myInt Skip(myInt const);

    // Returns a current position or a negative value on error.
    virtual myInt GetPosition();

    // Moves stream's pointer to a new position.
    // Returns position on success.
    // Returns a negative value on error.
    virtual myInt SetPosition(myInt const position);

    // Reads a specified number of bytes and returns an actual
    // read number of bytes.
    // Returns a negative value on error.
    virtual myInt Read(void* buffer, myInt const count) = 0;

    // Writes byte data into the stream at the current position.
    // If successful, the position is advanced to the end of
    // the write block.
    // Returns number of bytes written or a negative value on error.
    virtual myInt Write(void const * buffer, myInt const count) = 0;

    // Returns number of bytes read or a negative value on error.
    virtual myInt ReadByte(byte &);

    // Returns number of bytes written or a negative value on error.
    virtual myInt WriteByte(byte const);

    // Writes a string without a terminator
    // Err_None (0) on success
    // negative value on error
    virtual myInt WriteString(std::string const &);

    virtual bool IsReadable() const = 0;
    virtual bool IsSeekable() const = 0;
    virtual bool IsWritable() const = 0;

    // Copies the stream from a current position to
    // an another stream using internal buffer.
    // The position will change. This is useful if the copy
    // fails; you can query the position to see where it
    // got up to.
    myInt CopyTo(Stream * , myInt const buffer_size = 0);

    static myInt GetDefaultCopyBufferSize();
  
  private:
  
  };
}

#endif