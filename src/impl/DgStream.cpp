
#include "../DgStream.h"

namespace Dg
{
  Stream::Stream()
  {

  }

  Stream::~Stream()
  {

  }

  void Stream::Close()
  {
  
  }

  Stream::Stream(Stream && a_rhs) noexcept
  {

  }

  Stream & Stream::operator=(Stream && a_rhs) noexcept
  {
    return *this;
  }

  Stream::myInt Stream::Skip(myInt const a_count)
  {
    return Seek(a_count, StreamSeekOrigin::current);
  }

  Stream::myInt Stream::SetPosition(myInt const a_position)
  {
    return Seek(a_position, StreamSeekOrigin::begin);
  }

  Stream::myInt Stream::GetPosition()
  {
    return Seek(0, StreamSeekOrigin::current);
  }

  Stream::myInt Stream::ReadByte(byte & a_out)
  {
    return Read(&a_out, 1);
  }

  Stream::myInt Stream::WriteByte(byte const a_value)
  {
    return Write(&a_value, 1);
  }

  Stream::myInt Stream::WriteString(std::string const & a_str)
  {
    return Write(a_str.c_str(), static_cast<myInt>(a_str.length()));
  }

  Stream::myInt Stream::CopyTo(Stream * a_pStream, myInt const a_bufSze)
  {
    if (a_pStream == nullptr)
      return -Err_BadInput;

    if (!a_pStream->IsWritable())
      return -Err_Disallowed;

    myInt bufSze = a_bufSze;
    if (bufSze <= 0)
      bufSze = GetDefaultCopyBufferSize();

    byte * buffer = new byte[bufSze];
    myInt result = 0;

    while (true)
    {
      myInt readResult = Read(buffer, bufSze);

      if (readResult < 0) //Read error
      {
        result = readResult;
        break;
      }
      else if (readResult == 0) //No more to read
        break;

      myInt writeResult = a_pStream->Write(buffer, readResult);

      if (writeResult < 0) //Write error
      {
        result = writeResult;
        break;
      }
      result += writeResult;
    }

    delete[] buffer;
    return result;
  }

  Stream::myInt Stream::GetDefaultCopyBufferSize()
  {
    return 4096;
  }

  ErrorCode Stream::GetErrorCode(myInt const a_val)
  {
    if (a_val >= 0)
      return Err_None;
    if (-a_val < Err_COUNT)
      return static_cast<ErrorCode>(-a_val);
    return Err_Unknown;
  }
}