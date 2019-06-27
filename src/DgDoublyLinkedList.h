//! @file Dg_list.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: DoublyLinkedList

// old length : 855 lines

#ifndef DGDOUBLYLINKEDLIST_H
#define DGDOUBLYLINKEDLIST_H

#include <cstring>
#include <new>
#include <type_traits>
#include <exception>

#include "impl/DgContainerBase.h"

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class DoublyLinkedList
  //!
  //! Circular, pre-allocated doubly linked DoublyLinkedList. Similar to std::list with similarly named methods
  //! and functionality. The underlying array is preallocated and only change in
  //! size if extending DoublyLinkedList past that allocated, or manually resizing. This makes
  //! for fast insertion/erasing of elements.
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename T>
  class DoublyLinkedList : public ContainerBase
  {
  private:
    
	  struct Node
	  {
		  Node* pNext;
		  Node* pPrev;
	  };

  public:

    //! @class const_iterator
    //!
    //! Const iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
	  {
	  private:
		  friend class DoublyLinkedList;
		
	  private:
		  //! Special constructor, not for external use
		  const_iterator(Node const * pNode, Node const * pOffset, T const * pData);

	  public:

		  const_iterator();
		  ~const_iterator();

		  const_iterator(const_iterator const & a_it);
		  const_iterator& operator= (const_iterator const &);
		
		  bool operator==(const_iterator const & a_it) const;
		  bool operator!=(const_iterator const & a_it) const;

		  const_iterator& operator++();
		  const_iterator operator++(int);
		  const_iterator& operator--();
		  const_iterator operator--(int);

		  T const * operator->() const;
		  T const & operator*() const;

	  private:
      Node const * m_pNode;
      Node const * m_pOffset;
      T const * m_pData;
	  };


    //! @class iterator
    //!
    //! Iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
    {
    private:
      friend class DoublyLinkedList;

    private:
      //! Special constructor, not for external use
      iterator(Node * pNode, Node * pNodeBegin, T * pData);

    public:

      iterator();
      ~iterator();

      iterator(iterator const & a_it);
      iterator& operator= (iterator const &);

      bool operator==(iterator const & a_it) const;
      bool operator!=(iterator const & a_it) const;

      iterator& operator++();
      iterator operator++(int);
      iterator& operator--();
      iterator operator--(int);

      T * operator->();
      T & operator*();

      operator const_iterator() const;

    private:
      Node * m_pNode;
      Node * m_pOffset;
      T * m_pData;
    };

  public:

    DoublyLinkedList();
	  DoublyLinkedList(size_t memBlockSize);

	  ~DoublyLinkedList();

	  DoublyLinkedList(DoublyLinkedList const &);
	  DoublyLinkedList & operator=(DoublyLinkedList const &);

    DoublyLinkedList(DoublyLinkedList &&);
    DoublyLinkedList & operator=(DoublyLinkedList &&);

	  //! Returns an iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator begin();
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator end();
	  
    //! Returns a const iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cbegin() const;
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cend() const;
	  
    //! Returns number of elements in the DoublyLinkedList.
    size_t size() const;
	  
    //! Returns if the DoublyLinkedList is empty.
	  bool empty() const;

    //! Returns a reference to the last data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & back();
     
    //! Returns a reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & front();

    //! Returns a const reference to the last data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & back() const;

    //! Returns a const reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & front() const;

	  //! Add an data to the back of the DoublyLinkedList
    void push_back(T const &);

    //!Add an data to the front of the DoublyLinkedList
    void push_front(T const &);
    
    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & position, T const & item);

    //! Erase last data
	  void pop_back();

    //! Erase last data
	  void pop_front();

    //! Erase an data from the DoublyLinkedList
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
	  iterator erase(iterator const & position);
	  
    //! Clear the DoublyLinkedList, retains allocated memory.
    void clear();

    //! Resizes the DoublyLinkedList. This function also clears the DoublyLinkedList.
	  void resize(size_t newMemBlockSize);

  private:

    // Increases the size of the underlying memory block
    void Extend();
    Node * InsertNewAfter(Node * a_pNode, T const & a_data);
    void DestructAll();
    void InitMemory();
    void Init(DoublyLinkedList const & a_other);
    void InitEndNode();

    Node * Remove(Node * a_pNode);

    T * GetDataFromNode(Node * a_pNode);
    T const * GetDataFromNode(Node * a_pNode) const;

  private:

	  Node *    m_pNodes;      //Pre-allocated block of memory to hold items
    T    *    m_pData;
	  size_t    m_nItems;     //Number of items currently in the DoublyLinkedList
  };

  //--------------------------------------------------------------------------------
  //		const_iterator
  //--------------------------------------------------------------------------------
  template<typename T>
  DoublyLinkedList<T>::const_iterator::const_iterator(Node const * a_pNode, 
                                                      Node const * a_pNodeBegin, 
                                                      T const * a_pData)
  : m_pNode(a_pNode)
    , m_pOffset(a_pNodeBegin)
    , m_pData(a_pData)
  {

  }
  
  template<typename T>
  DoublyLinkedList<T>::const_iterator::const_iterator()
    : m_pNode(nullptr) 
    , m_pOffset(nullptr)
    , m_pData(nullptr)
  {

  }

  template<typename T>
  DoublyLinkedList<T>::const_iterator::~const_iterator()
  {

  }

  template<typename T>
  DoublyLinkedList<T>::const_iterator::const_iterator(const_iterator const & a_it)
    : m_pNode(a_it.m_pNode)
    , m_pData(a_it.m_pData)
    , m_pOffset(a_it.m_pOffset)
  {

  }

  template<typename T>
  typename DoublyLinkedList<T>::const_iterator &
    DoublyLinkedList<T>::const_iterator::operator=(const_iterator const & a_other)
  {
    m_pNode = a_other.m_pNode;
    m_pOffset = a_other.m_pOffset;
    m_pData = a_other.m_pData;
    return *this;
  }

  template<typename T>
  bool DoublyLinkedList<T>::const_iterator::operator==(const_iterator const & a_it) const 
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename T>
  bool DoublyLinkedList<T>::const_iterator::operator!=(const_iterator const & a_it) const 
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename T>
  typename DoublyLinkedList<T>::const_iterator &
    DoublyLinkedList<T>::const_iterator::operator++()
  {
    m_pNode = m_pNode->pNext;
    return *this;
  }

  template<typename T>
  typename DoublyLinkedList<T>::const_iterator
    DoublyLinkedList<T>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename DoublyLinkedList<T>::const_iterator &
    DoublyLinkedList<T>::const_iterator::operator--()
  {
    m_pNode = m_pNode->pPrev;
    return *this;
  }

  template<typename T>
  typename DoublyLinkedList<T>::const_iterator
    DoublyLinkedList<T>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T const *
    DoublyLinkedList<T>::const_iterator::operator->() const 
  {
    return m_pData + (m_pNode - m_pOffset);
  }

  template<typename T>
  T const &
    DoublyLinkedList<T>::const_iterator::operator*() const 
  {
    return *(m_pData + (m_pNode - m_pOffset));
  }

  //--------------------------------------------------------------------------------
  //		iterator
  //--------------------------------------------------------------------------------
  template<typename T>
  DoublyLinkedList<T>::iterator::iterator(Node * a_pNode, 
                                          Node* a_pOffset, 
                                          T * a_pData)
    : m_pNode(a_pNode)
    , m_pOffset(a_pOffset)
    , m_pData(a_pData)
  {

  }

  template<typename T>
  DoublyLinkedList<T>::iterator::iterator()
    : m_pNode(nullptr) 
    , m_pOffset(nullptr)
    , m_pData(nullptr)
  {

  }

  template<typename T>
  DoublyLinkedList<T>::iterator::~iterator()
  {

  }

  template<typename T>
  DoublyLinkedList<T>::iterator::iterator(iterator const & a_it)
    : m_pNode(a_it.m_pNode)
    , m_pData(a_it.m_pData)
    , m_pOffset(a_it.m_pOffset)
  {

  }

  template<typename T>
  typename DoublyLinkedList<T>::iterator &
    DoublyLinkedList<T>::iterator::operator=(iterator const & a_other)
  {
    m_pNode = a_other.m_pNode;
    m_pOffset = a_other.m_pOffset;
    m_pData = a_other.m_pData;
    return *this;
  }

  template<typename T>
  bool DoublyLinkedList<T>::iterator::operator==(iterator const & a_it) const 
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename T>
  bool DoublyLinkedList<T>::iterator::operator!=(iterator const & a_it) const 
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename T>
  typename DoublyLinkedList<T>::iterator &
    DoublyLinkedList<T>::iterator::operator++()
  {
    m_pNode = m_pNode->pNext;
    return *this;
  }

  template<typename T>
  typename DoublyLinkedList<T>::iterator
    DoublyLinkedList<T>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename T>
  typename DoublyLinkedList<T>::iterator &
    DoublyLinkedList<T>::iterator::operator--()
  {
    m_pNode = m_pNode->pPrev;
    return *this;
  }

  template<typename T>
  typename DoublyLinkedList<T>::iterator
    DoublyLinkedList<T>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename T>
  T *
    DoublyLinkedList<T>::iterator::operator->()
  {
    return m_pData + (m_pNode - m_pOffset);
  }

  template<typename T>
  T &
    DoublyLinkedList<T>::iterator::operator*()
  {
    return *(m_pData + (m_pNode - m_pOffset));
  }

  template<typename T>
  DoublyLinkedList<T>::iterator::operator
    typename DoublyLinkedList<T>::const_iterator() const
  {
    return const_iterator(m_pNode);
  }

  //--------------------------------------------------------------------------------
  //		DoublyLinkedList
  //--------------------------------------------------------------------------------
  template<typename T>
   DoublyLinkedList<T>::DoublyLinkedList()
    : ContainerBase()
    , m_nItems(0)
    , m_pNodes(nullptr)
    , m_pData(nullptr)
  {
    InitMemory();
    InitEndNode();
  }

   template<typename T>
   DoublyLinkedList<T>::DoublyLinkedList(size_t a_size)
     : ContainerBase(a_size)
     , m_nItems(0)
     , m_pNodes(nullptr)
     , m_pData(nullptr)
   {
     InitMemory();
     InitEndNode();
   }

   template<typename T>
   DoublyLinkedList<T>::~DoublyLinkedList()
   {
     DestructAll();
     free(m_pData);
     free(m_pNodes);
   }

   template<typename T>
   DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList const & a_other)
     : ContainerBase(a_other)
     , m_nItems(0)
     , m_pNodes(nullptr)
     , m_pData(nullptr)
   {
     InitMemory();
     Init(a_other);
   }

   template<typename T>
   DoublyLinkedList<T> & DoublyLinkedList<T>::operator=(DoublyLinkedList const & a_other)
   {
     if (this != &a_other)
     {
       DestructAll();

       if (pool_size() < a_other.pool_size())
       {
         pool_size(a_other.pool_size());
         InitMemory();
       }

       Init(a_other);
     }
     return *this;
   }

   template<typename T>
   DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList && a_other)
     : ContainerBase(a_other)
     , m_nItems(a_other.m_nItems)
     , m_pNodes(a_other.m_pNodes)
     , m_pData(a_other.m_pData)
   {
     a_other.m_pNodes = nullptr;
     a_other.m_pData = nullptr;
     a_other.m_nItems = 0;
   }

   template<typename T>
   DoublyLinkedList<T> & DoublyLinkedList<T>::operator=(DoublyLinkedList && a_other)
   {
     if (this != &a_other)
     {
       ContainerBase::operator=(a_other);
       m_pData = a_other.m_pData;
       m_pNodes = a_other.m_pNodes;
       m_nItems = a_other.m_nItems;

       a_other.m_pNodes = nullptr;
       a_other.m_pData = nullptr;
       a_other.m_nItems = 0;
     }
     return *this;
   }

   template<typename T>
   typename DoublyLinkedList<T>::iterator 
     DoublyLinkedList<T>::begin() 
   {
     return iterator(m_pNodes[0].pNext, m_pNodes + 1, m_pData);
   }

   template<typename T>
   typename DoublyLinkedList<T>::iterator
     DoublyLinkedList<T>::end() 
   {
     return iterator(&m_pNodes[0], m_pNodes + 1, m_pData); 
   }

   template<typename T>
   typename DoublyLinkedList<T>::const_iterator
     DoublyLinkedList<T>::cbegin() const 
   {
     return const_iterator(m_pNodes[0].pNext, m_pNodes + 1, m_pData);
   }

   template<typename T>
   typename DoublyLinkedList<T>::const_iterator
     DoublyLinkedList<T>::cend() const 
   {
     return const_iterator(&m_pNodes[0], m_pNodes + 1, m_pData); 
   }

   template<typename T>
   size_t DoublyLinkedList<T>::size() const 
   {
     return m_nItems;
   }

   template<typename T>
   bool DoublyLinkedList<T>::empty() const 
   {
     return m_nItems == 0;
   }

   template<typename T>
   T & DoublyLinkedList<T>::back() 
   { 
     return *GetDataFromNode(m_pNodes[0].pPrev);
   }

   template<typename T>
   T & DoublyLinkedList<T>::front() 
   { 
     return *GetDataFromNode(m_pNodes[0].pNext); 
   }

   template<typename T>
   T const & DoublyLinkedList<T>::back() const 
   { 
     return GetDataFromNode(m_pNodes[0].pPrev);
   }

   template<typename T>
   T const & DoublyLinkedList<T>::front() const 
   { 
     return GetDataFromNode(m_pNodes[0].pNext); 
   }

   template<typename T>
   void DoublyLinkedList<T>::push_back(T const & a_item)
   {
     InsertNewAfter(m_pNodes[0].pPrev, a_item);
   }

   template<typename T>
   void DoublyLinkedList<T>::push_front(T const & a_item)
   {
     InsertNewAfter(&m_pNodes[0], a_item);
   }

   template<typename T>
   typename DoublyLinkedList<T>::iterator
     DoublyLinkedList<T>::insert(iterator const & a_position, T const & a_item)
   {
     Node * pNode = InsertNewAfter(a_position.m_pNode->pPrev, a_item);
     return iterator(pNode, m_pNodes + 1, m_pData);
   }

   template<typename T>
   void DoublyLinkedList<T>::pop_back()
   {
     Remove(m_pNodes[0].pPrev);
   }

   template<typename T>
   void DoublyLinkedList<T>::pop_front()
   {
     Remove(m_pNodes[0].pNext);
   }

   template<typename T>
   typename DoublyLinkedList<T>::iterator
     DoublyLinkedList<T>::erase(iterator const & a_position)
   {
     Node * pNode = Remove(a_position.m_pNode);
     return iterator(pNode, m_pNodes + 1, m_pData);
   }

   template<typename T>
   void DoublyLinkedList<T>::clear()
   {
     DestructAll();
     m_nItems = 0;
     InitEndNode();
   }

   template<typename T>
   void DoublyLinkedList<T>::resize(size_t a_newSize)
   {
     DestructAll();
     Init(a_newSize);
   }

   template<typename T>
   void DoublyLinkedList<T>::Extend()
   {
     Node * pOldNodes(m_pNodes);
     set_next_pool_size();

     m_pNodes = static_cast<Node *>(realloc(m_pNodes, (pool_size()) * sizeof(Node)));
     if (m_pNodes == nullptr)
       throw std::bad_alloc();

     m_pData = static_cast<T *>(realloc(m_pData, (pool_size()) * sizeof(T)));
     if (m_pData == nullptr)
       throw std::bad_alloc();

     if (pOldNodes != m_pNodes)
     {
       for (size_t i = 0; i <= m_nItems; i++)
       {
         m_pNodes[i].pPrev = m_pNodes + (m_pNodes[i].pPrev - pOldNodes);
         m_pNodes[i].pNext= m_pNodes + (m_pNodes[i].pNext - pOldNodes);
       }
     }
   }

   template<typename T>
   typename DoublyLinkedList<T>::Node *
     DoublyLinkedList<T>::InsertNewAfter(Node * a_pNode, T const & a_data)
   {
     if (m_nItems == (pool_size() - 1))
     {
       //Extending might invalidate a_pNode, so we need to record its index in the pool.
       size_t index(a_pNode - m_pNodes);
       Extend();
       a_pNode = &m_pNodes[index]; //Reset the pointer in case we have extended.
     }

     new (&m_pData[m_nItems]) T(a_data);
     m_nItems++;

     Node * newNode = &m_pNodes[m_nItems];
     newNode->pPrev = a_pNode;
     newNode->pNext = a_pNode->pNext;
     a_pNode->pNext->pPrev = newNode;
     a_pNode->pNext = newNode;

     return newNode;
   }

   template<typename T>
   void DoublyLinkedList<T>::DestructAll()
   {
     for (size_t i = 0; i < m_nItems; i++)
       m_pData[i].~T();
   }

   template<typename T>
   void DoublyLinkedList<T>::InitMemory()
   {
     m_pNodes = static_cast<Node*> (realloc(m_pNodes, pool_size() * sizeof(Node)));
     if (m_pNodes == nullptr)
       throw std::bad_alloc();

     m_pData = static_cast<T*> (realloc(m_pData, pool_size() * sizeof(T)));
     if (m_pData == nullptr)
       throw std::bad_alloc();
   }

   template<typename T>
   void DoublyLinkedList<T>::Init(DoublyLinkedList const & a_other)
   {
     m_nItems = a_other.m_nItems;

     //We might as well sort the data in list order as we copy.
     //It will cost us nothing.
     Node const * node = &a_other.m_pNodes[0];
     for (size_t i = 0; i < m_nItems; i++)
     {
       node = node->pNext;
       size_t indOther = (node - a_other.m_pNodes);
       new (&m_pData[i]) T(a_other.m_pData[indOther - 1]);
     }

     for (size_t i = 1; i <= m_nItems; i++)
       m_pNodes[i].pPrev = &m_pNodes[i - 1];

     for (size_t i = 0; i < m_nItems; i++)
       m_pNodes[i].pNext = &m_pNodes[i + 1];

     m_pNodes[0].pPrev = &m_pNodes[m_nItems];
     m_pNodes[m_nItems].pNext = m_pNodes;
   }

   template<typename T>
   void DoublyLinkedList<T>::InitEndNode()
   {
     m_pNodes[0].pNext = &m_pNodes[0];
     m_pNodes[0].pPrev = &m_pNodes[0];
   }

   template<typename T>
   typename DoublyLinkedList<T>::Node *
     DoublyLinkedList<T>::Remove(Node * a_pNode)
   {
     Node * pNext(a_pNode->pNext);

     //Break node form list
     a_pNode->pPrev->pNext = a_pNode->pNext;
     a_pNode->pNext->pPrev = a_pNode->pPrev;

     T * pData = GetDataFromNode(a_pNode);
     pData->~T();

     if (&m_pNodes[m_nItems] != a_pNode)
     {
       //Move last node to fill gap
       *a_pNode = m_pNodes[m_nItems];
       a_pNode->pPrev->pNext = a_pNode;
       a_pNode->pNext->pPrev = a_pNode;
       memcpy(pData, &m_pData[m_nItems - 1], sizeof (T));
     }

     if (pNext == &m_pNodes[m_nItems])
       pNext = a_pNode;

     m_nItems--;
     return pNext;
   }

   template<typename T>
   T * DoublyLinkedList<T>::GetDataFromNode(Node * a_pNode)
   {
     return m_pData + (a_pNode - m_pNodes - 1);
   }

   template<typename T>
   T const * DoublyLinkedList<T>::GetDataFromNode(Node * a_pNode) const
   {
     return m_pData + (a_pNode - m_pNodes - 1);
   }
};
#endif