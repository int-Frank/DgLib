//@group Collections

#ifndef DGSET_AVL_H
#define DGSET_AVL_H

#include <exception>
#include <new>
#include <cstring>
#include <exception>

#include "DgPair.h"
#include "impl/DgPoolSizeManager.h"
#include "impl/DgAVLTree_Common.h"

#ifdef DEBUG
#include <sstream>
#include <iostream>
#endif

namespace Dg
{
  //AVL tree implemented with an object pool. 
  //Objectives:
  // 1) Fast searching
  // 2) No memory allocation on insert. Only caveat is on extending the object pool
  // 3) No memory deallocation on erasing elements
  // 4) Fast iteration over elements if order is not important (iterator_rand)
  //An end node will follow the last element in the tree.
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &) = impl::Less<ValueType>>
  class Set_AVL
  {
  public:

    typedef impl::Node<ValueType> Node;

  private:

    typedef size_t sizeType;

    class EraseData
    {
    public:

      EraseData();

      Node * oldNodeAdd;
      Node * newNodeAdd;
      Node * pNext;
      bool   firstSuccDeleted;
    };

  public:

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class const_iterator_rand
    {
      friend class Set_AVL;
      friend class iterator_rand;

    private:

      const_iterator_rand(Node const *);

    public:

      const_iterator_rand();
      ~const_iterator_rand();

      const_iterator_rand(const_iterator_rand const & a_it);
      const_iterator_rand& operator= (const_iterator_rand const & a_other);

      bool operator==(const_iterator_rand const & a_it) const;
      bool operator!=(const_iterator_rand const & a_it) const;

      const_iterator_rand& operator++();
      const_iterator_rand operator++(int);
      const_iterator_rand& operator--();
      const_iterator_rand operator--(int);

      ValueType const * operator->() const;
      ValueType const & operator*() const;

    private:
      Node const *  m_pNode;
    };

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class iterator_rand
    {
      friend class Set_AVL;

    private:

      iterator_rand(Node *);

    public:

      iterator_rand();
      ~iterator_rand();

      iterator_rand(iterator_rand const & a_it);
      iterator_rand& operator= (iterator_rand const & a_other);

      bool operator==(iterator_rand const & a_it) const;
      bool operator!=(iterator_rand const & a_it) const;

      iterator_rand& operator++();
      iterator_rand operator++(int);
      iterator_rand& operator--();
      iterator_rand operator--(int);

      operator const_iterator_rand() const;

      ValueType const * operator->();
      ValueType const & operator*();

    private:
      Node * m_pNode;
    };

    //Iterates through the map as sorted by the criterion
    class const_iterator
    {
      friend class Set_AVL;
      friend class iterator;
    private:

      const_iterator(Node const *);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const & a_it);
      const_iterator& operator=(const_iterator const & a_other);

      bool operator==(const_iterator const & a_it) const;
      bool operator!=(const_iterator const & a_it) const;

      ValueType const * operator->() const;
      ValueType const & operator*() const;

      const_iterator operator+(size_t) const;
      const_iterator operator-(size_t) const;

      const_iterator & operator+=(size_t);
      const_iterator & operator-=(size_t);

      const_iterator & operator++();
      const_iterator operator++(int);
      const_iterator & operator--();
      const_iterator operator--(int);

    private:
      Node const * m_pNode;
    };

    //Iterates through the map as sorted by the criterion
    class iterator
    {
      friend class Set_AVL;
    private:

      iterator(Node *);

    public:

      iterator();
      ~iterator();

      iterator(iterator const & a_it);
      iterator& operator=(iterator const & a_other);

      bool operator==(iterator const & a_it) const;
      bool operator!=(iterator const & a_it) const;

      ValueType const * operator->();
      ValueType const & operator*();

      iterator operator+(size_t) const;
      iterator operator-(size_t) const;

      iterator & operator+=(size_t);
      iterator & operator-=(size_t);

      iterator & operator++();
      iterator operator++(int);
      iterator & operator--();
      iterator operator--(int);

      operator const_iterator() const;

    private:
      Node * m_pNode;
    };

  public:

    Set_AVL();
    Set_AVL(sizeType requestSize);
    ~Set_AVL();

    Set_AVL(Set_AVL const &);
    Set_AVL & operator=(Set_AVL const & a_other);

    Set_AVL(Set_AVL && a_other) noexcept;
    Set_AVL & operator=(Set_AVL && a_other) noexcept;

    sizeType size() const;
    bool empty() const;

    iterator_rand begin_rand();
    iterator_rand end_rand();
    const_iterator_rand cbegin_rand() const;
    const_iterator_rand cend_rand() const;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    //If the key already exists in the map, the data is 
    //inserted at this key
    iterator insert(ValueType const &);

    void erase(ValueType const &);

    //Returns an iterator to the element that follows the element removed
    //(or end(), if the last element was removed).
    iterator erase(iterator);

    //Searches the container for an element with a key equivalent to a_value and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    const_iterator find(ValueType const &) const;

    //Searches the container for an element with a key equivalent to a_value and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    iterator find(ValueType const &);

    bool exists(ValueType const &) const;

    void clear();
