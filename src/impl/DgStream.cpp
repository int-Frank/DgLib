
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

  IO::ReturnType Stream::Skip(IO::myInt const a_count)
  {
    return Seek(a_count, StreamSeekOrigin::current);
  }

  IO::ReturnType Stream::SetPosition(IO::myInt const a_position)
  {
    return Seek(a_position, StreamSeekOrigin::begin);
  }

  IO::ReturnType Stream::GetPosition()
  {
    return Seek(0, StreamSeekOrigin::current);
  }

  IO::ReturnType Stream::ReadByte(IO::byte & a_out)
  {
    return Read(&a_out, 1);
  }

  IO::ReturnType Stream::WriteByte(IO::byte const a_value)
  {
    return Write(&a_value, 1);
  }

  IO::ReturnType Stream::WriteString(std::string const & a_str)
  {
    return Write(a_str.c_str(), static_cast<IO::myInt>(a_str.length()));
  }

  IO::ReturnType Stream::CopyTo(Stream * a_pStream, IO::myInt const a_bufSze)
  {
    if (a_pStream == nullptr)
      IO::ReturnType{ErrorCode::BadInput, 0};

    if (!a_pStream->IsWritable())
      IO::ReturnType{ErrorCode::Disallowed, 0};

    IO::myInt bufSze = a_bufSze;
    if (bufSze <= 0)
      bufSze = s_defaultCopyBufSize;

    IO::byte * buffer = new IO::byte[bufSze];
    IO::ReturnType result{ErrorCode::None, 0};

    while (true)
    {
      IO::ReturnType readResult = Read(buffer, bufSze);

      if (readResult.error != ErrorCode::None) //Read error
      {
        result.error = readResult.error;
        break;
      }
      else if (readResult.value == 0) //No more to read
        break;

      IO::ReturnType writeResult = a_pStream->Write(buffer, readResult.value);

      if (writeResult.error != ErrorCode::None) //Write error
      {
        result.error = writeResult.error;
        break;
      }
      result.value += writeResult.value;
    }

    delete[] buffer;
    return result;
  }
}