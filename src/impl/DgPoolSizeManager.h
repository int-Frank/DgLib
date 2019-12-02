//! @file DgContainerBase.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: ContainerBase

#ifndef DG_POOLSIZEMANAGER_H
#define DG_POOLSIZEMANAGER_H

#undef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

namespace Dg
{
  namespace impl
  {
    size_t const SizeTable_Root2[] =
    {
      0x10ull, 0x16ull, 0x20ull, 0x2dull, 0x40ull, 0x5aull, 0x80ull, 0xb5ull,
      0x100ull, 0x16aull, 0x200ull, 0x2d4ull, 0x400ull, 0x5a8ull, 0x800ull, 0xb50ull,
      0x1000ull, 0x16a0ull, 0x2000ull, 0x2d41ull, 0x4000ull, 0x5a82ull, 0x8000ull, 0xb504ull,
      0x10000ull, 0x16a09ull, 0x20000ull, 0x2d413ull, 0x40000ull, 0x5a827ull, 0x80000ull, 0xb504full,
      0x100000ull, 0x16a09eull, 0x200000ull, 0x2d413cull, 0x400000ull, 0x5a8279ull, 0x800000ull, 0xb504f3ull,
      0x1000000ull, 0x16a09e6ull, 0x2000000ull, 0x2d413ccull, 0x4000000ull, 0x5a82799ull, 0x8000000ull, 0xb504f33ull,
      0x10000000ull, 0x16a09e66ull, 0x20000000ull, 0x2d413cccull, 0x40000000ull, 0x5a827999ull, 0x80000000ull, 0xb504f333ull,
      0x100000000ull, 0x16a09e667ull, 0x200000000ull, 0x2d413cccfull, 0x400000000ull, 0x5a827999full, 0x800000000ull, 0xb504f333full,
      0x1000000000ull, 0x16a09e667full, 0x2000000000ull, 0x2d413cccfeull, 0x4000000000ull, 0x5a827999fcull, 0x8000000000ull, 0xb504f333f9ull,
      0x10000000000ull, 0x16a09e667f3ull, 0x20000000000ull, 0x2d413cccfe7ull, 0x40000000000ull, 0x5a827999fceull, 0x80000000000ull, 0xb504f333f9dull,
      0x100000000000ull, 0x16a09e667f3bull, 0x200000000000ull, 0x2d413cccfe77ull, 0x400000000000ull, 0x5a827999fcefull, 0x800000000000ull, 0xb504f333f9dfull,
      0x1000000000000ull, 0x16a09e667f3bfull, 0x2000000000000ull, 0x2d413cccfe77full, 0x4000000000000ull, 0x5a827999fcefeull, 0x8000000000000ull, 0xb504f333f9dfcull,
      0x10000000000000ull, 0x16a09e667f3bfaull, 0x20000000000000ull, 0x2d413cccfe77f6ull, 0x40000000000000ull, 0x5a827999fceff0ull, 0x80000000000000ull, 0xb504f333f9dfe8ull,
      0x100000000000000ull, 0x16a09e667f3bfe0ull, 0x200000000000000ull, 0x2d413cccfe77fe0ull, 0x400000000000000ull, 0x5a827999fcf0000ull, 0x800000000000000ull, 0xb504f333f9e0000ull,
      0x1000000000000000ull, 0x16a09e667f3c0100ull, 0x2000000000000000ull, 0x2d413cccfe780400ull, 0x4000000000000000ull, 0x5a827999fcf00c00ull, 0x8000000000000000ull, 0xb504f333f9e02000ull,
      0xFFFFFFFFFFFFFFFFull
    };

