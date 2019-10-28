
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

  MemoryStream::MemoryStream(IO::myInt const a_bufSize,
                             IO::byte const * a_buffer, 
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

  ErrorCode MemoryStream::Open(IO::myInt const a_bufSize,
                               IO::byte const * a_buffer, 
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

    return Write(a_buffer, a_bufSize).error;
  }


  void MemoryStream::Close()
  {
    CloseAndReset();
  }

  bool MemoryStream::IsOpen() const
  {
    return m_isOpen;
  }

  IO::ReturnType MemoryStream::GetSize()
  {
    return IO::ReturnType {Err_None, m_size};
  }

  IO::ReturnType MemoryStream::Seek(IO::myInt const a_offset, StreamSeekOrigin const a_origin)
  {
    if (!m_isOpen)
      return IO::ReturnType{Err_StreamNotOpen, IO::INVALID_VALUE};

    IO::myInt newPos = m_position;

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
      return IO::ReturnType{Err_OutOfBounds, m_position};

    m_position = newPos;
    return IO::ReturnType{Err_None, m_position};
  }

  IO::ReturnType MemoryStream::Skip(IO::myInt const a_offset)
  {
    return Seek(a_offset, StreamSeekOrigin::current);
  }

  IO::ReturnType MemoryStream::GetPosition()
  {
    return IO::ReturnType{Err_None, m_position};
  }

  IO::ReturnType MemoryStream::SetPosition(IO::myInt const a_position)
  {
    if (a_position < 0)
      return IO::ReturnType{Err_OutOfBounds, m_position};

    m_position = a_position;
    return IO::ReturnType{Err_None, m_position};
  }

  IO::ReturnType MemoryStream::Read(void * a_buffer, IO::myInt const a_count)
  {
    if (!m_isOpen)
      return IO::ReturnType{Err_StreamNotOpen, 0};

    if (!m_isReadable)
      return IO::ReturnType{Err_Disallowed, 0};

    if (a_buffer == nullptr || a_count < 0)
      return IO::ReturnType{Err_BadInput, 0};

    IO::myInt remain = m_size - m_position;
    if (remain <= 0)
      return IO::ReturnType{Err_None, 0};

    IO::myInt count = a_count;
    if (count > remain)
      count = remain;

    memcpy(a_buffer, &m_buffer[m_position], count);
    m_position += count;
    
    return IO::ReturnType{Err_None, count};
  }

  ErrorCode MemoryStream::SetBufferSize(IO::myInt const a_newSize)
  {
    if (a_newSize < 0)
      return Err_BadInput;

    IO::myInt newSize = a_newSize;
    if (newSize < s_defaultBufSze)
      newSize = s_defaultBufSze;

    size_t prevSize = m_bufSize.GetSize();
    m_bufSize.SetSize(newSize);
    IO::byte * newBuf = static_cast<IO::byte*>(realloc(m_buffer, m_bufSize.GetSize() * sizeof(IO::byte)));
    if (newBuf == nullptr)
    {
      m_bufSize.SetSize(prevSize);
      return Err_FailedToAllocMem;
    }
    m_buffer = newBuf;
    return Err_None;
  }

  IO::ReturnType MemoryStream::Write(void const * a_buffer, IO::myInt const a_count)
  {
    if (!m_isOpen)
      return IO::ReturnType{Err_StreamNotOpen, 0};

    if (!m_isWritable)
      return IO::ReturnType{Err_Disallowed, 0};

    if (a_buffer == nullptr || a_count < 0)
      return IO::ReturnType{Err_BadInput, 0};

    IO::myInt required = m_position + a_count;

    if (required > static_cast<IO::myInt>(m_bufSize.GetSize()))
    {
      ErrorCode result = SetBufferSize(required);
      if (result != Err_None)
        return IO::ReturnType{result, 0};
    }
    
    if (m_position > m_size)
      memset(&m_buffer[m_size], 0, (m_position - m_size));

    memcpy(&m_buffer[m_position], a_buffer, a_count);
    m_position += a_count;

    if (m_position > m_size)
      m_size = m_position;

    return IO::ReturnType{Err_None, a_count};
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

  IO::byte * MemoryStream::GetData()
  {
    return m_buffer;
  }

  IO::byte const * MemoryStream::GetData() const
  {
    return m_buffer;
  }

  IO::ReturnType MemoryStream::RemoveBlock(IO::myInt const a_count)
  {
    if (!m_isWritable)
      return IO::ReturnType{Err_Disallowed, m_position};

    if (!m_isOpen)
      return IO::ReturnType{Err_StreamNotOpen, m_position};

    if (a_count == 0)
      return IO::ReturnType{Err_None, m_position};

    IO::myInt count = a_count;
    if (count > 0)
    {
      IO::myInt remaining = m_size - m_position;
      if (count > remaining)
        return IO::ReturnType{Err_BadInput, m_position};
    }
    else
    {
      if (-count > m_position)
        return IO::ReturnType{Err_BadInput, m_position};
      m_position += count;
      count = -count;
    }

    m_size -= abs(count);
    memmove(&m_buffer[m_position], &m_buffer[m_position + count], count);
    
    return IO::ReturnType{Err_None, m_position};
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