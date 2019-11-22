#ifndef DGSLOTMAP_H
#define DGSLOTMAP_H

#include <cstdlib>
#include <new>
#include <cstring>
#include <exception>

#include "impl/DgPoolSizeManager.h"

namespace Dg
{
  template<typename T>
  class SlotMap
  {
    static size_t const s_default_capacity = 1024;
    static size_t const INVALID_VALUE = 0xFFFFFFFFFFFFFFFF;

  public:

    class const_iterator
    {
    private:
      friend class SlotMap;

    private:
      //! Special constructor, not for external use
      const_iterator(T const *);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const& a_it);
      const_iterator& operator= (const_iterator const&);

      bool operator==(const_iterator const& a_it) const;
      bool operator!=(const_iterator const& a_it) const;

      const_iterator operator+(size_t) const;
      const_iterator operator-(size_t) const;

      const_iterator& operator+=(size_t);
      const_iterator& operator-=(size_t);

      const_iterator& operator++();
      const_iterator operator++(int);
      const_iterator& operator--();
      const_iterator operator--(int);

      T const* operator->() const;
      T const& operator*() const;

    private:
      T const * m_pData;
    };


    //! @class iterator
    //!
    //! Iterator for the SlotMap.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
    {
    private:
      friend class SlotMap;

    private:
      //! Special constructor, not for external use
      iterator(T *);

    public:

      iterator();
      ~iterator();

      iterator(iterator const& a_it);
      iterator& operator= (iterator const&);

      bool operator==(iterator const& a_it) const;
      bool operator!=(iterator const& a_it) const;

      iterator operator+(size_t) const;
      iterator operator-(size_t) const;

      iterator& operator+=(size_t);
      iterator& operator-=(size_t);

      iterator& operator++();
      iterator operator++(int);
      iterator& operator--();
      iterator operator--(int);

      T* operator->();
      T& operator*();

      operator const_iterator() const;

    private:
      T * m_pData;
    };

  public:

    struct Key
    {
      friend SlotMap;
    private:
    
      size_t index;
      size_t generation;
    };

  public:
  
    SlotMap();
    SlotMap(size_t capacity);

    ~SlotMap();

    SlotMap(SlotMap const&);
    SlotMap& operator=(SlotMap const&);

    SlotMap(SlotMap&&)  noexcept;
    SlotMap& operator=(SlotMap&&)  noexcept;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    T & operator[](size_t);
    T const & operator[](size_t) const;
  
    size_t size() const;
    void clear();

    Key insert(T const &);
    void erase(Key const &);

  private:
  
    void Extend();
    void Init(SlotMap const &);
    void Init(size_t);
    void InitMemory(size_t);
    void DestructAll();

  private:

    struct Ind
    {
      size_t index;
      size_t generation;
      size_t next;
    };

    size_t m_nItems;

    PoolSizeManager m_poolSize;

    size_t m_freeListHead;
    size_t m_freeListTail;

