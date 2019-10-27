#include "../DgBinaryReader.h"

namespace Dg
{
  BinaryReader::BinaryReader(EndianConverter const a_ec)
    : m_endianConverter(a_ec)
    , m_pStream(nullptr)
  {

  }

  BinaryReader::BinaryReader(Stream * a_pStream, EndianConverter const a_ec)
    : m_endianConverter(a_ec)
    , m_pStream(nullptr)
  {
    Open(a_pStream);
  }

  BinaryReader::~BinaryReader()
  {
    Close();
  }

  BinaryReader::BinaryReader(BinaryReader && a_rhs) noexcept
    : m_endianConverter(std::move(a_rhs.m_endianConverter))
    , m_pStream(std::move(a_rhs.m_pStream))
  {

  }

  BinaryReader & BinaryReader::operator=(BinaryReader && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      m_endianConverter = std::move(a_rhs.m_endianConverter);
      m_pStream = std::move(a_rhs.m_pStream);
    }
    return *this;
  }

  void BinaryReader::SetEndianConverter(EndianConverter const a_ec)
  {
    m_endianConverter = a_ec;
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

  bool BinaryReader::IsOpen() const
  {
    return m_pStream != nullptr;
  }

  void BinaryReader::Close()
  {
    m_pStream = nullptr;
  }

  ErrorCode BinaryReader::Read_s8(int8_t & a_out)
  {
    int8_t tempOut(0);
    ErrorCode result = Read<int8_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_s8(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_u8(uint8_t & a_out)
  {
    uint8_t tempOut(0);
    ErrorCode result = Read<uint8_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_u8(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_s16(int16_t & a_out)
  {
    int16_t tempOut(0);
    ErrorCode result = Read<int16_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_s16(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_u16(uint16_t & a_out)
  {
    uint16_t tempOut(0);
    ErrorCode result = Read<uint16_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_u16(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_s32(int32_t & a_out)
  {
    int32_t tempOut(0);
    ErrorCode result = Read<int32_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_s32(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_u32(uint32_t & a_out)
  {
    uint32_t tempOut(0);
    ErrorCode result = Read<uint32_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_u32(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_s64(int64_t & a_out)
  {
    int64_t tempOut(0);
    ErrorCode result = Read<int64_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_s64(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_u64(uint64_t & a_out)
  {
    uint64_t tempOut(0);
    ErrorCode result = Read<uint64_t>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_u64(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_f32(float & a_out)
  {
    float tempOut(0.0f);
    ErrorCode result = Read<float>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_f32(tempOut);
    return result;
  }

  ErrorCode BinaryReader::Read_f64(double & a_out)
  {
    double tempOut(0.0);
    ErrorCode result = Read<double>(tempOut);
    if (result == Err_None)
      a_out = m_endianConverter.Convert_f64(tempOut);
    return result;
  }

  BinaryReader::myInt BinaryReader::Read_string(myInt const a_length, std::string & a_out)
  {
    ErrorCode result = Err_None;
    for (myInt i = 0; i < a_length; i++)
    {
      char tempOut(0);
      ErrorCode result = Read<char>(tempOut);
      if (result == Err_None)
        a_out += tempOut;
      else
        break;
    }
    if (result == Err_None)
      return a_length;
    return -result;
  }

  BinaryReader::myInt BinaryReader::Read(void * a_buffer, myInt const a_count)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Read(a_buffer, a_count);
  }

  BinaryReader::myInt BinaryReader::Seek(myInt const a_offset, 
                                         StreamSeekOrigin const a_origin)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Seek(a_offset, a_origin);
  }

  BinaryReader::myInt BinaryReader::Skip(myInt const a_offset)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Skip(a_offset);
  }

  BinaryReader::myInt BinaryReader::GetPosition()
  {
    if (m_pStream == nullptr)
      return -Err_NullObject;

    return m_pStream->GetPosition();
  }

  BinaryReader::myInt BinaryReader::SetPosition(myInt const a_position)
  {
    if (m_pStream == nullptr)
      return -Err_NullObject;

    return m_pStream->SetPosition(a_position);
  }
}