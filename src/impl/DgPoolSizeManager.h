//! @file DgContainerBase.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: ContainerBase

#ifndef DG_POOLSIZEMANAGER_H
#define DG_POOLSIZEMANAGER_H

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class ContainerBase
  //!
  //! Base class for containers. Contains a method of obtaining 
  //! a valid data pool size.
  //!
  //! @author Frank B. Hart
  //! @date 24/08/2016
  class PoolSizeManager
  {
  public:

    //! Constructor
    PoolSizeManager();
    PoolSizeManager(size_t);

    //! Get the current size of the memory pool.
    //! The memory pool is derived from a table of valid memory pool sizes.
    size_t GetSize() const;

    //! Set the current size of the memory pool.
    //! This is only a request. The memory pool is derived 
    //! from a table of valid memory pool sizes, but will endevour to 
    //! be at large enough to fit a_nItems.
    size_t SetSize(size_t a_nItems);

    //! Increase the memory pool size. This is done by selecting the
    //! next value in the table of valid memory pool sizes.
    size_t SetNextPoolSize();

    //! Decrease the memory pool size. This is done by selecting the
    //! previous value in the table of valid memory pool sizes.
    size_t SetPrevPoolSize();

  private:
    int   m_index;
  };
}

#endif