    Ind *     m_pIndices;
    T *       m_pData;
    size_t *  m_pEraseTable;
  };


  //--------------------------------------------------------------------------------
  //		const_iterator
  //--------------------------------------------------------------------------------
  template<typename T>
  SlotMap<T>::const_iterator::const_iterator(T const * a_pData)
    : m_pData(a_pData)
  {

  }

  template<typename T>
  SlotMap<T>::const_iterator::const_iterator()
    : m_pData(nullptr)
  {

  }

  template<typename T>
  SlotMap<T>::const_iterator::~const_iterator()
  {

  }

  template<typename T>
  SlotMap<T>::const_iterator::const_iterator(const_iterator const& a_it)
    : m_pData(a_it.m_pData)
  {

  }

  template<typename T>
  typename SlotMap<T>::const_iterator&
    SlotMap<T>::const_iterator::operator=(const_iterator const& a_other)
  {
    m_pData = a_other.m_pData;
    return *this;
  }

  template<typename T>
  bool SlotMap<T>::const_iterator::operator==(const_iterator const& a_it) const
  {
    return m_pData == a_it.m_pData;
  }

  template<typename T>
  bool SlotMap<T>::const_iterator::operator!=(const_iterator const& a_it) const
  {
    return m_pData != a_it.m_pData;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator
    SlotMap<T>::const_iterator::operator+(size_t a_val) const
  {
    return const_iterator(m_pData + a_val);
  }

  template<typename T>
  typename SlotMap<T>::const_iterator
    SlotMap<T>::const_iterator::operator-(size_t a_val) const
  {
    return const_iterator(m_pData - a_val);
  }

  template<typename T>
  typename SlotMap<T>::const_iterator&
    SlotMap<T>::const_iterator::operator+=(size_t a_val)
  {
    m_pData += a_val;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator&
    SlotMap<T>::const_iterator::operator-=(size_t a_val)
  {
    m_pData -= a_val;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator&
    SlotMap<T>::const_iterator::operator++()
  {
    m_pData++;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator
    SlotMap<T>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator&
    SlotMap<T>::const_iterator::operator--()
  {
    m_pData--;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::const_iterator
    SlotMap<T>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T const *
    SlotMap<T>::const_iterator::operator->() const
  {
    return m_pData;
  }

  template<typename T>
  T const &
    SlotMap<T>::const_iterator::operator*() const
  {
    return *m_pData;
  }

  //--------------------------------------------------------------------------------
  //		iterator
  //--------------------------------------------------------------------------------
  template<typename T>
  SlotMap<T>::iterator::iterator(T * a_pData)
    : m_pData(a_pData)
  {

  }

  template<typename T>
  SlotMap<T>::iterator::iterator()
    : m_pData(nullptr)
  {

  }

  template<typename T>
  SlotMap<T>::iterator::~iterator()
  {

  }

  template<typename T>
  SlotMap<T>::iterator::iterator(iterator const& a_it)
    : m_pData(a_it.m_pData)
  {

  }

  template<typename T>
  typename SlotMap<T>::iterator&
    SlotMap<T>::iterator::operator=(iterator const& a_other)
  {
    m_pData = a_other.m_pData;
    return *this;
  }

  template<typename T>
  bool SlotMap<T>::iterator::operator==(iterator const& a_it) const
  {
    return m_pData == a_it.m_pData;
  }

  template<typename T>
  bool SlotMap<T>::iterator::operator!=(iterator const& a_it) const
  {
    return m_pData != a_it.m_pData;
  }

  template<typename T>
  typename SlotMap<T>::iterator
    SlotMap<T>::iterator::operator+(size_t a_val) const
  {
    return iterator(m_pData + a_val);
  }

  template<typename T>
  typename SlotMap<T>::iterator
    SlotMap<T>::iterator::operator-(size_t a_val) const
  {
    return iterator(m_pData - a_val);
  }

  template<typename T>
  typename SlotMap<T>::iterator&
    SlotMap<T>::iterator::operator+=(size_t a_val)
  {
    m_pData += a_val;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::iterator&
    SlotMap<T>::iterator::operator-=(size_t a_val)
  {
    m_pData -= a_val;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::iterator&
    SlotMap<T>::iterator::operator++()
  {
    m_pData++;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::iterator
    SlotMap<T>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename SlotMap<T>::iterator&
    SlotMap<T>::iterator::operator--()
  {
    m_pData--;
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::iterator
    SlotMap<T>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T*
    SlotMap<T>::iterator::operator->()
  {
    return m_pData;
  }

  template<typename T>
  T&
    SlotMap<T>::iterator::operator*()
  {
    return *m_pData;
  }

  template<typename T>
  SlotMap<T>::iterator::operator
    typename SlotMap<T>::const_iterator() const
  {
    return const_iterator(m_pData);
  }

  //--------------------------------------------------------------------------------
  //		SlotMap
  //--------------------------------------------------------------------------------

  template<typename T>
  SlotMap<T>::SlotMap()
    : m_poolSize(s_default_capacity)
    , m_nItems(0)
    , m_freeListHead(INVALID_VALUE)
    , m_freeListTail(INVALID_VALUE)
    , m_pIndices(nullptr)
    , m_pData(nullptr)
    , m_pEraseTable(nullptr)
  {
    Init(s_default_capacity);
  }

  template<typename T>
  SlotMap<T>::SlotMap(size_t a_capacity)
    : m_poolSize(a_capacity)
    , m_nItems(0)
    , m_freeListHead(INVALID_VALUE)
    , m_freeListTail(INVALID_VALUE)
    , m_pIndices(nullptr)
    , m_pData(nullptr)
    , m_pEraseTable(nullptr)
  {
    Init(a_capacity);
  }

  template<typename T>
  SlotMap<T>::~SlotMap()
  {
    DestructAll();
    free(m_pIndices);
    free(m_pData);
    free(m_pEraseTable);
  }

  template<typename T>
  SlotMap<T>::SlotMap(SlotMap const & a_other)
    : m_poolSize(s_default_capacity)
    , m_nItems(0)
    , m_freeListHead(INVALID_VALUE)
    , m_freeListTail(INVALID_VALUE)
    , m_pIndices(nullptr)
    , m_pData(nullptr)
    , m_pEraseTable(nullptr)
  {
    Init(a_other);
  }

  template<typename T>
  SlotMap<T> & SlotMap<T>::operator=(SlotMap const & a_other)
  {
    if (this != &a_other)
      Init(a_other);
    return *this;
  }

  //TODO Fix all move operators to look like these:
  template<typename T>
  SlotMap<T>::SlotMap(SlotMap && a_other)  noexcept
    : m_poolSize(a_other.m_poolSize.GetSize())
    , m_nItems(a_other.m_nItems)
    , m_freeListHead(a_other.m_freeListHead)
    , m_freeListTail(a_other.m_freeListTail)
    , m_pIndices(a_other.m_pIndices)
    , m_pData(a_other.m_pData)
    , m_pEraseTable(a_other.m_pEraseTable)
  {
    a_other.m_nItems = 0;
    a_other.m_freeListHead = INVALID_VALUE;
    a_other.m_freeListTail = INVALID_VALUE;
    a_other.m_pIndices = nullptr;
    a_other.m_pData = nullptr;
    a_other.m_pEraseTable = nullptr;
  }

  template<typename T>
  SlotMap<T> & SlotMap<T>::operator=(SlotMap && a_other)  noexcept
  {
    if (this != &a_other)
    {
      DestructAll();

      m_poolSize = a_other.m_poolSize;
      m_nItems = a_other.m_nItems;
      m_freeListHead = a_other.m_freeListHead;
      m_freeListTail = a_other.m_freeListTail;
      m_pIndices = a_other.m_pIndices;
      m_pData = a_other.m_pData;
      m_pEraseTable = a_other.m_pEraseTable;

      a_other.m_nItems = 0;
      a_other.m_freeListHead = INVALID_VALUE;
      a_other.m_freeListTail = INVALID_VALUE;
      a_other.m_pIndices = nullptr;
      a_other.m_pData = nullptr;
      a_other.m_pEraseTable = nullptr;
    }
    return *this;
  }

  template<typename T>
  typename SlotMap<T>::iterator SlotMap<T>::begin()
  {
    return iterator(&m_pData[0]);
  }

  template<typename T>
  typename SlotMap<T>::iterator SlotMap<T>::end()
  {
    return iterator(&m_pData[m_nItems]);
  }

  template<typename T>
  typename SlotMap<T>::const_iterator SlotMap<T>::cbegin() const
  {
    return const_iterator(&m_pData[0]);
  }

  template<typename T>
  typename SlotMap<T>::const_iterator SlotMap<T>::cend() const
  {
    return const_iterator(&m_pData[m_nItems]);
  }

  template<typename T>
  T& SlotMap<T>::operator[](size_t a_index)
  {
    return m_pData[a_index];
  }

  template<typename T>
  T const & SlotMap<T>::operator[](size_t a_index) const
  {
    return m_pData[a_index];
  }

  template<typename T>
  typename SlotMap<T>::Key SlotMap<T>::insert(T const & a_item)
  {
    if ((m_nItems + 1) == m_poolSize.GetSize())
      Extend();

    size_t ind = m_freeListHead;
    m_freeListHead = m_pIndices[m_freeListHead].next;

    m_pIndices[ind].index = m_nItems;
    m_pIndices[ind].generation++;
    m_pIndices[ind].next = INVALID_VALUE;

    m_pEraseTable[m_nItems] = ind;

    new(&m_pData[m_nItems]) T(a_item);
    m_nItems++;

    Key result;
    result.index = ind;
    result.generation = m_pIndices[ind].generation;
    return result;
  }

  template<typename T>
  void SlotMap<T>::erase(Key const & a_key)
  {
    if (m_pIndices[a_key.index].generation != a_key.generation)
      return;

    size_t dataInd = m_pIndices[a_key.index].index;
    m_pData[dataInd].~T();

    if (dataInd + 1 != m_nItems)
    {
      memcpy(&m_pData[dataInd], &m_pData[m_nItems - 1], sizeof(T));
      m_pEraseTable[dataInd] = m_pEraseTable[m_nItems - 1];
      m_pIndices[m_pEraseTable[dataInd]].index = dataInd;
    }

    m_pIndices[a_key.index].generation++;
    m_pIndices[a_key.index].index = INVALID_VALUE;
    m_pIndices[a_key.index].next = INVALID_VALUE;
    m_pIndices[m_freeListTail].next = a_key.index;
    m_freeListTail = a_key.index;

    m_nItems--;
  }

  template<typename T>
  size_t SlotMap<T>::size() const
  {
    return m_nItems;
  }

  template<typename T>
  void SlotMap<T>::clear()
  {
    DestructAll();

    for (size_t i = 0; i < m_poolSize.GetSize(); i++)
    {
      m_pIndices[i] ={INVALID_VALUE, 0, i + 1};
      m_pEraseTable[i] = INVALID_VALUE;
    }
    m_pIndices[m_poolSize.GetSize() - 1].next = INVALID_VALUE;

    m_nItems = 0;
  }

  template<typename T>
  void SlotMap<T>::Extend()
  {
    Ind* tempIndices = static_cast<Ind*>(malloc(sizeof(Ind) * m_poolSize.PeekNextPoolSize()));
    T* tempData = static_cast<T*>(malloc(sizeof(T) * m_poolSize.PeekNextPoolSize()));
    size_t* tempEraseTable = static_cast<size_t*>(malloc(sizeof(size_t) * m_poolSize.PeekNextPoolSize()));

    if (tempIndices == nullptr
      || tempData == nullptr
      || tempEraseTable == nullptr)
    {
      free(tempIndices);
      free(tempData);
      free(tempEraseTable);

      throw std::exception("SlotMap failed to allocate memory");
    }

    m_poolSize.SetNextPoolSize();

    memcpy(tempIndices, m_pIndices, sizeof(Ind) * m_nItems);
    memcpy(tempData, m_pData, sizeof(T) * m_nItems);
    memcpy(tempEraseTable, m_pEraseTable, sizeof(size_t) * m_nItems);

    free(m_pIndices);
    free(m_pData);
    free(m_pEraseTable);

    m_pIndices = tempIndices;
    m_pData = tempData;
    m_pEraseTable = tempEraseTable;

    for (size_t i = m_nItems; i < m_poolSize.GetSize(); i++)
    {
      m_pIndices[i].index = INVALID_VALUE;
      m_pIndices[i].generation = 0;
      m_pIndices[i].next = i + 1;
      m_pEraseTable[i] = INVALID_VALUE;
    }
    m_pIndices[m_poolSize.GetSize() - 1].next = INVALID_VALUE;
    m_freeListTail = m_poolSize.GetSize() - 1;
  }

  template<typename T>
  void SlotMap<T>::Init(SlotMap const & a_other)
  {
    InitMemory(a_other.m_poolSize.GetSize());

    m_poolSize = a_other.m_poolSize;
    m_nItems = a_other.m_nItems;
    m_freeListHead = a_other.m_freeListHead;
    m_freeListTail = a_other.m_freeListTail;

    memcpy(m_pIndices, a_other.m_pIndices, sizeof(Ind) * m_poolSize.GetSize());
    memcpy(m_pEraseTable, a_other.m_pEraseTable, sizeof(size_t) * m_poolSize.GetSize());
    for (size_t i = 0; i < m_nItems; i++)
      new(&m_pData[i]) T(a_other.m_pData[i]);
  }

  template<typename T>
  void SlotMap<T>::Init(size_t a_size)
  {
    PoolSizeManager szeMgr(a_size);
    InitMemory(szeMgr.GetSize());

    m_poolSize = szeMgr;
    m_freeListHead = 0;
    m_freeListTail = m_poolSize.GetSize() - 1;
    m_nItems = 0;

    for (size_t i = 0; i < m_poolSize.GetSize(); i++)
    {
      m_pIndices[i] = {INVALID_VALUE, 0, i + 1};
      m_pEraseTable[i] = INVALID_VALUE;
    }
    m_pIndices[m_poolSize.GetSize() - 1].next = INVALID_VALUE;
  }

  template<typename T>
  void SlotMap<T>::InitMemory(size_t a_size)
  {
    Ind* tempIndices = static_cast<Ind*>(malloc(sizeof(Ind) * a_size));
    T* tempData = static_cast<T*>(malloc(sizeof(T) * a_size));
    size_t* tempEraseTable = static_cast<size_t*>(malloc(sizeof(size_t) * a_size));

    if (tempIndices == nullptr
      || tempData == nullptr
      || tempEraseTable == nullptr)
    {
      free(tempIndices);
      free(tempData);
      free(tempEraseTable);

      throw std::exception("SlotMap failed to allocate memory");
    }

    DestructAll();

    free(m_pIndices);
    free(m_pData);
    free(m_pEraseTable);

    m_pIndices = tempIndices;
    m_pData = tempData;
    m_pEraseTable = tempEraseTable;
  }

  template<typename T>
  void SlotMap<T>::DestructAll()
  {
    for (size_t i = 0; i < m_nItems; i++)
      m_pData[i].~T();
  }
}

#endif