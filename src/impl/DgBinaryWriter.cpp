#include "../DgBinaryWriter.h"

namespace Dg
{
  BinaryWriter::BinaryWriter(EndianConverter const a_ec)
    : m_endianConverter(a_ec)
    , m_pStream(nullptr)
  {

  }

  BinaryWriter::BinaryWriter(Stream * a_pStream, EndianConverter const a_ec)
    : m_endianConverter(a_ec)
    , m_pStream(nullptr)
  {
    Open(a_pStream);
  }

  BinaryWriter::~BinaryWriter()
  {
    Close();
  }

  BinaryWriter::BinaryWriter(BinaryWriter && a_rhs) noexcept
    : m_endianConverter(std::move(a_rhs.m_endianConverter))
    , m_pStream(std::move(a_rhs.m_pStream))
  {

  }

  BinaryWriter & BinaryWriter::operator=(BinaryWriter && a_rhs) noexcept
  {
    if (this != &a_rhs)
    {
      m_endianConverter = std::move(a_rhs.m_endianConverter);
      m_pStream = std::move(a_rhs.m_pStream);
    }
    return *this;
  }

  void BinaryWriter::SetEndianConverter(EndianConverter const a_ec)
  {
    m_endianConverter = a_ec;
  }

  ErrorCode BinaryWriter::Open(Stream * a_pStream)
  {
    Close();

    if (a_pStream == nullptr)
      return Err_BadInput;

    if (!a_pStream->IsWritable())
      return Err_Disallowed;

    m_pStream = a_pStream;
    return Err_None;
  }

  bool BinaryWriter::IsOpen() const
  {
    return m_pStream != nullptr;
  }

  void BinaryWriter::Close()
  {
    m_pStream = nullptr;
  }

  BinaryWriter::myInt BinaryWriter::Write_s8(int8_t const a_val)
  {
    int8_t tempOut = m_endianConverter.Convert_s8(a_val);
    return Write<int8_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_u8(uint8_t const a_val)
  {
    uint8_t tempOut = m_endianConverter.Convert_u8(a_val);
    return Write<int8_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_s16(int16_t const a_val)
  {
    int16_t tempOut = m_endianConverter.Convert_s16(a_val);
    return Write<int16_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_u16(uint16_t const a_val)
  {
    uint16_t tempOut = m_endianConverter.Convert_u16(a_val);
    return Write<uint16_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_s32(int32_t const a_val)
  {
    int32_t tempOut = m_endianConverter.Convert_s32(a_val);
    return Write<int32_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_u32(uint32_t const a_val)
  {
    uint32_t tempOut = m_endianConverter.Convert_u32(a_val);
    return Write<uint32_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_s64(int64_t const a_val)
  {
    int64_t tempOut = m_endianConverter.Convert_s64(a_val);
    return Write<int64_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_u64(uint64_t const a_val)
  {
    uint64_t tempOut = m_endianConverter.Convert_u64(a_val);
    return Write<uint64_t>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_f32(float const a_val)
  {
    float tempOut = m_endianConverter.Convert_f32(a_val);
    return Write<float>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_f64(double const a_val)
  {
    double tempOut = m_endianConverter.Convert_f64(a_val);
    return Write<double>(tempOut);
  }

  BinaryWriter::myInt BinaryWriter::Write_string(std::string const & a_str)
  {
    if (m_pStream == nullptr)
      return -Err_NullObject;

    return m_pStream->Write(&a_str[0], static_cast<Stream::myInt>(a_str.length()));
  }

  BinaryWriter::myInt BinaryWriter::Write(void * a_buffer, myInt const a_count)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Write(a_buffer, a_count);
  }

  BinaryWriter::myInt BinaryWriter::Seek(myInt const a_offset, 
                                         StreamSeekOrigin const a_origin)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Seek(a_offset, a_origin);
  }

  BinaryWriter::myInt BinaryWriter::Skip(myInt const a_offset)
  {
    if (m_pStream == nullptr)
      return Err_NullObject;

    return m_pStream->Skip(a_offset);
  }

  BinaryWriter::myInt BinaryWriter::GetPosition()
  {
    if (m_pStream == nullptr)
      return -Err_NullObject;

    return m_pStream->GetPosition();
  }

  BinaryWriter::myInt BinaryWriter::SetPosition(myInt const a_position)
  {
    if (m_pStream == nullptr)
      return -Err_NullObject;

    return m_pStream->SetPosition(a_position);
  }
}