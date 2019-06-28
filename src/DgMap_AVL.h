#ifndef DGMAP_AVL_H
#define DGMAP_AVL_H

#include <exception>
#include <new>
#include <cstring>
#include <exception>

#include "DgPair.h"
#include "impl/DgContainerBase.h"

#ifdef DEBUG
#include <sstream>
#include <iostream>
#endif

namespace Dg
{
  namespace impl
  {
    template<typename T>
    bool Less(T const & t0, T const & t1)
    {
      return t0 < t1;
    }

    template<typename T>
    T Max(T a, T b)
    {
      return a > b ? a : b;
    }

    struct Node
    {
      Node *       pParent;
      Node *       pLeft;
      Node *       pRight;
      int          height;
    };

    Node * GetNext(Node const *);
    Node * GetPrevious(Node const *);
  }

  //AVL tree implemented with an object pool. 
  //Objectives:
  // 1) Fast searching
  // 2) No memory allocation on insert. Only caveat is on extending the object pool
  // 3) No memory deallocation on erasing elements
  // 4) Fast iteration over elements if order is not important (iterator_rand)
  //An end node will follow the last element in the tree.
  template<typename K, typename V, bool (*Compare)(K const &, K const &) = impl::Less<K>>
  class Map_AVL : public ContainerBase
  {
  public:

    typedef Pair<K const, V> ValueType;

  private:

    typedef size_t sizeType;

    struct EraseData
    {
      impl::Node * oldNodeAdd;
      impl::Node * newNodeAdd;
      impl::Node * pNext;
    };

  public:

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class const_iterator_rand
    {
      friend class Map_AVL;
      friend class iterator_rand;

    private:

      const_iterator_rand(ValueType const *  a_pKV);

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
      ValueType const *  m_pKV;
    };

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class iterator_rand
    {
      friend class Map_AVL;

    private:

      iterator_rand(ValueType *  a_pKV);

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

      ValueType * operator->();
      ValueType & operator*();

    private:
      ValueType *  m_pKV;
    };

    //Iterates through the map as sorted by the criterion
    class const_iterator
    {
      friend class Map_AVL;
      friend class iterator;
    private:

      const_iterator(impl::Node const * pNode,
                     impl::Node const * pNodeBegin,
                     ValueType const * pKVBegin);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const & a_it);
      const_iterator& operator=(const_iterator const & a_other);

      bool operator==(const_iterator const & a_it) const;
      bool operator!=(const_iterator const & a_it) const;

      ValueType const * operator->() const;
      ValueType const & operator*() const;

      const_iterator & operator++();
      const_iterator operator++(int);
      const_iterator & operator--();
      const_iterator operator--(int);

    private:
      impl::Node const *      m_pNode;
      impl::Node const *      m_pOffset;
      ValueType const * m_pKVBegin;
    };

    //Iterates through the map as sorted by the criterion
    class iterator
    {
      friend class Map_AVL;
    private:

      iterator(impl::Node * pNode,
               impl::Node * pNodeBegin,
               ValueType * pKVBegin);

    public:

      iterator();
      ~iterator();

      iterator(iterator const & a_it);
      iterator& operator=(iterator const & a_other);

      bool operator==(iterator const & a_it) const;
      bool operator!=(iterator const & a_it) const;

      ValueType * operator->();
      ValueType & operator*();

      iterator & operator++();
      iterator operator++(int);
      iterator & operator--();
      iterator operator--(int);

      operator const_iterator() const;

    private:
      impl::Node *      m_pNode;
      impl::Node *      m_pOffset;
      ValueType * m_pKVBegin;
    };

  public:

    Map_AVL();
    Map_AVL(sizeType requestSize);
    ~Map_AVL();

    Map_AVL(Map_AVL const &);
    Map_AVL & operator=(Map_AVL const & a_other);

    Map_AVL(Map_AVL && a_other);
    Map_AVL & operator=(Map_AVL && a_other);

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
    iterator insert(K const & a_key, V const & a_data);

    void erase(K const &);

    //Returns an iterator to the element that follows the element removed
    //(or end(), if the last element was removed).
    iterator erase(iterator);

    //Searches the container for an element with a key equivalent to a_key and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    const_iterator find(K const &) const;