#ifdef DEBUG
  public:
    void Print() const;
  private:
    sizeType Ind(Node const *) const;
    std::string ToString(Node const *) const;
    void PrintNode(Node const *) const;
#endif
  private:

    void DestructAll();
    void InitMemory();
    void InitDefaultNode();
    void Init(Set_AVL const &);

    //Sets a_out to the node index which references the key, or
    //if the key does not exist, the node at which the key should be
    //added
    bool ValueExists(ValueType const & a_value, Node *& a_out) const;

    void Extend();
    int GetBalance(Node *) const;

    // A utility function to get height  
    // of the tree  
    int Height(Node *) const;
    Node * LeftRotate(Node *);
    Node * RightRotate(Node * a_y);

    //Constructs a new node after a_pParent. New node will point back to a_pParent.
    Node * NewNode(Node * pParent, ValueType const &);
    inline Node * EndNode();
    inline Node const * EndNode() const;

    //Returns the pointer to the new node.
    //Assumes tree has at least 1 element
    Node * __Insert(Node * pNode, Node * pParent,
                    ValueType const & data,
                    Node *& newNode);

    template<bool GetNext>
    Node * __Erase(Node * root, ValueType const &, EraseData &);

  private:

    PoolSizeMngr_Default m_poolSize;
    Node *          m_pRoot;
    Node *          m_pNodes;
    sizeType        m_nItems;
  };
  //------------------------------------------------------------------------------------------------
  // EraseData
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::EraseData::EraseData()
    : oldNodeAdd(nullptr)
    , newNodeAdd(nullptr)
    , pNext(nullptr)
    , firstSuccDeleted(false)
  {

  }

  //------------------------------------------------------------------------------------------------
  // const_iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator_rand::const_iterator_rand(Set_AVL<ValueType, Compare>::Node const * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator_rand::const_iterator_rand()
    : m_pNode(nullptr)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator_rand::~const_iterator_rand()
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator_rand::const_iterator_rand(const_iterator_rand const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator_rand & 
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator=(const_iterator_rand const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::const_iterator_rand::operator==(const_iterator_rand const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::const_iterator_rand::operator!=(const_iterator_rand const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator_rand & 
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator++()
  {
    m_pNode++;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator_rand
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator++(int)
  {
    const_iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator_rand & 
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator--()
  {
    m_pNode--;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator_rand
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator--(int)
  {
    const_iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const * 
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator->() const
  {
    return &(m_pNode->data);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const & 
    Set_AVL<ValueType, Compare>::const_iterator_rand::operator*() const
  {
    return m_pNode->data;
  }

  //------------------------------------------------------------------------------------------------
  // iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator_rand::iterator_rand(Set_AVL<ValueType, Compare>::Node * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator_rand::iterator_rand()
    : m_pNode(nullptr)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator_rand::~iterator_rand()
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator_rand::iterator_rand(iterator_rand const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator_rand & 
    Set_AVL<ValueType, Compare>::iterator_rand::operator=(iterator_rand const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::iterator_rand::operator==(iterator_rand const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::iterator_rand::operator!=(iterator_rand const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator_rand & 
    Set_AVL<ValueType, Compare>::iterator_rand::operator++()
  {
    m_pNode++;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator_rand
    Set_AVL<ValueType, Compare>::iterator_rand::operator++(int)
  {
    iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator_rand & 
    Set_AVL<ValueType, Compare>::iterator_rand::operator--()
  {
    m_pNode--;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator_rand
    Set_AVL<ValueType, Compare>::iterator_rand::operator--(int)
  {
    iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator_rand::operator
    typename Set_AVL<ValueType, Compare>::const_iterator_rand() const
  {
    return Set_AVL<ValueType, Compare>::const_iterator_rand(m_pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const * 
    Set_AVL<ValueType, Compare>::iterator_rand::operator->()
  {
    return &(m_pNode->data);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const & 
    Set_AVL<ValueType, Compare>::iterator_rand::operator*()
  {
    return m_pNode->data;
  }


  //------------------------------------------------------------------------------------------------
  // const_iterator
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator::const_iterator(Node const * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator::const_iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator::~const_iterator()
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::const_iterator::const_iterator(const_iterator const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator & 
    Set_AVL<ValueType, Compare>::const_iterator::operator=(const_iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::const_iterator::operator==(const_iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::const_iterator::operator!=(const_iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator
    Set_AVL<ValueType, Compare>::const_iterator::operator+(size_t a_val) const
  {
    Node const * pNode = m_pNode;
    for (size_t i = 0; i < a_val; i++)
      pNode = impl::GetNext(pNode);
    return const_iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator
    Set_AVL<ValueType, Compare>::const_iterator::operator-(size_t a_val) const
  {
    Node const * pNode = m_pNode;
    for (size_t i = 0; i < a_val; i++)
      pNode = impl::GetPrevious(pNode);
    return const_iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator &
    Set_AVL<ValueType, Compare>::const_iterator::operator+=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
      m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator &
    Set_AVL<ValueType, Compare>::const_iterator::operator-=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
      m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator & 
    Set_AVL<ValueType, Compare>::const_iterator::operator++()
  {
    m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator
    Set_AVL<ValueType, Compare>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator & 
    Set_AVL<ValueType, Compare>::const_iterator::operator--()
  {
    m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::const_iterator
    Set_AVL<ValueType, Compare>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const * 
    Set_AVL<ValueType, Compare>::const_iterator::operator->() const
  {
    return &(m_pNode->data);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const & 
    Set_AVL<ValueType, Compare>::const_iterator::operator*() const
  {
    return m_pNode->data;
  }

  //------------------------------------------------------------------------------------------------
  // iterator
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator::iterator(Node * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator::iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator::~iterator()
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator::iterator(iterator const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator & 
    Set_AVL<ValueType, Compare>::iterator::operator=(iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::iterator::operator==(iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::iterator::operator!=(iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator
    Set_AVL<ValueType, Compare>::iterator::operator+(size_t a_val) const
  {
    Node * pNode = m_pNode;
    for (size_t i = 0; i < a_val; i++)
      pNode = impl::GetNext(pNode);
    return iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator
    Set_AVL<ValueType, Compare>::iterator::operator-(size_t a_val) const
  {
    Node * pNode = m_pNode;
    for (size_t i = 0; i < a_val; i++)
      pNode = impl::GetPrevious(pNode);
    return iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator &
    Set_AVL<ValueType, Compare>::iterator::operator+=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
      m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator &
    Set_AVL<ValueType, Compare>::iterator::operator-=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
      m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator & 
    Set_AVL<ValueType, Compare>::iterator::operator++()
  {
    m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator
    Set_AVL<ValueType, Compare>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator & 
    Set_AVL<ValueType, Compare>::iterator::operator--()
  {
    m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator
    Set_AVL<ValueType, Compare>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const * 
    Set_AVL<ValueType, Compare>::iterator::operator->()
  {
    return &(m_pNode->data);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  ValueType const & 
    Set_AVL<ValueType, Compare>::iterator::operator*()
  {
    return m_pNode->data;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::iterator::operator
    typename Set_AVL<ValueType, Compare>::const_iterator() const
  {
    return Set_AVL<ValueType, Compare>::const_iterator(m_pNode);
  }

  //------------------------------------------------------------------------------------------------
  // Set_AVL
  //------------------------------------------------------------------------------------------------
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::Set_AVL()
    : m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    InitDefaultNode();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::Set_AVL(sizeType a_request)
    : m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    m_poolSize.SetSize(a_request);
    InitMemory();
    InitDefaultNode();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::~Set_AVL()
  {
    DestructAll();
    free(m_pNodes);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::Set_AVL(Set_AVL const & a_other)
    : m_poolSize(a_other.m_poolSize)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    Init(a_other);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare> & 
    Set_AVL<ValueType, Compare>::operator=(Set_AVL const & a_other)
  {
    if (this != &a_other)
    {
      if (m_poolSize.GetSize() < a_other.m_poolSize.GetSize())
      {
        Node * pMem = static_cast<Node*>(malloc(a_other.m_poolSize.GetSize() * sizeof(Node)));
        if (pMem == nullptr)
          throw std::bad_alloc();

        DestructAll();
        free(m_pNodes);
        m_pNodes = pMem;
        m_poolSize = a_other.m_poolSize;
      }
      else
        DestructAll();

      Init(a_other);
    }
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare>::Set_AVL(Set_AVL && a_other) noexcept
    : m_poolSize(a_other.m_poolSize)
    , m_pNodes(a_other.m_pNodes)
    , m_nItems(a_other.m_nItems)
    , m_pRoot(a_other.m_pRoot)
  {
    a_other.m_pNodes = nullptr;
    a_other.m_nItems = 0;
    a_other.m_pRoot = s_nullValue;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  Set_AVL<ValueType, Compare> &
    Set_AVL<ValueType, Compare>::operator=(Set_AVL && a_other) noexcept
  {
    if (this != &a_other)
    {
      m_poolSize = a_other.m_poolSize;
      m_pNodes = a_other.m_pNodes;
      m_nItems = a_other.m_nItems;
      m_pRoot = a_other.m_pRoot;

      a_other.m_pNodes = nullptr;
      a_other.m_nItems = 0;
      a_other.m_pRoot = s_nullValue;
    }
    return *this;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::sizeType
    Set_AVL<ValueType, Compare>::Set_AVL::size() const
  {
    return m_nItems;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::Set_AVL::empty() const
  {
    return m_nItems == 0;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::iterator_rand
    Set_AVL<ValueType, Compare>::Set_AVL::begin_rand()
  {
    return iterator_rand(&m_pNodes[1]);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::iterator_rand
    Set_AVL<ValueType, Compare>::Set_AVL::end_rand()
  {
    return iterator_rand(&m_pNodes[m_nItems]);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::const_iterator_rand
    Set_AVL<ValueType, Compare>::Set_AVL::cbegin_rand() const
  {
    return const_iterator_rand(&m_pNodes[1]);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::const_iterator_rand
    Set_AVL<ValueType, Compare>::Set_AVL::cend_rand() const
  {
    return const_iterator_rand(&m_pNodes[m_nItems]);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::iterator
    Set_AVL<ValueType, Compare>::Set_AVL::begin()
  {
    Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
      pNode = pNode->pLeft;
    return iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::iterator
    Set_AVL<ValueType, Compare>::Set_AVL::end()
  {
    return iterator(m_pNodes);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::const_iterator
    Set_AVL<ValueType, Compare>::Set_AVL::cbegin() const
  {
    Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
      pNode = pNode->pLeft;
    return const_iterator(pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::const_iterator
    Set_AVL<ValueType, Compare>::Set_AVL::cend() const
  {
    return const_iterator(m_pNodes);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::const_iterator
    Set_AVL<ValueType, Compare>::Set_AVL::find(ValueType const & a_value) const
  {
    Node * pNode;
    if (ValueExists(a_value, pNode))
      return const_iterator(pNode);
    return cend();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::Set_AVL::iterator
    Set_AVL<ValueType, Compare>::Set_AVL::find(ValueType const & a_value)
  {
    Node * pNode;
    if (ValueExists(a_value, pNode))
      return iterator(pNode);
    return end();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator Set_AVL<ValueType, Compare>::insert(ValueType const & a_value)
  {
    if ((m_nItems + 1) == m_poolSize.GetSize())
      Extend();

    Node * foundNode(nullptr);
    m_pRoot = __Insert(m_pRoot, nullptr, a_value, foundNode);
    return iterator(foundNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::erase(ValueType const & a_value)
  {
    EraseData eData;
    m_pRoot = __Erase<false>(m_pRoot, a_value, eData);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::iterator
    Set_AVL<ValueType, Compare>::erase(iterator a_it)
  {
    EraseData eData;
    m_pRoot = __Erase<true>(m_pRoot, *a_it, eData);
    return iterator(eData.pNext);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::Set_AVL::exists(ValueType const & a_value) const
  {
    Node * pNode;
    return ValueExists(a_value, pNode);
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::Set_AVL::clear()
  {
    DestructAll();
    m_nItems = 0;
    InitDefaultNode();
  }

#ifdef DEBUG
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  typename Set_AVL<ValueType, Compare>::sizeType
    Set_AVL<ValueType, Compare>::Set_AVL::Ind(Node const * a_pNode) const
  {
    return a_pNode - m_pNodes;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  std::string 
    Set_AVL<ValueType, Compare>::Set_AVL::ToString(Node const * a_pNode) const
  {
    std::stringstream ss;
    if (a_pNode == nullptr)
      ss << "NULL";
    else
      ss << Ind(a_pNode);
    return ss.str();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::PrintNode(Node const * a_pNode) const
  {
    std::cout << "Index: " << ToString(a_pNode)
      << ", Parent: "  << ToString(a_pNode->pParent)
      << ", Left: "  << ToString(a_pNode->pLeft)
      << ", Right: "  << ToString(a_pNode->pRight)
      << ", Key: ";
    if (a_pNode == EndNode())
      std::cout << " NONE";
    else
      std::cout << a_pNode->data.first;

    std::cout << ", Height: " << a_pNode->height << "\n";
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::Print() const
  {
    std::cout << "nItems: " << m_nItems << '\n';
    std::cout << "Root:\n";
    PrintNode(m_pRoot);
    std::cout << "\nItems:\n";
    for (sizeType i = 1; i <= m_nItems; i++)
    {
      PrintNode(&m_pNodes[i]);
    } 
    std::cout << '\n';
    std::cout << "End:\n";
    PrintNode(EndNode());
    std::cout << '\n';
  }
#endif

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::DestructAll()
  {
    for (sizeType i = 1; i <= m_nItems; i++)
      m_pNodes[i].data.~ValueType();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::InitMemory()
  {
    m_pNodes = static_cast<Node*> (realloc(m_pNodes, m_poolSize.GetSize() * sizeof(Node)));
    if (m_pNodes == nullptr)
      throw std::bad_alloc();
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::InitDefaultNode()
  {
    m_pRoot = &m_pNodes[0];
    m_pNodes[0].pParent = nullptr;
    m_pNodes[0].pLeft = nullptr;
    m_pNodes[0].pRight = nullptr;
    m_pNodes[0].height = 0;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::Init(Set_AVL const & a_other)
  {
    m_nItems = a_other.m_nItems;

    for (sizeType i = 0; i <= m_nItems; i++)
    {
      Node newNode{nullptr, nullptr, nullptr, 0};
      newNode.pParent = m_pNodes + (a_other.m_pNodes[i].pParent - a_other.m_pNodes);

      if (a_other.m_pNodes[i].pLeft)
        newNode.pLeft = m_pNodes + (a_other.m_pNodes[i].pLeft - a_other.m_pNodes);

      if (a_other.m_pNodes[i].pRight)
        newNode.pRight = m_pNodes + (a_other.m_pNodes[i].pRight - a_other.m_pNodes);

      new (&m_pNodes[i]) Node(newNode);
    }

    for (sizeType i = 1; i <= m_nItems; i++)
      new (&m_pNodes[i].data) ValueType(a_other.m_pNodes[i].data);

    m_pRoot = m_pNodes + (a_other.m_pRoot - a_other.m_pNodes);
    m_pRoot->pParent = nullptr;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  bool Set_AVL<ValueType, Compare>::ValueExists(ValueType const & a_value, Node *& a_out) const
  {
    a_out = m_pRoot;
    bool result = false;
    while (a_out != EndNode())
    {
      if (Compare(a_value, a_out->data))
      {
        if (a_out->pLeft != nullptr)
          a_out = a_out->pLeft;
        else
          break;
      }
      else if (a_out->data == a_value)
      {
        result = true;
        break;
      }
      else
      {
        if (a_out->pRight != nullptr)
          a_out = a_out->pRight;
        else
          break;
      }
    }
    return result;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  void Set_AVL<ValueType, Compare>::Extend()
  {
    m_poolSize.SetNextPoolSize();
    size_t oldSize = m_poolSize.GetSize();
    Node * oldNodes = m_pNodes;

    Node * pNodesTemp = static_cast<Node*> (realloc(m_pNodes, m_poolSize.GetSize() * sizeof(Node)));
    if (pNodesTemp == nullptr)
    {
      m_poolSize.SetSize(oldSize);
      throw std::bad_alloc();
    }

    m_pNodes = pNodesTemp;

    if (oldNodes != m_pNodes)
    {
      m_pRoot = m_pNodes + (m_pRoot - oldNodes);
      for (sizeType i = 0; i <= m_nItems; i++)
      {
        m_pNodes[i].pParent = m_pNodes + (m_pNodes[i].pParent - oldNodes);

        if (m_pNodes[i].pLeft)
          m_pNodes[i].pLeft = m_pNodes + (m_pNodes[i].pLeft - oldNodes);

        if (m_pNodes[i].pRight)
          m_pNodes[i].pRight = m_pNodes + (m_pNodes[i].pRight - oldNodes);
      }

      m_pRoot->pParent = nullptr;
    }
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  int Set_AVL<ValueType, Compare>::GetBalance(Node * a_pNode) const
  {  
    if (a_pNode == nullptr)
      return 0;  
    return Height(a_pNode->pLeft) - Height(a_pNode->pRight);  
  } 

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  int Set_AVL<ValueType, Compare>::Height(Node * a_pNode) const  
  {  
    if (a_pNode == nullptr)  
      return 0;  
    return a_pNode->height;  
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> * 
    Set_AVL<ValueType, Compare>::LeftRotate(Node * a_x)
  {  
    Node * y = a_x->pRight;  
    Node * T2 = y->pLeft;  
    Node * xParent = a_x->pParent;

    // Perform rotation  
    y->pLeft = a_x;  
    y->pParent = xParent;
    a_x->pParent = y;
    a_x->pRight = T2; 
    if (T2)
      T2->pParent = a_x;

    if (xParent)
    {
      if (xParent->pLeft == a_x)
        xParent->pLeft = y;
      else if (xParent->pRight == a_x)
        xParent->pRight = y;
    }

    // Update heights  
    a_x->height = impl::Max(Height(a_x->pLeft),  
      Height(a_x->pRight)) + 1;  
    y->height = impl::Max(Height(y->pLeft),  
      Height(y->pRight)) + 1;  

    // Return new root  
    return y;  
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> * 
    Set_AVL<ValueType, Compare>::RightRotate(Node * a_y)
  {  
    Node * x = a_y->pLeft;  
    Node * T2 = x->pRight;  
    Node * yParent = a_y->pParent;

    // Perform rotation
    x->pRight = a_y;
    x->pParent = yParent;
    a_y->pParent = x;
    a_y->pLeft = T2;
    if (T2)
      T2->pParent = a_y;

    if (yParent)
    {
      if (yParent->pLeft == a_y)
        yParent->pLeft = x;
      else if (yParent->pRight == a_y)
        yParent->pRight = x;
    }

    // Update heights  
    a_y->height = impl::Max(Height(a_y->pLeft),  
      Height(a_y->pRight)) + 1;  
    x->height = impl::Max(Height(x->pLeft),  
      Height(x->pRight)) + 1;  

    // Return new root  
    return x;  
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> * 
    Set_AVL<ValueType, Compare>::NewNode(Node * a_pParent, ValueType const & a_value)
  {
    //Insert data
    m_nItems++;
    new (&m_pNodes[m_nItems].data) ValueType(a_value);

    //Insert new node
    Node * newNode = &m_pNodes[m_nItems];
    newNode->pLeft = nullptr;
    newNode->pRight = nullptr;
    newNode->pParent = a_pParent;
    newNode->height = 1;

    return newNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> * 
    Set_AVL<ValueType, Compare>::EndNode()
  {
    return &m_pNodes[0];
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> const * 
    Set_AVL<ValueType, Compare>::EndNode() const
  {
    return &m_pNodes[0];
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  impl::Node<ValueType> * 
    Set_AVL<ValueType, Compare>::__Insert(Node * a_pNode, Node * a_pParent,
                                          ValueType const & a_value,
                                          Node *& a_newNode)
  {
    //Check if leaf node or final node in the tree. The final node will point
    //to the end node.
    bool isEndNode = (a_pNode == EndNode());
    if (a_pNode == nullptr || isEndNode )
    {
      a_newNode = NewNode(a_pParent, a_value);
      if (isEndNode)
      {
        EndNode()->pParent = a_newNode;
        a_newNode->pRight = EndNode();
      }
      return a_newNode;
    }

    //Same key
    if (a_value == a_pNode->data)
    {
      a_newNode = a_pNode;
      return a_pNode;
    }

    if (Compare(a_value, a_pNode->data))
      a_pNode->pLeft = __Insert(a_pNode->pLeft, a_pNode, a_value, a_newNode);
    else
      a_pNode->pRight = __Insert(a_pNode->pRight, a_pNode, a_value, a_newNode);

    a_pNode->height = 1 + impl::Max(Height(a_pNode->pLeft), Height(a_pNode->pRight));
    int balance = GetBalance(a_pNode);

    // Left Left Case  
    if (balance > 1 && Compare(a_value, a_pNode->data))  
      return RightRotate(a_pNode);  

    // Right Right Case  
    if (balance < -1 && !Compare(a_value, a_pNode->data))  
      return LeftRotate(a_pNode);  

    // Left Right Case  
    if (balance > 1 && !Compare(a_value, a_pNode->data))  
    {  
      a_pNode->pLeft = LeftRotate(a_pNode->pLeft);  
      return RightRotate(a_pNode);  
    }  

    // Right Left Case  
    if (balance < -1 && Compare(a_value, a_pNode->data))  
    {
      a_pNode->pRight = RightRotate(a_pNode->pRight);
      return LeftRotate(a_pNode);
    }  
    return a_pNode;
  }

  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &)>
  template<bool GetNext>
  impl::Node<ValueType> *
    Set_AVL<ValueType, Compare>::__Erase(Node * a_pRoot, ValueType const & a_value, EraseData & a_data)
  {
    if (a_pRoot == nullptr || a_pRoot == EndNode())
      return a_pRoot;

    if (Compare(a_value, a_pRoot->data))
    {
      Node * temp = __Erase<GetNext>(a_pRoot->pLeft, a_value, a_data);
      if (a_data.oldNodeAdd == a_pRoot)
        a_pRoot = a_data.newNodeAdd;

      a_pRoot->pLeft = temp;
    }
    else if (a_value == a_pRoot->data)
    {
      if constexpr (GetNext)
      {
        if (a_data.pNext == nullptr)
          a_data.pNext = impl::GetNext(a_pRoot);
      }

      bool noLeftChild = a_pRoot->pLeft == nullptr;
      bool rightIsNull = a_pRoot->pRight == nullptr;
      bool rightIsEnd = a_pRoot->pRight == EndNode();
      bool noRightChild = rightIsNull || rightIsEnd;

      // node with only one child or no child
      Node * returnNode = nullptr;
      if (noLeftChild || noRightChild)  
      {
        //Case 1:
        // left = null, right == null,  return = null
        //Case 2:
        // left = node, right == null,  return = left
        //Case 3:
        // left = null, right == node,  return = right
        //Case 4:
        // left = null, right == End,  return = right
        //Case 5:
        // left = node, right == End,  return = left

        //Break the node out of the tree
        //Case 1, 3, 4:
        if (noLeftChild)
        {
          if (a_pRoot->pRight)
            a_pRoot->pRight->pParent = a_pRoot->pParent;
          if (a_pRoot->pParent)
          {
            if (a_pRoot->pParent->pLeft == a_pRoot)
              a_pRoot->pParent->pLeft = a_pRoot->pRight;
            else
              a_pRoot->pParent->pRight = a_pRoot->pRight;
          }
          returnNode = a_pRoot->pRight;
        }
        //Case 2, 5:
        else
        {
          //Move end node
          if (rightIsEnd)
          {
            Node * temp = a_pRoot->pLeft;
            while (temp->pRight)
              temp = temp->pRight;
            temp->pRight = a_pRoot->pRight;
            a_pRoot->pRight->pParent = temp;
            a_pRoot->pRight = nullptr;
          }

          //Break node from tree
          a_pRoot->pLeft->pParent = a_pRoot->pParent;
          if (a_pRoot->pParent)
          {
            if (a_pRoot->pParent->pLeft == a_pRoot)
              a_pRoot->pParent->pLeft = a_pRoot->pLeft;
            else
              a_pRoot->pParent->pRight = a_pRoot->pLeft;
          }
          returnNode = a_pRoot->pLeft;
        }

        if (!a_data.firstSuccDeleted)
          a_pRoot->data.~ValueType();
        //TODO mem out with 0xdeadbeef

        //Was this the last node? If not we need to move the last node to fill the gap.
        if (a_pRoot != &m_pNodes[m_nItems])
        {
          //Shift last node to fill in empty slot
          memcpy(&(a_pRoot->data), &(m_pNodes[m_nItems].data), sizeof(ValueType));

          Node * oldNode = &m_pNodes[m_nItems];

          //We have a couple of pointers to nodes in use that may need to be updated... 
          a_data.oldNodeAdd = oldNode;
          a_data.newNodeAdd = a_pRoot;

          if (returnNode == oldNode)
            returnNode = a_pRoot;
          if constexpr (GetNext)
          {
            if (a_data.pNext == oldNode)
              a_data.pNext = a_pRoot;
          }

          a_pRoot->pLeft = oldNode->pLeft;
          a_pRoot->pRight = oldNode->pRight;
          a_pRoot->pParent = oldNode->pParent;
          a_pRoot->height = oldNode->height;

          if (a_pRoot->pParent)
          {
            if (a_pRoot->pParent->pLeft == oldNode)
              a_pRoot->pParent->pLeft = a_pRoot;
            else
              a_pRoot->pParent->pRight = a_pRoot;
          }
          if (a_pRoot->pLeft)
            a_pRoot->pLeft->pParent = a_pRoot;
          if (a_pRoot->pRight)
            a_pRoot->pRight->pParent = a_pRoot;
        }

        m_nItems--;
        a_pRoot = returnNode;
      }  
      else
      {  
        // node with two children: Get the inorder  
        // successor (smallest in the right subtree)
        Node * successor = a_pRoot->pRight;
        while (successor->pLeft)
          successor = successor->pLeft;

        if constexpr (GetNext)
        {
          if (a_data.pNext == successor)
            a_data.pNext = a_pRoot;
        }

        // Copy the inorder successor's  
        // data to this node

        if (!a_data.firstSuccDeleted)
        {
          successor->data.~ValueType();
          a_data.firstSuccDeleted = true;
        }

        memcpy(&(a_pRoot->data), &(successor->data), sizeof(ValueType));

        // Delete the inorder successor
        Node * temp = __Erase<GetNext>(a_pRoot->pRight, successor->data, a_data);
        if (a_data.oldNodeAdd == a_pRoot)
          a_pRoot = a_data.newNodeAdd;

        a_pRoot->pRight = temp;
      }
    }
    else
    {
      Node * temp = __Erase<GetNext>(a_pRoot->pRight, a_value, a_data);
      if (a_data.oldNodeAdd == a_pRoot)
        a_pRoot = a_data.newNodeAdd;

      a_pRoot->pRight = temp;
    }

    //The tree only had one node
    if (a_pRoot == nullptr || a_pRoot == EndNode())
      return a_pRoot;

    a_pRoot->height = 1 + impl::Max(Height(a_pRoot->pLeft), Height(a_pRoot->pRight));
    int balance = GetBalance(a_pRoot);

    // Left Left Case  
    if (balance > 1 && GetBalance(a_pRoot->pLeft) >= 0)
      return RightRotate(a_pRoot);

    // Left Right Case  
    if (balance > 1 && GetBalance(a_pRoot->pLeft) < 0)
    {
      a_pRoot->pLeft = LeftRotate(a_pRoot->pLeft);  
      return RightRotate(a_pRoot);
    }

    // Right Right Case  
    if (balance < -1 && GetBalance(a_pRoot->pRight) <= 0)
      return LeftRotate(a_pRoot);

    // Right Left Case  
    if (balance < -1 && GetBalance(a_pRoot->pRight) > 0)
    {
      a_pRoot->pRight = RightRotate(a_pRoot->pRight);  
      return LeftRotate(a_pRoot);
    }

    return a_pRoot;
  }
}

#endif