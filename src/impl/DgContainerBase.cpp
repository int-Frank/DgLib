#include "DgContainerBase.h"


#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)


namespace Dg
{
  namespace impl
  {
    //! The memory pool sizes in the containers can only be a size from this table.
    //! These sizes start at 16 and go up by multiples of sqrt(2).
    size_t const validContainerPoolSizes[] =
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
      0x1000000000001ull, 0x16a09e667f3bfull, 0x2000000000003ull, 0x2d413cccfe77full, 0x4000000000007ull, 0x5a827999fcefeull, 0x800000000000full, 0xb504f333f9dfcull,
      0x10000000000020ull, 0x16a09e667f3bfaull, 0x20000000000042ull, 0x2d413cccfe77f6ull, 0x40000000000084ull, 0x5a827999fceff0ull, 0x80000000000110ull, 0xb504f333f9dfe8ull,
      0x100000000000220ull, 0x16a09e667f3bfe0ull, 0x200000000000460ull, 0x2d413cccfe77fe0ull, 0x400000000000900ull, 0x5a827999fcf0000ull, 0x800000000001200ull, 0xb504f333f9e0000ull,
      0x1000000000002500ull, 0x16a09e667f3c0100ull, 0x2000000000004c00ull, 0x2d413cccfe780400ull, 0x4000000000009800ull, 0x5a827999fcf00c00ull, 0x8000000000013800ull, 0xb504f333f9e02000ull
    };
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::ContainerBase()
  //--------------------------------------------------------------------------------
  ContainerBase::ContainerBase() : m_poolSizeIndex(0)
  {
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::~ContainerBase()
  //--------------------------------------------------------------------------------
  ContainerBase::~ContainerBase()
  {
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::ContainerBase()
  //--------------------------------------------------------------------------------
  ContainerBase::ContainerBase(size_t a_nItems) : m_poolSizeIndex(0)
  {
    pool_size(a_nItems);
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::ContainerBase()
  //--------------------------------------------------------------------------------
  ContainerBase::ContainerBase(ContainerBase const & a_other) 
    : m_poolSizeIndex(a_other.m_poolSizeIndex)
  {
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::operator=()
  //--------------------------------------------------------------------------------
  ContainerBase & ContainerBase::operator=(ContainerBase const & a_other)
  {
    m_poolSizeIndex = a_other.m_poolSizeIndex;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::ContainerBase()
  //--------------------------------------------------------------------------------
  ContainerBase::ContainerBase(ContainerBase && a_other)
    : m_poolSizeIndex(a_other.m_poolSizeIndex)
  {
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::operator=()
  //--------------------------------------------------------------------------------
  ContainerBase & ContainerBase::operator=(ContainerBase && a_other)
  {
    m_poolSizeIndex = a_other.m_poolSizeIndex;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::pool_size()
  //--------------------------------------------------------------------------------
  size_t ContainerBase::pool_size() const
  {
    return impl::validContainerPoolSizes[m_poolSizeIndex];
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::pool_size()
  //--------------------------------------------------------------------------------
  size_t ContainerBase::pool_size(size_t a_nItems)
  {
    for (int i = 0; i < ARRAY_SIZE(impl::validContainerPoolSizes); i++)
    {
      if (a_nItems <= impl::validContainerPoolSizes[i])
      {
        m_poolSizeIndex = i;
        return pool_size();
      }
    }
    m_poolSizeIndex = ARRAY_SIZE(impl::validContainerPoolSizes) - 1;
    return pool_size();
  }

  //--------------------------------------------------------------------------------
  //	@	ContainerBase::set_next_pool_size()
  //--------------------------------------------------------------------------------
  size_t ContainerBase::set_next_pool_size()
  {
    if (m_poolSizeIndex == ARRAY_SIZE(impl::validContainerPoolSizes))
    {
      return pool_size();
    }
    m_poolSizeIndex++;
    return pool_size();
  }
}