    //Searches the container for an element with a key equivalent to a_key and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    iterator find(K const &);

    //If k matches the key of an element in the container, the function returns 
    //a reference to its mapped value.
    //If k does not match the key of any element in the container, the function 
    //inserts a new element with that key and returns a reference to its mapped value.
    V & operator[](K const &);

    //Returns a reference to the mapped value of the element identified with key k.
    //If k does not match the key of any element in the container, the function 
    //throws an out_of_range exception.
    V & at(K const &);

    //Returns a reference to the mapped value of the element identified with key k.
    //If k does not match the key of any element in the container, the function 
    //throws an out_of_range exception.
    V const & at(K const &) const;

    void clear();
#ifdef DEBUG
  public:
    void Print() const;
  private:
    sizeType Ind(impl::Node const *) const;
    std::string ToString(impl::Node const *) const;
    void PrintNode(impl::Node const * a_pNode) const;
#endif
  private:

    void DestructAll();
    void InitMemory();
    void InitDefaultNode();
    void Init(Map_AVL const &);

    //Sets a_out to the node index which references the key, or
    //if the key does not exist, the node at which the key should be
    //added
    bool KeyExists(K const & a_key, impl::Node *& a_out) const;

    sizeType RawIndex(K const &) const;
    void Extend();
    int GetBalance(impl::Node *) const;

    // A utility function to get height  
    // of the tree  
    int Height(impl::Node *) const;
    impl::Node * LeftRotate(impl::Node *);
    impl::Node * RightRotate(impl::Node * a_y);

    //Constructs a new node after a_pParent. New node will point back to a_pParent.
    impl::Node * NewNode(impl::Node * pParent, K const & key, V const & data);
    inline impl::Node * EndNode();
    inline impl::Node const * EndNode() const;

    inline ValueType * GetAssociatedKV(impl::Node *);
    inline ValueType const * GetAssociatedKV(impl::Node const *) const;

    //Returns the pointer to the new node.
    //Assumes tree has at least 1 element
    impl::Node * __Insert(impl::Node * pNode, impl::Node * pParent,
                    K const & key, V const & data,
                    impl::Node *& newNode);

    template<bool GetNext>
    impl::Node * __Erase(impl::Node * root, K const &, EraseData &);

  private:

