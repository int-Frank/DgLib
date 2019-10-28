#include "../DgBinaryReader.h"

namespace Dg
{
  BinaryReader::BinaryReader(EndianConverter const a_ec)
    : BinaryIO(a_ec)
  {

  }

  BinaryReader::BinaryReader(Stream * a_pStream, EndianConverter const a_ec)
    : BinaryIO(a_ec)
  {
    Open(a_pStream);
  }

  BinaryReader::~BinaryReader()
  {

  }

  BinaryReader::BinaryReader(BinaryReader && a_rhs) noexcept
    : BinaryIO(std::move(a_rhs))
  {

  }

  BinaryReader & BinaryReader::operator=(BinaryReader && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      BinaryIO::operator=(std::move(a_rhs));
    }
    return *this;
  }

  ErrorCode BinaryReader::Open(Stream * a_pStream)
  {
    Close();

    if (a_pStream == nullptr)
      return Err_BadInput;

    if (!a_pStream->IsReadable())
      return Err_Disallowed;

    m_pStream = a_pStream;
    return Err_None;
  }

  IO::ReturnType BinaryReader::Read_string(std::string * a_out, IO::myInt const a_count)
  {
    if (a_count < 0 || a_out == nullptr)
      return IO::ReturnType{Err_BadInput, 0};

    IO::myInt curSze = static_cast<IO::myInt>(a_out->size());
    a_out->resize(curSze + a_count);
    return Read<char>(&(*a_out)[curSze], a_count);
  }

  IO::ReturnType BinaryReader::ReadRaw(void * a_buffer, IO::myInt const a_count)
  {
    if (m_pStream == nullptr)
      return IO::ReturnType{Err_NullObject, IO::INVALID_VALUE};

    return m_pStream->Read(a_buffer, a_count);
  }
}