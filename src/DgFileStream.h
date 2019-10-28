
#ifndef DGFILESTREAM_H
#define DGFILESTREAM_H

#include <cstdio>

#include "DgStream.h"

namespace Dg
{
  /*
  Behaviour with the various open mode options:
   ----------------------------------------------
    bits | File must |    Can   |    Can   | Notes
    set  |   exist?  |   read?  |  write?  | 
   ---------------------------------------------------------
     r   |     Y     |     Y    |     N    |    
     w   |     N     |     N    |     Y    | 
     rw  |     N     |     Y    |     Y    |      
     wt  |     N     |     N    |     Y    | will truncate file on open
     rwt |     N     |     Y    |     Y    | will truncate file on open
  */
  class FileStream : public Stream
  {
  public:

    FileStream();
    FileStream(std::string const & filePath, uint32_t openMode = StreamOpenMode::read);

    FileStream(FileStream const &) = delete;
    FileStream & operator=(FileStream const &) = delete;

    FileStream(FileStream &&) noexcept;
    FileStream & operator=(FileStream &&) noexcept;

    virtual ~FileStream();

    ErrorCode Open(std::string const & filePath, uint32_t const openMode = StreamOpenMode::read);

    void Close() override;
    bool IsOpen() const override;
    IO::ReturnType GetSize() override;
    IO::ReturnType Seek(IO::myInt const offset, StreamSeekOrigin const) override;
    IO::ReturnType Skip(IO::myInt const) override;
    IO::ReturnType GetPosition() override;
    IO::ReturnType SetPosition(IO::myInt const) override;

    IO::ReturnType Read(void * buffer, IO::myInt const count) override;
    IO::ReturnType Write(void const * buffer, IO::myInt const count) override;

    bool IsReadable() const override;
    bool IsSeekable() const override;
    bool IsWritable() const override;

  private:

    void SetFlags(uint32_t const);
    std::string GetOpenFormat(uint32_t const);
    bool IsValidMode(uint32_t const);

  private:

    bool m_isReadable;
    bool m_isWritable;

    FILE * m_file;

  };
}

#endif