    impl::Node *        m_pRoot;
    impl::Node *        m_pNodes;
    ValueType *         m_pKVs;
    sizeType            m_nItems;
  };

  //------------------------------------------------------------------------------------------------
  // const_iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator_rand::const_iterator_rand(ValueType const * a_pKV)
    : m_pKV(a_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator_rand::const_iterator_rand()
    : m_pKV(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator_rand::~const_iterator_rand()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator_rand::const_iterator_rand(const_iterator_rand const & a_it)
    : m_pKV(a_it.m_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator_rand & 
    Map_AVL<K, V, Compare>::const_iterator_rand::operator=(const_iterator_rand const & a_it)
  {
    m_pKV = a_it.m_pKV;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::const_iterator_rand::operator==(const_iterator_rand const & a_it) const
  {
    return m_pKV == a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::const_iterator_rand::operator!=(const_iterator_rand const & a_it) const
  {
    return m_pKV != a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator_rand & 
    Map_AVL<K, V, Compare>::const_iterator_rand::operator++()
  {
    m_pKV++;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator_rand
    Map_AVL<K, V, Compare>::const_iterator_rand::operator++(int)
  {
    const_iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator_rand & 
    Map_AVL<K, V, Compare>::const_iterator_rand::operator--()
  {
    m_pKV--;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator_rand
    Map_AVL<K, V, Compare>::const_iterator_rand::operator--(int)
  {
    const_iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType const * 
    Map_AVL<K, V, Compare>::const_iterator_rand::operator->() const
  {
    return m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType const & 
    Map_AVL<K, V, Compare>::const_iterator_rand::operator*() const
  {
    return *m_pKV;
  }

  //------------------------------------------------------------------------------------------------
  // iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator_rand::iterator_rand(ValueType * a_pKV)
    : m_pKV(a_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator_rand::iterator_rand()
    : m_pKV(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator_rand::~iterator_rand()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator_rand::iterator_rand(iterator_rand const & a_it)
    : m_pKV(a_it.m_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator_rand & 
    Map_AVL<K, V, Compare>::iterator_rand::operator=(iterator_rand const & a_it)
  {
    m_pKV = a_it.m_pKV;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::iterator_rand::operator==(iterator_rand const & a_it) const
  {
    return m_pKV == a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::iterator_rand::operator!=(iterator_rand const & a_it) const
  {
    return m_pKV != a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator_rand & 
    Map_AVL<K, V, Compare>::iterator_rand::operator++()
  {
    m_pKV++;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator_rand
    Map_AVL<K, V, Compare>::iterator_rand::operator++(int)
  {
    iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator_rand & 
    Map_AVL<K, V, Compare>::iterator_rand::operator--()
  {
    m_pKV--;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator_rand
    Map_AVL<K, V, Compare>::iterator_rand::operator--(int)
  {
    iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator_rand::operator
    typename Map_AVL<K, V, Compare>::const_iterator_rand() const
  {
    return Map_AVL<K, V, Compare>::const_iterator_rand(m_pKV);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType * 
    Map_AVL<K, V, Compare>::iterator_rand::operator->()
  {
    return m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType & 
    Map_AVL<K, V, Compare>::iterator_rand::operator*()
  {
    return *m_pKV;
  }


  //------------------------------------------------------------------------------------------------
  // const_iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator::const_iterator(impl::Node const * a_pNode,
                                                            impl::Node const * a_pNodeBegin,
                                                            ValueType const * a_pKVBegin)
    : m_pNode(a_pNode)
    , m_pOffset(a_pNodeBegin)
    , m_pKVBegin(a_pKVBegin)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator::const_iterator()
    : m_pNode(nullptr)
    , m_pOffset(nullptr)
    , m_pKVBegin(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator::~const_iterator()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::const_iterator::const_iterator(const_iterator const & a_it)
    : m_pNode(a_it.m_pNode)
    , m_pOffset(a_it.m_pOffset)
    , m_pKVBegin(a_it.m_pKVBegin)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator & 
    Map_AVL<K, V, Compare>::const_iterator::operator=(const_iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    m_pOffset = a_it.m_pOffset;
    m_pKVBegin = a_it.m_pKVBegin;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::const_iterator::operator==(const_iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::const_iterator::operator!=(const_iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator & 
    Map_AVL<K, V, Compare>::const_iterator::operator++()
  {
    m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator
    Map_AVL<K, V, Compare>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator & 
    Map_AVL<K, V, Compare>::const_iterator::operator--()
  {
    m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::const_iterator
    Map_AVL<K, V, Compare>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType const * 
    Map_AVL<K, V, Compare>::const_iterator::operator->() const
  {
    return m_pKVBegin + (m_pNode - m_pOffset);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType const & 
    Map_AVL<K, V, Compare>::const_iterator::operator*() const
  {
    return *(m_pKVBegin + (m_pNode - m_pOffset));
  }

  //------------------------------------------------------------------------------------------------
  // iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator::iterator(impl::Node * a_pNode,
                                                impl::Node * a_pNodeBegin,
                                                ValueType * a_pKVBegin)
    : m_pNode(a_pNode)
    , m_pOffset(a_pNodeBegin)
    , m_pKVBegin(a_pKVBegin)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator::iterator()
    : m_pNode(nullptr)
    , m_pOffset(nullptr)
    , m_pKVBegin(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator::~iterator()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator::iterator(iterator const & a_it)
    : m_pNode(a_it.m_pNode)
    , m_pOffset(a_it.m_pOffset)
    , m_pKVBegin(a_it.m_pKVBegin)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator & 
    Map_AVL<K, V, Compare>::iterator::operator=(iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    m_pOffset = a_it.m_pOffset;
    m_pKVBegin = a_it.m_pKVBegin;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::iterator::operator==(iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::iterator::operator!=(iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator & 
    Map_AVL<K, V, Compare>::iterator::operator++()
  {
    m_pNode = impl::GetNext(m_pNode);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator
    Map_AVL<K, V, Compare>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator & 
    Map_AVL<K, V, Compare>::iterator::operator--()
  {
    m_pNode = impl::GetPrevious(m_pNode);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator
    Map_AVL<K, V, Compare>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType * 
    Map_AVL<K, V, Compare>::iterator::operator->()
  {
    return m_pKVBegin + (m_pNode - m_pOffset);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType & 
    Map_AVL<K, V, Compare>::iterator::operator*()
  {
    return *(m_pKVBegin + (m_pNode - m_pOffset));
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::iterator::operator
    typename Map_AVL<K, V, Compare>::const_iterator() const
  {
    return Map_AVL<K, V, Compare>::const_iterator(m_pNode);
  }

  //------------------------------------------------------------------------------------------------
  // Map_AVL
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::Map_AVL()
    : ContainerBase()
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    InitDefaultNode();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::Map_AVL(sizeType a_request)
    : ContainerBase(a_request)
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    InitDefaultNode();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::~Map_AVL()
  {
    DestructAll();
    free(m_pKVs);
    free(m_pNodes);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::Map_AVL(Map_AVL const & a_other)
    : ContainerBase(a_other.pool_size())
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    Init(a_other);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare> & 
    Map_AVL<K, V, Compare>::operator=(Map_AVL const & a_other)
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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare>::Map_AVL(Map_AVL && a_other)
    : ContainerBase(a_other)
    , m_pKVs(a_other.m_pKVs)
    , m_pNodes(a_other.m_pNodes)
    , m_nItems(a_other.m_nItems)
    , m_pRoot(a_other.m_pRoot)
  {
    a_other.m_pKVs = nullptr;
    a_other.m_pNodes = nullptr;
    a_other.m_nItems = 0;
    a_other.m_pRoot = s_nullValue;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  Map_AVL<K, V, Compare> &
    Map_AVL<K, V, Compare>::operator=(Map_AVL && a_other)
  {
    if (this != &a_other)
    {
      ContainerBase::operator=(a_other);
      m_pKVs = a_other.m_pKVs;
      m_pNodes = a_other.m_pNodes;
      m_nItems = a_other.m_nItems;
      m_pRoot = a_other.m_pRoot;

      a_other.m_pKVs = nullptr;
      a_other.m_pNodes = nullptr;
      a_other.m_nItems = 0;
      a_other.m_pRoot = s_nullValue;
    }
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::sizeType
    Map_AVL<K, V, Compare>::Map_AVL::size() const
  {
    return m_nItems;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::Map_AVL::empty() const
  {
    return m_nItems == 0;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::iterator_rand
    Map_AVL<K, V, Compare>::Map_AVL::begin_rand()
  {
    return iterator_rand(m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::iterator_rand
    Map_AVL<K, V, Compare>::Map_AVL::end_rand()
  {
    return iterator_rand(m_pKVs + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::const_iterator_rand
    Map_AVL<K, V, Compare>::Map_AVL::cbegin_rand() const
  {
    return const_iterator_rand(m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::const_iterator_rand
    Map_AVL<K, V, Compare>::Map_AVL::cend_rand() const
  {
    return const_iterator_rand(m_pKVs + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::iterator
    Map_AVL<K, V, Compare>::Map_AVL::begin()
  {
    impl::Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
      pNode = pNode->pLeft;
    return iterator(pNode, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::iterator
    Map_AVL<K, V, Compare>::Map_AVL::end()
  {
    return iterator(m_pNodes, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::const_iterator
    Map_AVL<K, V, Compare>::Map_AVL::cbegin() const
  {
    impl::Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
      pNode = pNode->pLeft;
    return const_iterator(pNode, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::const_iterator
    Map_AVL<K, V, Compare>::Map_AVL::cend() const
  {
    return const_iterator(m_pNodes, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::const_iterator
    Map_AVL<K, V, Compare>::Map_AVL::find(K const & a_key) const
  {
    impl::Node * pNode;
    if (KeyExists(a_key, pNode))
      return const_iterator(pNode, m_pNodes + 1, m_pKVs);
    return cend();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::Map_AVL::iterator
    Map_AVL<K, V, Compare>::Map_AVL::find(K const & a_key)
  {
    impl::Node * pNode;
    if (KeyExists(a_key, pNode))
      return iterator(pNode, m_pNodes + 1, m_pKVs);
    return end();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V & Map_AVL<K, V, Compare>::Map_AVL::operator[](K const & a_key)
  {
    iterator it = insert(a_key, V());
    return it->second;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator Map_AVL<K, V, Compare>::insert(K const & a_key, V const & a_data)
  {
    if ((m_nItems + 1) == pool_size())
      Extend();

    impl::Node * foundNode(nullptr);
    m_pRoot = __Insert(m_pRoot, nullptr, a_key, a_data, foundNode);
    return iterator(foundNode, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::erase(K const & a_key)
  {
    EraseData eData{nullptr, nullptr, nullptr};
    m_pRoot = __Erase<false>(m_pRoot, a_key, eData);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::iterator
    Map_AVL<K, V, Compare>::erase(iterator a_it)
  {
    EraseData eData{nullptr, nullptr, nullptr};
    m_pRoot = __Erase<true>(m_pRoot, a_it->first, eData);
    return iterator(eData.pNext, m_pNodes + 1, m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V & Map_AVL<K, V, Compare>::Map_AVL::at(K const & a_key)
  {
    return m_pKVs[RawIndex(a_key)].second;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V const & Map_AVL<K, V, Compare>::Map_AVL::at(K const & a_key) const
  {
    return m_pKVs[RawIndex(a_key)].second;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::Map_AVL::clear()
  {
    DestructAll();
    m_nItems = 0;
    InitDefaultNode();
  }

#ifdef DEBUG
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::sizeType
    Map_AVL<K, V, Compare>::Map_AVL::Ind(impl::Node const * a_pNode) const
  {
    return a_pNode - m_pNodes;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  std::string 
    Map_AVL<K, V, Compare>::Map_AVL::ToString(impl::Node const * a_pNode) const
  {
    std::stringstream ss;
    if (a_pNode == nullptr)
      ss << "NULL";
    else
      ss << Ind(a_pNode);
    return ss.str();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::PrintNode(impl::Node const * a_pNode) const
  {
    std::cout << "Index: " << ToString(a_pNode)
      << ", Parent: "  << ToString(a_pNode->pParent)
      << ", Left: "  << ToString(a_pNode->pLeft)
      << ", Right: "  << ToString(a_pNode->pRight)
      << ", Key: ";
    if (a_pNode == EndNode())
      std::cout << " NONE";
    else
      std::cout << GetAssociatedKV(a_pNode)->first;

    std::cout << ", Height: " << a_pNode->height << "\n";
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::Print() const
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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::DestructAll()
  {
    for (sizeType i = 0; i < m_nItems; i++)
      m_pKVs[i].~ValueType();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::InitMemory()
  {
    m_pNodes = static_cast<impl::Node*> (realloc(m_pNodes, pool_size() * sizeof(impl::Node)));
    if (m_pNodes == nullptr)
      throw std::bad_alloc();

    m_pKVs = static_cast<ValueType*> (realloc(m_pKVs, pool_size() * sizeof(ValueType)));
    if (m_pKVs == nullptr)
      throw std::bad_alloc();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::InitDefaultNode()
  {
    m_pRoot = &m_pNodes[0];
    impl::Node endNode;
    endNode.pParent = nullptr;
    endNode.pLeft = nullptr;
    endNode.pRight = nullptr;
    endNode.height = 0;
    new (&m_pNodes[0]) impl::Node(endNode);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::Init(Map_AVL const & a_other)
  {
    m_nItems = a_other.m_nItems;

    for (sizeType i = 0; i <= m_nItems; i++)
    {
      impl::Node newNode{nullptr, nullptr, nullptr, 0};
      newNode.pParent = m_pNodes + (a_other.m_pNodes[i].pParent - a_other.m_pNodes);
      
      if (a_other.m_pNodes[i].pLeft)
        newNode.pLeft = m_pNodes + (a_other.m_pNodes[i].pLeft - a_other.m_pNodes);
        
      if (a_other.m_pNodes[i].pRight)
        newNode.pRight = m_pNodes + (a_other.m_pNodes[i].pRight - a_other.m_pNodes);

      new (&m_pNodes[i]) impl::Node(newNode);
    }

    for (sizeType i = 0; i < m_nItems; i++)
      new (&m_pKVs[i]) ValueType(a_other.m_pKVs[i]);

    m_pRoot = m_pNodes + (a_other.m_pRoot - a_other.m_pNodes);
    m_pRoot->pParent = nullptr;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool Map_AVL<K, V, Compare>::KeyExists(K const & a_key, impl::Node *& a_out) const
  {
    a_out = m_pRoot;
    bool result = false;
    while (a_out != EndNode())
    {
      ValueType const * pKV = GetAssociatedKV(a_out);
      if (Compare(a_key, pKV->first))
      {
        if (a_out->pLeft != nullptr)
          a_out = a_out->pLeft;
        else
          break;
      }
      else if (pKV->first == a_key)
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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::sizeType 
    Map_AVL<K, V, Compare>::RawIndex(K const & a_key) const
  {
    impl::Node * pResult;
    if (!KeyExists(a_key, pResult))
      throw std::out_of_range("Invalid key!");
    return pResult - m_pNodes - 1;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void Map_AVL<K, V, Compare>::Extend()
  {
    set_next_pool_size();

    impl::Node * oldNodes = m_pNodes;

    m_pNodes = static_cast<impl::Node*> (realloc(m_pNodes, pool_size() * sizeof(impl::Node)));
    if (m_pNodes == nullptr)
      throw std::bad_alloc();

    m_pKVs = static_cast<ValueType*> (realloc(m_pKVs, pool_size() * sizeof(ValueType)));
    if (m_pKVs == nullptr)
      throw std::bad_alloc();


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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  int Map_AVL<K, V, Compare>::GetBalance(impl::Node * a_pNode) const
  {  
    if (a_pNode == nullptr)
      return 0;  
    return Height(a_pNode->pLeft) - Height(a_pNode->pRight);  
  } 

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  int Map_AVL<K, V, Compare>::Height(impl::Node * a_pNode) const  
  {  
    if (a_pNode == nullptr)  
      return 0;  
    return a_pNode->height;  
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node * 
    Map_AVL<K, V, Compare>::LeftRotate(impl::Node * a_x)
  {  
    impl::Node * y = a_x->pRight;  
    impl::Node * T2 = y->pLeft;  
    impl::Node * xParent = a_x->pParent;

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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node * 
    Map_AVL<K, V, Compare>::RightRotate(impl::Node * a_y)
  {  
    impl::Node * x = a_y->pLeft;  
    impl::Node * T2 = x->pRight;  
    impl::Node * yParent = a_y->pParent;

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

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node * 
    Map_AVL<K, V, Compare>::NewNode(impl::Node * a_pParent, K const & a_key, V const & a_data)
  {
    //Insert data
    new (&m_pKVs[m_nItems]) ValueType{a_key, a_data};

    m_nItems++;

    //Insert new node
    impl::Node * newNode = &m_pNodes[m_nItems];
    newNode->pLeft = nullptr;
    newNode->pRight = nullptr;
    newNode->pParent = a_pParent;
    newNode->height = 1;

    return newNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node * 
    Map_AVL<K, V, Compare>::EndNode()
  {
    return &m_pNodes[0];
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node const * 
    Map_AVL<K, V, Compare>::EndNode() const
  {
    return &m_pNodes[0];
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType * 
    Map_AVL<K, V, Compare>::GetAssociatedKV(impl::Node * a_pNode)
  {
    sizeType ind = a_pNode - m_pNodes;
    return &m_pKVs[ind - 1];
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename Map_AVL<K, V, Compare>::ValueType const * 
    Map_AVL<K, V, Compare>::GetAssociatedKV(impl::Node const * a_pNode) const
  {
    sizeType ind = a_pNode - m_pNodes;
    return &m_pKVs[ind - 1];
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  impl::Node * 
    Map_AVL<K, V, Compare>::__Insert(impl::Node * a_pNode, impl::Node * a_pParent,
      K const & a_key, V const & a_data,
      impl::Node *& a_newNode)
  {
    //Check if leaf node or final node in the tree. The final node will point
    //to the end node.
    bool isEndNode = (a_pNode == EndNode());
    if (a_pNode == nullptr || isEndNode )
    {
      a_newNode = NewNode(a_pParent, a_key, a_data);
      if (isEndNode)
      {
        EndNode()->pParent = a_newNode;
        a_newNode->pRight = EndNode();
      }
      return a_newNode;
    }

    //Same key
    if (a_key == GetAssociatedKV(a_pNode)->first)
    {
      a_newNode = a_pNode;
      return a_pNode;
    }

    if (Compare(a_key, GetAssociatedKV(a_pNode)->first))
      a_pNode->pLeft = __Insert(a_pNode->pLeft, a_pNode, a_key, a_data, a_newNode);
    else
      a_pNode->pRight = __Insert(a_pNode->pRight, a_pNode, a_key, a_data, a_newNode);

    a_pNode->height = 1 + impl::Max(Height(a_pNode->pLeft), Height(a_pNode->pRight));
    int balance = GetBalance(a_pNode);

    // Left Left Case  
    if (balance > 1 && Compare(a_key, GetAssociatedKV(a_pNode->pLeft)->first))  
      return RightRotate(a_pNode);  

    // Right Right Case  
    if (balance < -1 && !Compare(a_key, GetAssociatedKV(a_pNode->pRight)->first))  
      return LeftRotate(a_pNode);  

    // Left Right Case  
    if (balance > 1 && !Compare(a_key, GetAssociatedKV(a_pNode->pLeft)->first))  
    {  
      a_pNode->pLeft = LeftRotate(a_pNode->pLeft);  
      return RightRotate(a_pNode);  
    }  

    // Right Left Case  
    if (balance < -1 && Compare(a_key, GetAssociatedKV(a_pNode->pRight)->first))  
    {
      a_pNode->pRight = RightRotate(a_pNode->pRight);
      return LeftRotate(a_pNode);
    }  
    return a_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  template<bool GetNext>
  impl::Node *
    Map_AVL<K, V, Compare>::__Erase(impl::Node * a_pRoot, K const & a_key, EraseData & a_data)
  {
    if (a_pRoot == nullptr || a_pRoot == EndNode())
      return a_pRoot;

    if (Compare(a_key, GetAssociatedKV(a_pRoot)->first))
    {
      impl::Node * temp = __Erase<GetNext>(a_pRoot->pLeft, a_key, a_data);
      if (a_data.oldNodeAdd == a_pRoot)
        a_pRoot = a_data.newNodeAdd;

      a_pRoot->pLeft = temp;
    }
    else if (a_key == GetAssociatedKV(a_pRoot)->first)
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
      impl::Node * returnNode = nullptr;
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
            impl::Node * temp = a_pRoot->pLeft;
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

        ValueType * pKV = GetAssociatedKV(a_pRoot);
        pKV->~ValueType();

        //Was this the last node? If not we need to move the last node to fill the gap.
        if (pKV != &m_pKVs[m_nItems - 1])
        {
          memcpy(pKV, &m_pKVs[m_nItems - 1], sizeof(ValueType));

          //Shift last node to fill in empty slot
          impl::Node * oldNode = &m_pNodes[m_nItems];

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

          *a_pRoot = *oldNode;
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
        impl::Node * successor = a_pRoot->pRight;
        while (successor->pLeft)
          successor = successor->pLeft;

        if constexpr (GetNext)
        {
          if (a_data.pNext == successor)
            a_data.pNext = a_pRoot;
        }

        // Copy the inorder successor's  
        // data to this node  
        ValueType * pKVRoot = GetAssociatedKV(a_pRoot);
        ValueType * pKVSucc = GetAssociatedKV(successor);
        pKVRoot->~ValueType();
        new (pKVRoot) ValueType(*pKVSucc);

        // Delete the inorder successor
        impl::Node * temp = __Erase<GetNext>(a_pRoot->pRight, pKVSucc->first, a_data);
        if (a_data.oldNodeAdd == a_pRoot)
          a_pRoot = a_data.newNodeAdd;

        a_pRoot->pRight = temp;
      }
    }
    else
    {
      impl::Node * temp = __Erase<GetNext>(a_pRoot->pRight, a_key, a_data);
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