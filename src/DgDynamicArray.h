//@group Collections

//! @file Dg_vector.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: DynamicArray<>

#ifndef DGDYNAMICARRAY_H
#define DGDYNAMICARRAY_H

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <stdint.h>

#include "impl/DgPoolSizeManager.h"

namespace Dg
{
  template<typename T>
  class DynamicArray
  {
  public:

    class const_iterator
    {
      friend class DynamicArray;
      friend class iterator;
    private:

      const_iterator(T const *);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const& a_it);
      const_iterator& operator=(const_iterator const& a_other);

      bool operator==(const_iterator const& a_it) const;
      bool operator!=(const_iterator const& a_it) const;

      T const* operator->() const;
      T const& operator*() const;

      const_iterator operator+(size_t) const;
      const_iterator operator-(size_t) const;

      const_iterator& operator+=(size_t);
      const_iterator& operator-=(size_t);

      const_iterator& operator++();
      const_iterator operator++(int);
      const_iterator& operator--();
      const_iterator operator--(int);

    private:
      T const * m_pData;
    };

    class iterator
    {
      friend class DynamicArray;
    private:

      iterator(T*);

    public:

      iterator();
      ~iterator();

      iterator(iterator const& a_it);
      iterator& operator=(iterator const& a_other);

      bool operator==(iterator const& a_it) const;
      bool operator!=(iterator const& a_it) const;

      T* operator->();
      T& operator*();

      iterator operator+(size_t) const;
      iterator operator-(size_t) const;

      iterator& operator+=(size_t);
      iterator& operator-=(size_t);

      iterator& operator++();
      iterator operator++(int);
      iterator& operator--();
      iterator operator--(int);

      operator const_iterator() const;

    private:
      T* m_pData;
    };

  public:

    DynamicArray();
    DynamicArray(size_t memBlockSize);

    ~DynamicArray();

    DynamicArray(DynamicArray const &);
    DynamicArray& operator= (DynamicArray const &);

    DynamicArray(DynamicArray &&) noexcept;
    DynamicArray& operator= (DynamicArray &&) noexcept;

    T & operator[](size_t);
    T const & operator[](size_t) const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to item in the DynamicArray
    T & back();

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to item in the DynamicArray
    T const & back() const;

    //! Current size of the array
    size_t size() const;

    //! Is the array empty
    bool empty() const;

    //! Get pointer to first element.
    T* data();

    //! Get pointer to first element.
    const T* data() const;

    //! Add element to the back of the array.
    void push_back(T const &);

    //! Remove element from the back of the array.
    void pop_back();

    //! Remove element at position
    void erase(size_t position);

    //! Insert element at position.
    void insert(size_t position, T const &item);

    //! Current size is flagged as 0. Elements are NOT destroyed.
    void clear();

    //! Set the current size to 0 and the reserve to new_size
    void resize(size_t);

    //! Erase the element at index by swapping in the last element.
    //! Calling this on an empty DynamicArray will no doubt cause a crash.
    void erase_swap(size_t a_ind);

  private:
    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend();
    void init(DynamicArray const &);

