
#include <exception>
#include <cstring>
#include <cstdlib>
#include "../DgMemoryStream.h"

namespace Dg
{
  MemoryStream::MemoryStream(uint32_t const a_openMode)
    : m_isOpen(false)
    , m_isReadable(false)
    , m_isWritable(false)
    , m_buffer(nullptr)
    , m_position(0)
    , m_size(0)
  {
    m_bufSize.SetSize(s_defaultBufSze);
    Open(a_openMode);
  }

  MemoryStream::MemoryStream(myInt const a_bufSize,
                             byte const * a_buffer, 
                             uint32_t const a_openMode)
    : m_isOpen(false)
    , m_isReadable(false)
    , m_isWritable(false)
    , m_buffer(nullptr)
    , m_position(0)
    , m_size(0)
    , m_bufSize(s_defaultBufSze)
  {
    m_bufSize.SetSize(s_defaultBufSze);
    Open(a_bufSize, a_buffer, a_openMode);
  }

  MemoryStream::MemoryStream(MemoryStream && a_rhs) noexcept
    : Stream(std::move(a_rhs))
    , m_isOpen(std::move(a_rhs.m_isOpen))
    , m_isReadable(std::move(a_rhs.m_isReadable))
    , m_isWritable(std::move(a_rhs.m_isWritable))
    , m_position(std::move(a_rhs.m_position))
    , m_size(std::move(a_rhs.m_size))
    , m_bufSize(std::move(a_rhs.m_bufSize))
    , m_buffer(std::move(a_rhs.m_buffer))
  {

  }

  MemoryStream & MemoryStream::operator=(MemoryStream && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      Stream::operator=(std::move(a_rhs));
      m_isOpen = std::move(a_rhs.m_isOpen);
      m_isReadable = std::move(a_rhs.m_isReadable);
      m_isWritable = std::move(a_rhs.m_isWritable);
      m_position = std::move(a_rhs.m_position);
      m_size = std::move(a_rhs.m_size);
      m_bufSize = std::move(a_rhs.m_bufSize);
      m_buffer = std::move(a_rhs.m_buffer);
    }

