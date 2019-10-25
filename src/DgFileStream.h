
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
     wt  |     N     |     N    |     Y    | 
     rwt |     N     |     Y    |     Y    | 
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
    myInt GetSize() override;
    myInt Seek(myInt const offset, StreamSeekOrigin const) override;
    myInt Skip(myInt const) override;
    myInt GetPosition() override;
    myInt SetPosition(myInt const) override;

    myInt Read(void * buffer, myInt const count) override;
    myInt Write(void const * buffer, myInt const count) override;

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