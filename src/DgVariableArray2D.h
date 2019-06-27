//! @file Dg_vector.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: VariableArray2D<>

#ifndef DGVARIABLEARRAY2D_H
#define DGVARIABLEARRAY2D_H

#include <sstream>

#include "DgDynamicArray.h"

//TODO check for nullptr returns in realloc and throw
namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class VariableArray2D
  //!
  //! @brief Contiguous array, similar to std::vector.
  //!
  //! http://www.cplusplus.com/reference/VariableArray2D/VariableArray2D/
  //!
  //! Similar to std::vector. Constructors/destructors are not called, so use for pod types only.
  //!
  //!
  //! @author Frank Hart
  //! @date 7/01/2014
  template<class T>
  class VariableArray2D : public ContainerBase
  {
  public:
    //Constructor / destructor
    VariableArray2D();

    //! Construct with a set size
    ~VariableArray2D();

    //! Copy constructor
    VariableArray2D(VariableArray2D const &);

    //! Assignment
    VariableArray2D& operator= (VariableArray2D const &);

    //! Move constructor
    VariableArray2D(VariableArray2D &&);

    //! Move assignment
    VariableArray2D& operator= (VariableArray2D &&);

    //! Access element
    T & operator()(size_t row, size_t element);

    //! Accessor
    T const & operator()(size_t row, size_t element) const;

    //! Access element
    T & at(size_t row, size_t element);

    //! Accessor
    T const & at(size_t row, size_t element) const;

    //! Current size of the array
    size_t rows() const
    {
      return m_indices.size();
    }

    size_t elements(size_t a_row) const
    {
      rangeCheckRow(a_row);
      return m_indices.size();
    }

    //! Add element to the back of the array.
    void push_back(T const * pItems, size_t count);

    void clear();

  private:

    void rangeCheck(size_t a_row, size_t a_element) const;
    void rangeCheckRow(size_t a_row) const;

    //Data members
    struct Index
    {
      size_t start;
      size_t count;
    };

    DynamicArray<T>     m_data;
    DynamicArray<Index> m_indices;
  };

  template<class T>
  VariableArray2D<T>::VariableArray2D() 
  {

  }

  template<class T>
  VariableArray2D<T>::~VariableArray2D()
  {

  }

  template<class T>
  VariableArray2D<T>::VariableArray2D(VariableArray2D<T> const & a_other)
    : m_data(a_other.m_data)
    , m_indices(a_other.m_indices)
  {

  }

  template<class T>
  VariableArray2D<T>::VariableArray2D(VariableArray2D<T> && a_other) 
    : m_data(std::move(a_other.m_data))
    , m_indices(std::move(a_other.m_indices))
  {

  }

  template<class T>
  VariableArray2D<T> & VariableArray2D<T>::operator=(VariableArray2D<T> && a_other)
  {
    if (this != &a_other)
    {
      m_data = std::move(a_other.m_data);
      m_indices = std::move(a_other.m_indices);
    }
    return *this;
  }

  template<class T>
  VariableArray2D<T>& VariableArray2D<T>::operator=(VariableArray2D const & a_other)
  {
    if (this != &a_other)
    {
      m_data = a_other.m_data;
      m_indices = a_other.m_indices;
    }
    return *this;
  }

  template<class T>
  void VariableArray2D<T>::push_back(T const * a_pItems, size_t a_count)
  {
    Index ind;
    ind.start = m_data.size();
    ind.count = a_count;

    m_indices.push_back(ind);

    for (size_t i = 0; i < a_count; i++)
      m_data.push_back(a_pItems[i]);
  }

  template<class T>
  void VariableArray2D<T>::clear()
  {
    m_data.clear();
    m_indices.clear();
  }

  template<class T>
  T & VariableArray2D<T>::operator()(size_t a_row, size_t a_element)
  {
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }

  template<class T>
  T const & VariableArray2D<T>::operator()(size_t a_row, size_t a_element) const
  {
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }

  template<class T>
  T & VariableArray2D<T>::at(size_t a_row, size_t a_element)
  {
    rangeCheck(a_row, a_element);
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }

  template<class T>
  T const & VariableArray2D<T>::at(size_t a_row, size_t a_element) const
  {
    rangeCheck(a_row, a_element);
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }

  template<class T>
  void VariableArray2D<T>::rangeCheck(size_t a_row, size_t a_element) const
  {
    std::ostringstream oss;
    if (a_row >= m_indices.size())
      oss << "Row '" << a_row << "' out of range. ";
    else if (a_element >= m_indices[a_row].count)
    {
      oss << "Element '" << a_element << "' out of range for row " << a_row 
        << ". This row has " a_indices[a_row].count << " elements. ";
    }

    // if nothing has been written to oss then all indices are valid
    if (!oss.str().empty())
      throw std::out_of_range(oss.str());
  }

  template<class T>
  void VariableArray2D<T>::rangeCheckRow(size_t a_row) const
  {
    std::ostringstream oss;
    if (a_row >= m_indices.size())
      oss << "Row '" << a_row << "' out of range. ";

    // if nothing has been written to oss then all indices are valid
    if (!oss.str().empty())
      throw std::out_of_range(oss.str());
  }
}
#endif