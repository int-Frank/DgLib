
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


  ErrorCode FileStream::Open(std::string const & a_filePath, uint32_t const a_openMode)
  {
    Close();

    if (!IsValidMode(a_openMode))
      return Err_BadInput;

    std::string format = GetOpenFormat(a_openMode);

    if (format.empty())
      return Err_Failure;

    m_file = fopen(a_filePath.c_str(), format.c_str());

    if (m_file == nullptr)
      return Err_Failure;

    SetFlags(a_openMode);
    return Err_None;
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

  Stream::myInt FileStream::GetSize()
  {
    if (m_file == nullptr)
      return -Err_StreamNotOpen;

    fpos_t position;
    fgetpos(m_file, &position);
    fseek(m_file, 0L, SEEK_END);
    myInt size = ftell(m_file);
    fsetpos(m_file, &position);
    return size;
  }

  Stream::myInt FileStream::Seek(myInt const a_offset, StreamSeekOrigin const a_origin)
  {
    if (m_file == nullptr)
      return -Err_StreamNotOpen;

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
      return -Err_Failure;
    }

    return GetPosition();
  }

  Stream::myInt FileStream::Skip(myInt const a_offset)
  {
    return Seek(a_offset, StreamSeekOrigin::current);
  }

  Stream::myInt FileStream::GetPosition()
  {
    return static_cast<myInt>(ftell(m_file));
  }

  Stream::myInt FileStream::SetPosition(myInt const a_position)
  {
    return Seek(a_position, StreamSeekOrigin::begin);
  }

  Stream::myInt FileStream::Read(void * a_buffer, myInt const a_count)
  {
    if (m_file == nullptr)
      return -Err_StreamNotOpen;

    if (!m_isReadable)
      return -Err_Disallowed;

    if (a_buffer == nullptr || a_count < 0)
      return -Err_BadInput;

    return static_cast<myInt>(fread(a_buffer, 1, a_count, m_file));
  }

  Stream::myInt FileStream::Write(void const * a_buffer, myInt const a_count)
  {
    if (m_file == nullptr)
      return -Err_StreamNotOpen;

    if (!m_isWritable)
      return -Err_Disallowed;

    fwrite(a_buffer, 1, a_count, m_file);
    return a_count;
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