    return *this;
  }


  MemoryStream::~MemoryStream()
  {
    CloseAndReset();
    delete[] m_buffer;
    m_buffer = nullptr;
  }


  ErrorCode MemoryStream::Open(uint32_t const a_openMode)
  {
    ErrorCode code = CloseAndReset();
    if (code != Err_None)
      return code;

    SetFlags(a_openMode);
    m_isOpen = true;
    return code;
  }

  ErrorCode MemoryStream::Open(myInt const a_bufSize,
                               byte const * a_buffer, 
                               uint32_t const a_openMode)
  {
    if (a_bufSize < 0 || a_buffer == nullptr)
      return Err_BadInput;

    ErrorCode code = CloseAndReset();
    if (code != Err_None) 
      return code;

    code = SetBufferSize(a_bufSize);
    if (code != Err_None) 
      return code;

    m_isOpen = true;
    SetFlags(a_openMode);

    return GetErrorCode(Write(a_buffer, a_bufSize));
  }


  void MemoryStream::Close()
  {
    CloseAndReset();
  }

  bool MemoryStream::IsOpen() const
  {
    return m_isOpen;
  }

  Stream::myInt MemoryStream::GetSize()
  {
    return m_size;
  }

  Stream::myInt MemoryStream::Seek(myInt const a_offset, StreamSeekOrigin const a_origin)
  {
    if (!m_isOpen)
      return -Err_StreamNotOpen;

    myInt newPos = m_position;

    switch (a_origin)
    {
      case StreamSeekOrigin::begin:
      {
        newPos = a_offset;
        break;
      }
      case StreamSeekOrigin::current:
      {
        newPos += a_offset;
        break;
      }
      default: //case StreamSeekOrigin::end:
        newPos = m_size + a_offset;
    }

    if (newPos < 0)
      return -Err_OutOfBounds;

    m_position = newPos;
    return m_position;
  }

  Stream::myInt MemoryStream::Skip(myInt const a_offset)
  {
    return Seek(a_offset, StreamSeekOrigin::current);
  }

  Stream::myInt MemoryStream::GetPosition()
  {
    return m_position;
  }

  Stream::myInt MemoryStream::SetPosition(myInt const a_position)
  {
    if (a_position < 0)
      return -Err_OutOfBounds;

    m_position = a_position;
    return m_position;
  }

  Stream::myInt MemoryStream::Read(void * a_buffer, myInt const a_count)
  {
    if (!m_isOpen)
      return -Err_StreamNotOpen;

    if (!m_isReadable)
      return -Err_Disallowed;

    if (a_buffer == nullptr || a_count < 0)
      return -Err_BadInput;

    myInt remain = m_size - m_position;
    if (remain < 0)
      return -Err_OutOfBounds;

    if (remain == 0)
      return 0;

    myInt count = a_count;
    if (count > remain)
      count = remain;

    memcpy(a_buffer, &m_buffer[m_position], count);
    m_position += count;
    return count;
  }

  ErrorCode MemoryStream::SetBufferSize(myInt const a_newSize)
  {
    if (a_newSize < 0)
      return Err_BadInput;

    myInt newSize = a_newSize;
    if (newSize < s_defaultBufSze)
      newSize = s_defaultBufSze;

    size_t prevSize = m_bufSize.GetSize();
    m_bufSize.SetSize(newSize);
    byte * newBuf = static_cast<byte*>(realloc(m_buffer, m_bufSize.GetSize() * sizeof(byte)));
    if (newBuf == nullptr)
    {
      m_bufSize.SetSize(prevSize);
      return Err_FailedToAllocMem;
    }
    m_buffer = newBuf;
    return Err_None;
  }

  Stream::myInt MemoryStream::Write(void const * a_buffer, myInt const a_count)
  {
    if (!m_isOpen)
      return -Err_StreamNotOpen;

    if (!m_isWritable)
      return -Err_Disallowed;

    if (a_buffer == nullptr || a_count < 0)
      return -Err_BadInput;

    myInt required = m_position + a_count;

    if (required > static_cast<myInt>(m_bufSize.GetSize()))
    {
      ErrorCode result = SetBufferSize(required);
      if (result != Err_None)
        return -result;
    }
    
    if (m_position > m_size)
      memset(&m_buffer[m_size], 0, (m_position - m_size));

    memcpy(&m_buffer[m_position], a_buffer, a_count);
    m_position += a_count;

    if (m_position > m_size)
      m_size = m_position;

    return a_count;
  }


  bool MemoryStream::IsReadable() const
  {
    return m_isReadable;
  }

  bool MemoryStream::IsSeekable() const
  {
    return true;
  }

  bool MemoryStream::IsWritable() const
  {
    return m_isWritable;
  }

  Stream::byte * MemoryStream::GetData()
  {
    return m_buffer;
  }

  Stream::byte const * MemoryStream::GetData() const
  {
    return m_buffer;
  }

  ErrorCode MemoryStream::RemoveBlock(myInt const a_count)
  {
    if (!m_isWritable)
      return Err_Disallowed;

    if (!m_isOpen)
      return Err_StreamNotOpen;

    if (a_count == 0)
      return Err_None;

    myInt count = a_count;
    if (count > 0)
    {
      myInt remaining = m_size - m_position;
      if (count > remaining)
        return Err_BadInput;
    }
    else
    {
      if (-count > m_position)
        return Err_BadInput;
      m_position += count;
      count = -count;
    }

    m_size -= abs(count);
    memmove(&m_buffer[m_position], &m_buffer[m_position + count], count);
    
    return Err_None;
  }

  ErrorCode MemoryStream::CloseAndReset()
  {
    m_isOpen = false;
    m_isReadable = false;
    m_isWritable = false;
    m_position = 0;
    m_size = 0;
    return SetBufferSize(s_defaultBufSze);
  }

  void MemoryStream::SetFlags(uint32_t const a_mode)
  {
    m_isReadable = false;
    m_isWritable = false;

    if (a_mode & StreamOpenMode::read)
      m_isReadable = true;

    if (a_mode & StreamOpenMode::write)
      m_isWritable = true;
  }
}