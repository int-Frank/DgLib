//@group Misc/impl


#include "../DgFileStream.h"

namespace Dg
{
  FileStream::FileStream()
    : m_isReadable(false)
    , m_isWritable(false)
    , m_file(nullptr)
  {

  }

  FileStream::FileStream(std::string const & a_filePath, uint32_t const a_openMode)
    : m_isReadable(false)
    , m_isWritable(false)
    , m_file(nullptr)
  {
    Open(a_filePath, a_openMode);
  }

  FileStream::FileStream(FileStream && a_rhs) noexcept
    : Stream(std::move(a_rhs))
    , m_isReadable(std::move(a_rhs.m_isReadable))
    , m_isWritable(std::move(a_rhs.m_isWritable))
    , m_file(std::move(a_rhs.m_file))
  {

  }

  FileStream & FileStream::operator=(FileStream && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      Stream::operator=(std::move(a_rhs));
      m_isReadable = std::move(a_rhs.m_isReadable);
      m_isWritable = std::move(a_rhs.m_isWritable);
      m_file = std::move(a_rhs.m_file);
    }

    return *this;
  }


  FileStream::~FileStream()
  {
    Close();
  }


  ErrorCode::Type FileStream::Open(std::string const & a_filePath, uint32_t const a_openMode)
  {
    Close();

    if (!IsValidMode(a_openMode))
      return ErrorCode::BadInput;

    std::string format = GetOpenFormat(a_openMode);

    if (format.empty())
      return ErrorCode::Failure;

    errno_t err = fopen_s(&m_file, a_filePath.c_str(), format.c_str());

    if (err != 0)
      return ErrorCode::Failure;

    SetFlags(a_openMode);
    return ErrorCode::None;
  }


  void FileStream::Close()
  {
    if (m_file)
    {
      fclose(m_file);
      m_file = nullptr;
    }
    m_isReadable = false;
    m_isWritable = false;
  }

  bool FileStream::IsOpen() const
  {
    return m_file != nullptr;
  }

  IO::ReturnType FileStream::GetSize()
  {
    if (m_file == nullptr)
      return IO::ReturnType{ErrorCode::StreamNotOpen, IO::INVALID_VALUE};

    fpos_t position;
    fgetpos(m_file, &position);
    fseek(m_file, 0L, SEEK_END);
    IO::myInt size = ftell(m_file);
    fsetpos(m_file, &position);
    return IO::ReturnType{ErrorCode::None, size};
  }

  IO::ReturnType FileStream::Seek(IO::myInt const a_offset, StreamSeekOrigin const a_origin)
  {
    if (m_file == nullptr)
      return IO::ReturnType{ErrorCode::StreamNotOpen, IO::INVALID_VALUE};

    fpos_t position;
    fgetpos(m_file, &position);
    int result = 0;

    switch (a_origin)
    {
      case StreamSeekOrigin::begin:
      {
        result = fseek(m_file, static_cast<long>(a_offset), SEEK_SET);
        break;
      }
      case StreamSeekOrigin::current:
      {
        result = fseek(m_file, static_cast<long>(a_offset), SEEK_CUR);
        break;
      }
      default: //case StreamSeekOrigin::end:
      {
        result = fseek(m_file, static_cast<long>(a_offset), SEEK_END);
        break;
      }
    }

    if (result != 0)
    {
      fsetpos(m_file, &position);
      return IO::ReturnType{ErrorCode::Failure, position};
    }

    return GetPosition();
  }

  IO::ReturnType FileStream::Skip(IO::myInt const a_offset)
  {
    return Seek(a_offset, StreamSeekOrigin::current);
  }

  IO::ReturnType FileStream::GetPosition()
  {
    IO::ReturnType rt;
    rt.value = static_cast<IO::myInt>(ftell(m_file));
    if (rt.value < 0)
      rt.error = ErrorCode::Failure;
    else
      rt.error = ErrorCode::None;
    return rt;
  }

  IO::ReturnType FileStream::SetPosition(IO::myInt const a_position)
  {
    return Seek(a_position, StreamSeekOrigin::begin);
  }

  IO::ReturnType FileStream::Read(void * a_buffer, IO::myInt const a_count)
  {
    if (m_file == nullptr)
      return IO::ReturnType{ErrorCode::StreamNotOpen, 0};

    if (!m_isReadable)
      return IO::ReturnType{ErrorCode::Disallowed, 0};

    if (a_buffer == nullptr || a_count < 0)
      return IO::ReturnType{ErrorCode::BadInput, 0};

    return IO::ReturnType{ErrorCode::None, static_cast<IO::myInt>(fread(a_buffer, 1, a_count, m_file))};
  }

  IO::ReturnType FileStream::Write(void const * a_buffer, IO::myInt const a_count)
  {
    if (m_file == nullptr)
      return IO::ReturnType{ErrorCode::StreamNotOpen, 0};

    if (!m_isWritable)
      return IO::ReturnType{ErrorCode::Disallowed, 0};

    if (a_count == 0)
      return IO::ReturnType{ErrorCode::None, 0};

    //We need this as fwrite cannot follow after a fread without calling a repositioning function.
    //That is, FileStream::Write will fail if it follows a FileStream::Read().
    fseek(m_file, 0, SEEK_CUR);

    IO::ReturnType rt;
    rt.value = static_cast<IO::myInt>(fwrite(a_buffer, 1, a_count, m_file));
    
    if (rt.value != a_count)
      rt.error = ErrorCode::WriteError;
    else
      rt.error = ErrorCode::None;

    return rt;
  }


  bool FileStream::IsReadable() const
  {
    return m_isReadable;
  }

  bool FileStream::IsSeekable() const
  {
    return true;
  }

  bool FileStream::IsWritable() const
  {
    return m_isWritable;
  }

  void FileStream::SetFlags(uint32_t const a_mode)
  {
    m_isReadable = false;
    m_isWritable = false;

    if (a_mode & StreamOpenMode::read)
      m_isReadable = true;

    if (a_mode & StreamOpenMode::write)
      m_isWritable = true;
  }

  std::string FileStream::GetOpenFormat(uint32_t const a_openMode)
  {
    uint32_t const r = StreamOpenMode::read;
    uint32_t const w = StreamOpenMode::write;
    uint32_t const t = StreamOpenMode::trunc;

    if (a_openMode ==  r)           return "rb";
    if (a_openMode ==  w)           return "r+b";
    if (a_openMode == (r | w))      return "r+b";
    if (a_openMode == (w | t))      return "wb";
    if (a_openMode == (r | w | t))  return "w+b";
    return "";
  }

  bool FileStream::IsValidMode(uint32_t const a_mode)
  {
    uint32_t const r = StreamOpenMode::read;
    uint32_t const w = StreamOpenMode::write;
    uint32_t const t = StreamOpenMode::trunc;

    switch (a_mode)
    {
      case r:
      case w:
      case (r | w):
      case (w | t):
      case (r | w | t):
        return true;
      default:
      {}
    }
    return false;
  }
}