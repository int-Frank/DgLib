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

    MemoryStream(IO::myInt const bufSize,
                 IO::byte const * buffer, 
                 uint32_t const openMode = StreamOpenMode::read);
  
    MemoryStream(MemoryStream const &) = delete;
    MemoryStream & operator=(MemoryStream const &) = delete;

    MemoryStream(MemoryStream &&) noexcept;
    MemoryStream & operator=(MemoryStream &&) noexcept;

    virtual ~MemoryStream();

    ErrorCode Open(uint32_t const openMode = StreamOpenMode::read | StreamOpenMode::write);
    ErrorCode Open(IO::myInt const bufSize,
                   IO::byte const * buffer, 
                   uint32_t const openMode = StreamOpenMode::read);

    void Close() override;
    bool IsOpen() const override;
    IO::ReturnType GetSize() override;

    //Cannot move cursor before the start of the stream
    IO::ReturnType Seek(IO::myInt const offset, StreamSeekOrigin const) override;

    IO::ReturnType Skip(IO::myInt const) override;
    IO::ReturnType GetPosition() override;

    IO::ReturnType SetPosition(IO::myInt const) override;

    //Will fail if the cursor is beyond the end of the stream
    IO::ReturnType Read(void * buffer, IO::myInt const count) override;

    //If the stream is written to beyond the end of the stream,
    //the gap will be filled with 0s.
    IO::ReturnType Write(void const * buffer, IO::myInt const count) override;

    bool IsReadable() const override;
    bool IsSeekable() const override;
    bool IsWritable() const override;

    IO::byte * GetData();
    IO::byte const * GetData() const;

    //count can be +ve or -ve. Either way,
    //on success the current position will be moved to the
    //start of where the block was located.
    IO::ReturnType RemoveBlock(IO::myInt const count);

  private:

    ErrorCode SetBufferSize(IO::myInt const);
    ErrorCode CloseAndReset();
    void SetFlags(uint32_t const);

  private:
  
    bool m_isOpen;
    bool m_isReadable;
    bool m_isWritable;
    IO::myInt m_position;
    IO::myInt m_size;

    PoolSizeManager m_bufSize;
    IO::byte * m_buffer;

  };
}

#endif