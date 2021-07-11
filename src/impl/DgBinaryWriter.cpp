//@group Misc/impl

#include "../DgBinaryWriter.h"

namespace Dg
{
  BinaryWriter::BinaryWriter(EndianConverter const a_ec)
    : BinaryIO(a_ec)
  {

  }

  BinaryWriter::BinaryWriter(Stream * a_pStream, EndianConverter const a_ec)
    : BinaryIO(a_ec)
  {
    Open(a_pStream);
  }

  BinaryWriter::~BinaryWriter()
  {

  }

  BinaryWriter::BinaryWriter(BinaryWriter && a_rhs) noexcept
    : BinaryIO(std::move(a_rhs))
  {

  }

  BinaryWriter & BinaryWriter::operator=(BinaryWriter && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      BinaryIO::operator=(std::move(a_rhs));
    }
    return *this;
  }

  ErrorCode BinaryWriter::Open(Stream * a_pStream)
  {
    Close();

    if (a_pStream == nullptr)
      return ErrorCode::InvalidInput;

    if (!a_pStream->IsWritable())
      return ErrorCode::Disallowed;

    m_pStream = a_pStream;
    return ErrorCode::None;
  }

  IO::ReturnType BinaryWriter::Write_string(std::string const & a_str)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->Write(&a_str[0], static_cast<IO::myInt>(a_str.length()));
  }

  IO::ReturnType BinaryWriter::WriteRaw(void const * a_buffer, IO::myInt const a_count)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{ErrorCode::NullObject, IO::INVALID_VALUE};

    return m_pStream->Write(a_buffer, a_count);
  }
}