    size_t const SizeTable_Prime[] =
    {
      0x13, 0x1f, 0x2b, 0x3d, 0x59, 0x7f, 0xb5, 0xfb,
      0x167, 0x1fd, 0x2cf, 0x3fd, 0x5a7, 0x7f7, 0xb47, 0xffd,
      0x169f, 0x1fff, 0x2d3b, 0x3ffd, 0x5a7f, 0x7fed, 0xb501, 0xfff1,
      0x16a09, 0x1ffff, 0x2d413, 0x3fffb, 0x5a823, 0x7ffff, 0xb5037, 0xffffd,
      0x16a09b, 0x1ffff7, 0x2d413b, 0x3ffffd, 0x5a8279, 0x7ffff1, 0xb504ef, 0xfffffd,
      0x16a09e1, 0x1ffffd9, 0x2d413b7, 0x3fffffb, 0x5a82789, 0x7ffffd9, 0xb504f1b, 0xfffffc7,
      0x16a09e57, 0x1ffffffd, 0x2d413cbf, 0x3fffffdd, 0x5a827975, 0x7fffffff, 0xb504f32d, 0xfffffffb,
      0x16a09e655, 0x1fffffff7, 0x2d413ccab, 0x3ffffffd7, 0x5a827995d, 0x7ffffffe1, 0xb504f3321, 0xffffffffb,
      0x16a09e665b, 0x1fffffffe7, 0x2d413cccd1, 0x3fffffffd3, 0x5a827999f9, 0x7ffffffff9, 0xb504f333ef, 0xffffffffa9,
      0x16a09e667d7, 0x1ffffffffeb, 0x2d413cccfe1, 0x3fffffffff5, 0x5a827999fb3, 0x7ffffffffc7, 0xb504f333f99, 0xfffffffffef,
      0x16a09e667f05, 0x1fffffffffc9, 0x2d413cccfe73, 0x3fffffffffeb, 0x5a827999fce9, 0x7fffffffff8d, 0xb504f333f9dd, 0xffffffffffc5,
      0x16a09e667f3b3, 0x1ffffffffffaf, 0x2d413cccfe763, 0x3ffffffffffe5, 0x5a827999fcef9, 0x7ffffffffff7f, 0xb504f333f9d9b,
      0x10000000000015, 0x16a09e667f3bc9, 0x20000000000039, 0x2d413cccfe77f1, 0x3fffffffffffdf, 0x5a827999fcefa9, 0x80000000000099, 0xb504f333f9dfe3,
      0x100000000000213, 0x16a09e667f3bfdf, 0x200000000000429, 0x2d413cccfe77fd1, 0x4000000000008f1, 0x5a827999fcefff3, 0x8000000000011f9, 0xb504f333f9dfff7,
      0x10000000000024f1, 0x16a09e667f3c00ef, 0x2000000000004be1, 0x2d413cccfe7803f7, 0x40000000000097f3, 0x5a827999fcf00bff, 0x80000000000137db, 0xb504f333f9e01ffb, 0x7fffffffffffffe7
    };
  }

  //! @ingroup DgContainers
  //!
  //! @class ContainerBase
  //!
  //! Base class for containers. Contains a method of obtaining 
  //! a valid data pool size.
  //!
  //! @author Frank B. Hart
  //! @date 24/08/2016
  template<size_t const * ARRAY, size_t N_ELEMENTS>
  class PoolSizeManager
  {
  public:

    //! Constructor
    PoolSizeManager() 
      : m_index(0)
    {

    }

    PoolSizeManager(size_t a_size) 
      : m_index(0)
    {
      SetSize(a_size);
    }

    //! Get the current size of the memory pool.
    //! The memory pool is derived from a table of valid memory pool sizes.
    size_t GetSize() const
    {
      return ARRAY[m_index];
    }

    //! Set the current size of the memory pool.
    //! This is only a request. The memory pool is derived 
    //! from a table of valid memory pool sizes, but will endevour to 
    //! be at large enough to fit a_nItems.
    size_t SetSize(size_t a_nItems)
    {
      for (size_t i = 0; i < N_ELEMENTS; i++)
      {
        if (a_nItems <= ARRAY[i])
        {
          m_index = static_cast<int>(i);
          return GetSize();
        }
      }
      m_index = N_ELEMENTS - 1;
      return GetSize();
    }

    //! Increase the memory pool size. This is done by selecting the
    //! next value in the table of valid memory pool sizes.
    size_t SetNextPoolSize()
    {
      if (m_index == N_ELEMENTS - 1)
        return GetSize();
      m_index++;
      return GetSize();
    }

    size_t PoolSizeManager::PeekNextPoolSize() const
    {
      if (m_index == N_ELEMENTS - 1)
        return GetSize();
      return ARRAY[m_index + 1];
    }

    //! Decrease the memory pool size. This is done by selecting the
    //! previous value in the table of valid memory pool sizes.
    size_t SetPrevPoolSize()
    {
      if (m_index == 0)
        return GetSize();
      m_index--;
      return GetSize();
    }

  private:
    int   m_index;
  };

  using PoolSizeMngr_Root2 = PoolSizeManager<impl::SizeTable_Root2, ARRAY_SIZE(impl::SizeTable_Root2)>;
  using PoolSizeMngr_Prime = PoolSizeManager<impl::SizeTable_Prime, ARRAY_SIZE(impl::SizeTable_Prime)>;

  typedef PoolSizeMngr_Root2 PoolSizeMngr_Default;
}

#endif