  private:
    //Data members
    T*              m_pData;
    size_t          m_nItems;
    PoolSizeMngr_Default m_poolSize;
  };

  //------------------------------------------------------------------------------------------------
  // const_iterator
  //------------------------------------------------------------------------------------------------
  template<typename T>
  DynamicArray<T>::const_iterator::const_iterator(T const* a_pData)
    : m_pData(a_pData)
  {

  }

  template<typename T>
  DynamicArray<T>::const_iterator::const_iterator()
    : m_pData(nullptr)
  {

  }

  template<typename T>
  DynamicArray<T>::const_iterator::~const_iterator()
  {

  }

  template<typename T>
  DynamicArray<T>::const_iterator::const_iterator(const_iterator const& a_it)
    : m_pData(a_it.m_pData)
  {

  }

  template<typename T>
  typename DynamicArray<T>::const_iterator&
    DynamicArray<T>::const_iterator::operator=(const_iterator const& a_it)
  {
    m_pData = a_it.m_pData;
    return *this;
  }

  template<typename T>
  bool DynamicArray<T>::const_iterator::operator==(const_iterator const& a_it) const
  {
    return m_pData == a_it.m_pData;
  }

  template<typename T>
  bool DynamicArray<T>::const_iterator::operator!=(const_iterator const& a_it) const
  {
    return m_pData != a_it.m_pData;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::const_iterator::operator+(size_t a_val) const
  {
    T const * pData = m_pData + a_val;
    return const_iterator(pData);
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::const_iterator::operator-(size_t a_val) const
  {
    T const * pData = m_pData - a_val;
    return const_iterator(pData);
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator&
    DynamicArray<T>::const_iterator::operator+=(size_t a_val)
  {
    m_pData += a_val;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator&
    DynamicArray<T>::const_iterator::operator-=(size_t a_val)
  {
    m_pData -= a_val;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator&
    DynamicArray<T>::const_iterator::operator++()
  {
    m_pData++;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator&
    DynamicArray<T>::const_iterator::operator--()
  {
    m_pData--;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T const * DynamicArray<T>::const_iterator::operator->() const
  {
    return m_pData;
  }

  template<typename T>
  T const & DynamicArray<T>::const_iterator::operator*() const
  {
    return *m_pData;
  }

  //------------------------------------------------------------------------------------------------
  // iterator
  //------------------------------------------------------------------------------------------------
  template<typename T>
  DynamicArray<T>::iterator::iterator(T* a_pData)
    : m_pData(a_pData)
  {

  }

  template<typename T>
  DynamicArray<T>::iterator::iterator()
    : m_pData(nullptr)
  {

  }

  template<typename T>
  DynamicArray<T>::iterator::~iterator()
  {

  }

  template<typename T>
  DynamicArray<T>::iterator::iterator(iterator const& a_it)
    : m_pData(a_it.m_pData)
  {

  }

  template<typename T>
  typename DynamicArray<T>::iterator&
    DynamicArray<T>::iterator::operator=(iterator const& a_it)
  {
    m_pData = a_it.m_pData;
    return *this;
  }

  template<typename T>
  bool DynamicArray<T>::iterator::operator==(iterator const& a_it) const
  {
    return m_pData == a_it.m_pData;
  }

  template<typename T>
  bool DynamicArray<T>::iterator::operator!=(iterator const& a_it) const
  {
    return m_pData != a_it.m_pData;
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::iterator::operator+(size_t a_val) const
  {
    T* pData = m_pData + a_val;
    return iterator(pData);
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::iterator::operator-(size_t a_val) const
  {
    T* pData = m_pData - a_val;
    return iterator(pData);
  }

  template<typename T>
  typename DynamicArray<T>::iterator&
    DynamicArray<T>::iterator::operator+=(size_t a_val)
  {
    m_pData += a_val;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::iterator&
    DynamicArray<T>::iterator::operator-=(size_t a_val)
  {
    m_pData -= a_val;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::iterator&
    DynamicArray<T>::iterator::operator++()
  {
    m_pData++;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename DynamicArray<T>::iterator&
    DynamicArray<T>::iterator::operator--()
  {
    m_pData--;
    return *this;
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T * DynamicArray<T>::iterator::operator->()
  {
    return m_pData;
  }

  template<typename T>
  T & DynamicArray<T>::iterator::operator*()
  {
    return *m_pData;
  }

  template<typename T>
  DynamicArray<T>::iterator::operator
    typename DynamicArray<T>::const_iterator() const
  {
    return const_iterator(m_pData);
  }

  //--------------------------------------------------------------------------------
  //		DynamicArray
  //--------------------------------------------------------------------------------

  template<typename T>
  DynamicArray<T>::DynamicArray()
    : m_pData(nullptr)
    , m_nItems(0)
  {
    m_pData = static_cast<T*>(malloc(m_poolSize.GetSize() * sizeof(T)));
    if (m_pData == nullptr)
      throw std::bad_alloc();
  }

  template<typename T>
  DynamicArray<T>::DynamicArray(size_t a_size)
    : m_pData(nullptr)
    , m_nItems(0)
  {
    m_poolSize.SetSize(a_size);
    m_pData = static_cast<T*>(malloc(m_poolSize.GetSize() * sizeof(T)));
    if (m_pData == nullptr)
      throw std::bad_alloc();
  }

  template<typename T>
  DynamicArray<T>::~DynamicArray()
  {
    for (size_t i = 0; i < m_nItems; i++)
      m_pData[i].~T();

    free(m_pData);
  }

  template<typename T>
  DynamicArray<T>::DynamicArray(DynamicArray const & a_other)
    : m_poolSize(a_other.m_poolSize)
    , m_pData(nullptr)
    , m_nItems(0)
  {
    m_poolSize.SetSize(a_other.m_poolSize.GetSize());
    init(a_other);
  }

  template<typename T>
  DynamicArray<T> & DynamicArray<T>::operator= (DynamicArray const & a_other)
  {
    if (this != &a_other)
    {
      clear();
      m_poolSize = a_other.m_poolSize;
      init(a_other);
    }
    return *this;
  }

  template<typename T>
  DynamicArray<T>::DynamicArray(DynamicArray && a_other) noexcept
    : m_poolSize(a_other.m_poolSize)
    , m_pData(a_other.m_pData)
    , m_nItems(a_other.m_nItems)
  {
    a_other.m_pData = nullptr;
    a_other.m_nItems = 0;
  }

  template<typename T>
  DynamicArray<T> & DynamicArray<T>::operator= (DynamicArray && a_other) noexcept
  {
    if (this != &a_other)
    {
      //Assign to this
      m_nItems = a_other.m_nItems;
      m_pData = a_other.m_pData;
      m_poolSize = a_other.m_poolSize;

      //Clear other
      a_other.m_pData = nullptr;
      a_other.m_nItems = 0;
    }
    return *this;
  }

  template<typename T>
  T & DynamicArray<T>::operator[](size_t i)				
  { 
    return m_pData[i]; 
  }

  template<typename T>
  T const & DynamicArray<T>::operator[](size_t i) const
  { 
    return m_pData[i]; 
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::begin()
  {
    return iterator(m_pData);
  }

  template<typename T>
  typename DynamicArray<T>::iterator
    DynamicArray<T>::end()
  {
    return iterator(m_pData + m_nItems);
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::cbegin() const
  {
    return const_iterator(m_pData);
  }

  template<typename T>
  typename DynamicArray<T>::const_iterator
    DynamicArray<T>::cend() const
  {
    return const_iterator(m_pData + m_nItems);
  }

  template<typename T>
  T & DynamicArray<T>::back() 
  { 
    return m_pData[m_nItems - 1]; 
  }

  template<typename T>
  T const & DynamicArray<T>::back() const
  { 
    return m_pData[m_nItems - 1]; 
  }

  template<typename T>
  size_t DynamicArray<T>::size() const			
  { 
    return m_nItems; 
  }

  template<typename T>
  bool DynamicArray<T>::empty() const			
  { 
    return m_nItems == 0; 
  }

  template<typename T>
  T * DynamicArray<T>::data()
  { 
    return m_pData; 
  }

  template<typename T>
  T const * DynamicArray<T>::data() const
  { 
    return m_pData; 
  }

  template<typename T>
  void DynamicArray<T>::push_back(T const &a_item)
  {
    if (m_nItems == m_poolSize.GetSize())
      extend();

    new(&m_pData[m_nItems]) T(a_item);
    m_nItems++;
  }

  template<typename T>
  void DynamicArray<T>::insert(size_t a_position, T const &a_item)
  {
    if (a_position > m_nItems)
      throw std::out_of_range("Index out of bounds when inserting element.");

    if (m_nItems == m_poolSize.GetSize())
      extend();

    for (size_t i = m_nItems; i > a_position; i--)
      memcpy(&m_pData[i], &m_pData[i - 1], sizeof(T));

    new(&m_pData[a_position]) T(a_item);
    m_nItems++;
  }

  template<typename T>
  void DynamicArray<T>::erase(size_t a_position)
  {
    if (a_position > m_nItems)
      throw std::out_of_range("Index out of bounds when inserting element.");

    m_pData[a_position].~T();

    for (size_t i = a_position; (i + 1) < m_nItems; i++)
      memcpy(&m_pData[i], &m_pData[i + 1], sizeof(T));

    m_nItems--;
  }

  template<typename T>
  void DynamicArray<T>::pop_back()
  {
    m_pData[m_poolSize.GetSize() - 1].~T();
    --m_nItems;
  }

  template<typename T>
  void DynamicArray<T>::clear()
  {
    for (size_t i = 0; i < m_nItems; i++)
      m_pData[i].~T();
    m_nItems = 0;
  }

  template<typename T>
  void DynamicArray<T>::resize(size_t a_size)
  {
    pool_size(a_size);

    if (m_poolSize.GetSize() < m_nItems)
    {
      for (size_t i = m_poolSize.GetSize(); i < m_nItems; i++)
        m_pData[i].~T();
      m_nItems = m_poolSize.GetSize();
    }

    m_pData = static_cast<T*>(realloc(m_pData, m_poolSize.GetSize() * sizeof(T)));
    if (m_pData == nullptr)
      throw std::bad_alloc();
  }

  template<typename T>
  void DynamicArray<T>::erase_swap(size_t a_ind)
  {
    m_pData[a_ind].~T();
    memcpy(&m_pData[a_ind], &m_pData[m_nItems - 1], sizeof(T));
    --m_nItems;
  }

  template<typename T>
  void DynamicArray<T>::extend()
  {
    m_poolSize.SetNextPoolSize();
    m_pData = static_cast<T*>(realloc(m_pData, m_poolSize.GetSize() * sizeof(T)));
    if (m_pData == nullptr)
      throw std::bad_alloc();
  }

  //TODO initializing from another can fail. If so, the array
  //should be left in its original state. Currently it is left 
  //in an invalid state.
  template<typename T>
  void DynamicArray<T>::init(DynamicArray const & a_other)
  {
    m_poolSize = a_other.m_poolSize;
    m_nItems = a_other.m_nItems;
    m_pData = static_cast<T*>(realloc(m_pData, m_poolSize.GetSize() * sizeof(T)));
    if (m_pData == nullptr)
      throw std::bad_alloc();

    for (size_t i = 0; i < m_nItems; i++)
      new (&m_pData[i]) T(a_other.m_pData[i]);
  }
  
  //--------------------------------------------------------------------------------
  //		Bool specialization
  //--------------------------------------------------------------------------------
  template<>
  class DynamicArray<bool>
  {
  private:

    template<int T>
    struct Attr;

    template<>
    struct Attr<1>
    {
      typedef uint8_t intType;
      static intType const shift = 3;
      static intType const mask = 7;
      static intType const nBits = CHAR_BIT * sizeof(intType);
    };

    template<>
    struct Attr<2>
    {
      typedef uint16_t intType;
      static intType const shift = 4;
      static intType const mask = 15;
      static intType const nBits = CHAR_BIT * sizeof(intType);
    };

    template<>
    struct Attr<4>
    {
      typedef uint32_t intType;
      static intType const shift = 5;
      static intType const mask = 31;
      static intType const nBits = CHAR_BIT * sizeof(intType);
    };

    template<>
    struct Attr<8>
    {
      typedef uint64_t intType;
      static intType const shift = 6;
      static intType const mask = 63;
      static intType const nBits = CHAR_BIT * sizeof(intType);
    };

    typedef Attr<8> TypeTraits;

  public:

    class reference 
    {
      friend class DynamicArray<bool>;
      reference(TypeTraits::intType & a_rBucket, int a_bitIndex)
        : m_rBucket(a_rBucket)
        , m_bitIndex(a_bitIndex)
      {

      }

    public:

      ~reference() {}

      operator bool() const
      {
        return ((m_rBucket >> m_bitIndex) & size_t(1)) != 0;
      }

      reference & operator= (bool a_val)
      {
        TypeTraits::intType x = (a_val) ? 1 : 0;
        m_rBucket = m_rBucket & ~(TypeTraits::intType(1) << m_bitIndex) | (x << m_bitIndex);
        return *this;
      }

      reference & operator= (reference const & a_val)
      {
        TypeTraits::intType x = (a_val.m_rBucket >> a_val.m_bitIndex) & TypeTraits::intType(1);
        m_rBucket = m_rBucket & ~(TypeTraits::intType(1) << m_bitIndex) | (x << m_bitIndex);
        return *this;
      }

    private:

      TypeTraits::intType &  m_rBucket;
      int       m_bitIndex;
    };

  public:

    DynamicArray()
      : m_pBuckets(nullptr)
      , m_nItems(0)
    {
      m_pBuckets = static_cast<TypeTraits::intType*>(malloc(m_poolSize.GetSize() * sizeof(TypeTraits::intType)));
      if (m_pBuckets == nullptr)
        throw std::bad_alloc();
    }

    //! Construct with a set size
    DynamicArray(TypeTraits::intType a_size)
      : m_pBuckets(nullptr)
      , m_nItems(0)
    {
      m_poolSize.SetSize(a_size);
      m_pBuckets = static_cast<TypeTraits::intType*>(malloc(m_poolSize.GetSize() * sizeof(TypeTraits::intType)));
      if (m_pBuckets == nullptr)
        throw std::bad_alloc();
    }

    ~DynamicArray()
    {
      free(m_pBuckets);
    }

    //! Copy constructor
    DynamicArray(DynamicArray const & a_other)
      : m_poolSize(a_other.m_poolSize)
      , m_pBuckets(nullptr)
    {
      init(a_other);
    }

    //! Assignment
    DynamicArray& operator= (DynamicArray const & a_other)
    {
      if (this == &a_other)
        return *this;

      clear();
      m_poolSize = a_other.m_poolSize;
      init(a_other);

      return *this;
    }

    //! Move constructor
    DynamicArray(DynamicArray && a_other) noexcept
      : m_poolSize(a_other.m_poolSize)
      , m_pBuckets(a_other.m_pBuckets)
      , m_nItems(a_other.m_nItems)
    {
      a_other.m_pBuckets = nullptr;
      a_other.m_nItems = 0;
    }

    //! Move assignment
    DynamicArray& operator= (DynamicArray && a_other) noexcept
    {
      if (this != &a_other)
      {
        //Assign to this
        m_nItems = a_other.m_nItems;
        m_pBuckets = a_other.m_pBuckets;
        m_poolSize = a_other.m_poolSize;

        //Clear other
        a_other.m_pBuckets = nullptr;
        a_other.m_nItems = 0;
      }
      return *this;
    }

    //! Access element
    bool operator[](TypeTraits::intType i) const
    {
      TypeTraits::intType bucket(i >> TypeTraits::shift);
      TypeTraits::intType n = i & TypeTraits::mask;

      return ((m_pBuckets[bucket] >> n) & TypeTraits::intType(1)) != 0;
    }

    //! Access element
    reference operator[](TypeTraits::intType i)
    {
      TypeTraits::intType bucket(i >> TypeTraits::shift);
      TypeTraits::intType n = i & TypeTraits::mask;

      return reference(m_pBuckets[bucket], static_cast<int>(n));
    }

    void Set(TypeTraits::intType i, bool a_val)
    {
      TypeTraits::intType bucket(i >> TypeTraits::shift);
      TypeTraits::intType n = i & TypeTraits::mask;
      TypeTraits::intType x = (a_val) ? 1 : 0;

      m_pBuckets[bucket] = (m_pBuckets[bucket] & ~(TypeTraits::intType(1) << n)) | (x << n);
    }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to item in the DynamicArray
    bool back() { return this->operator[](m_nItems - 1); }
    reference back() const 
    { 
      TypeTraits::intType i = m_nItems - 1;
      TypeTraits::intType bucket(i >> TypeTraits::shift);
      TypeTraits::intType n = i & TypeTraits::mask;

      return reference(m_pBuckets[bucket], static_cast<int>(n)); 
    }

    //! Current size of the array
    TypeTraits::intType size()		const { return m_nItems; }

    //! Is the array empty
    bool empty()		const { return m_nItems == 0; }

    //! Add element to the back of the array.
    void push_back(bool a_val)
    {
      if (m_nItems == m_poolSize.GetSize() * (TypeTraits::nBits))
      {
        extend();
      }

      Set(m_nItems, a_val);
      m_nItems++;
    }

    //! Remove element from the back of the array.
    void pop_back()
    {
      m_nItems--;
    }

    //! Current size is set to 0
    void clear()
    {
      m_nItems = 0;
    }

    //! Set the current size to 0 and the reserve to new_size
    void resize(TypeTraits::intType newSize)
    {
      newSize = newSize >> TypeTraits::shift;
      newSize = m_poolSize.SetSize(newSize);
      TypeTraits::intType * tempBuckets = static_cast<TypeTraits::intType*>(realloc(m_pBuckets, m_poolSize.GetSize() * sizeof(TypeTraits::intType)));
      if (tempBuckets == nullptr)
        throw std::bad_alloc();
      m_pBuckets = tempBuckets;
    }

  private:

    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend()
    {
      m_poolSize.SetNextPoolSize();
      TypeTraits::intType* tempBuckets = static_cast<TypeTraits::intType*>(realloc(m_pBuckets, m_poolSize.GetSize() * sizeof(TypeTraits::intType)));
      if (tempBuckets == nullptr)
        throw std::bad_alloc();
      m_pBuckets = tempBuckets;
    }

    void init(DynamicArray const & a_other)
    {
      resize(a_other.m_poolSize.GetSize() << TypeTraits::shift);
      TypeTraits::intType sze = ((a_other.m_nItems + 7) >> 3);
      memcpy(m_pBuckets, a_other.m_pBuckets, sze);
      m_nItems = a_other.m_nItems;
    }

  private:
    //Data members
    PoolSizeMngr_Default          m_poolSize;
    TypeTraits::intType *    m_pBuckets;
    TypeTraits::intType      m_nItems;
  };
};
#endif