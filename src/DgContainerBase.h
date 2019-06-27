//! @file DgContainerBase.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: ContainerBase

#ifndef DG_CONTAINERBASE_H
#define DG_CONTAINERBASE_H

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
  class ContainerBase
  {
  public:

    //! Constructor
    ContainerBase();

    //! Request the container to hold at least a_nItems. The memory pool
    //! will be large enough to hold a_nItems.
    ContainerBase(size_t a_nItems);

    //! Destructor.
    virtual ~ContainerBase();

    //! Copy constructor.
    ContainerBase(ContainerBase const &);

    //! Assignment
    ContainerBase & operator=(ContainerBase const &);

    //! Move constructor
    ContainerBase(ContainerBase &&);

    //! Move assignment
    ContainerBase & operator=(ContainerBase &&);

    //! Get the current size of the memory pool.
    //! The memory pool is derived from a table of valid memory pool sizes.
    size_t pool_size() const;

    //! Set the current size of the memory pool.
    //! This is only a request. The memory pool is derived 
    //! from a table of valid memory pool sizes, but will endevour to 
    //! be at large enough to fit a_nItems.
    size_t pool_size(size_t a_nItems);

    //! Increase the memory pool size. This is done by selecting the
    //! next value in the table of valid memory pool sizes.
    size_t set_next_pool_size();

  private:
    int   m_poolSizeIndex;
  };
}

#endif