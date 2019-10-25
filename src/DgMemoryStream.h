/*
BStone: A Source port of
Blake Stone: Aliens of Gold and Blake Stone: Planet Strike

Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2019 Boris I. Bendovsky (bibendovsky@hotmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef DGMEMORYSTREAM_H
#define DGMEMORYSTREAM_H

#include "DgStream.h"
#include "impl/DgPoolSizeManager.h"

namespace Dg
{
  class MemoryStream : public Stream
  {
  private:

    static size_t const s_defaultBufSze = 1024;

  public:
  
    MemoryStream(uint32_t const openMode = StreamOpenMode::read | StreamOpenMode::write);

    MemoryStream(myInt const bufSize,
                 byte const * buffer, 
                 uint32_t const openMode = StreamOpenMode::read);
  
    MemoryStream(MemoryStream const &) = delete;
    MemoryStream & operator=(MemoryStream const &) = delete;

    MemoryStream(MemoryStream &&) noexcept;
    MemoryStream & operator=(MemoryStream &&) noexcept;

    virtual ~MemoryStream();

    ErrorCode Open(uint32_t const openMode = StreamOpenMode::read | StreamOpenMode::write);
    ErrorCode Open(myInt const bufSize,
                   byte const * buffer, 
                   uint32_t const openMode = StreamOpenMode::read);

    void Close() override;
    bool IsOpen() const override;
    myInt GetSize() override;

    //Cannot move cursor before the start of the stream
    myInt Seek(myInt const offset, StreamSeekOrigin const) override;

    myInt Skip(myInt const) override;
    myInt GetPosition() override;

    myInt SetPosition(myInt const) override;

    //Will fail if the cursor is beyond the end of the stream
    myInt Read(void * buffer, myInt const count) override;

    //If the stream is written to beyond the end of the stream,
    //the gap will be filled with 0s.
    myInt Write(void const * buffer, myInt const count) override;

    bool IsReadable() const override;
    bool IsSeekable() const override;
    bool IsWritable() const override;

    byte * GetData();
    byte const * GetData() const;

    //count can be +ve or -ve. Either way,
    //on success the current position will be moved to the
    //start of where the block was located.
    ErrorCode RemoveBlock(myInt const count);

  private:

    ErrorCode SetBufferSize(myInt const);
    ErrorCode CloseAndReset();
    void SetFlags(uint32_t const);

  private:
  
    bool m_isOpen;
    bool m_isReadable;
    bool m_isWritable;
    myInt m_position;
    myInt m_size;

    PoolSizeManager m_bufSize;
    byte * m_buffer;

  };
}

#endif