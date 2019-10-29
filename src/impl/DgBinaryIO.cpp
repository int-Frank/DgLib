#include "../DgBinaryIO.h"

namespace Dg
{
  BinaryIO::BinaryIO(EndianConverter const a_ec)
    : m_endianConverter(a_ec)
    , m_pStream(nullptr)
  {

  }

  BinaryIO::~BinaryIO()
  {
    Close();
  }

  BinaryIO::BinaryIO(BinaryIO && a_rhs) noexcept
    : m_endianConverter(std::move(a_rhs.m_endianConverter))
    , m_pStream(std::move(a_rhs.m_pStream))
  {

  }

  BinaryIO & BinaryIO::operator=(BinaryIO && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      m_endianConverter = std::move(a_rhs.m_endianConverter);
      m_pStream = std::move(a_rhs.m_pStream);
    }
    return *this;
  }

  void BinaryIO::SetEndianConverter(EndianConverter const a_ec)
  {
    m_endianConverter = a_ec;
  }

  bool BinaryIO::IsOpen() const
  {
    return m_pStream != nullptr;
  }

  void BinaryIO::Close()
  {
    m_pStream = nullptr;
  }

  IO::ReturnType BinaryIO::Seek(IO::myInt const a_offset, 
                                StreamSeekOrigin const a_origin)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->Seek(a_offset, a_origin);
  }

  IO::ReturnType BinaryIO::Skip(IO::myInt const a_offset)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->Skip(a_offset);
  }

  IO::ReturnType BinaryIO::GetPosition()
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->GetPosition();
  }

  IO::ReturnType BinaryIO::SetPosition(IO::myInt const a_position)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->SetPosition(a_position);
  }
}