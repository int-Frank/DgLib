//! @file Dg_HashTable.h
//!
//! @author Frank Hart
//! @date 22/08/2016
//!
//! Class declaration: OpenHashTable

#ifndef DGOPENHASHMAP_H
#define DGOPENHASHMAP_H

#include <type_traits>
#include <exception>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "DgPair.h"
#include "impl/DgPoolSizeManager.h"
#include "DgBit.h"

#undef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

namespace Dg
{
  namespace impl
  {
    namespace OpenHashMap
    {
      class NodeIndex
      {
      public:

        enum Type : uint64_t
        {
          Data = 0,
          Bucket = 1
        };

        static uint64_t const NULLNODE = Mask<uint64_t, 0, 63>::value;

        NodeIndex::NodeIndex() : m_data(0) {SetNull();}
        NodeIndex::NodeIndex(Type a_type, uint64_t a_index)
          : m_data(0)
        {
          SetType(a_type);
          SetIndex(a_index);
        }

        inline operator uint64_t()              {return GetIndex();}
        inline void SetType(Type a_type)        {m_data = SetSubInt<uint64_t, 63, 1>(m_data, a_type);}
        inline Type GetType() const             {return static_cast<Type>(GetSubInt<uint64_t, 63, 1>(m_data));}
        inline void SetIndex(uint64_t a_index)  {m_data = SetSubInt<uint64_t, 0, 63>(m_data, a_index);}
        inline uint64_t GetIndex() const        {return GetSubInt<uint64_t, 0, 63>(m_data);}
        inline void SetNull()                   {m_data = SetSubInt<uint64_t, 0, 63>(m_data, NULLNODE);}
        inline bool IsNull() const              {return GetIndex() == NULLNODE;}
        inline void Set(Type a_type, uint64_t a_index)
        {
          SetType(a_type);
          SetIndex(a_index);
        }

      private:
        uint64_t m_data;
      };

      //! The default hasher simply returns the key cast to a size_t. 
      //! In a the table, the result will be modded against the (prime)
      //! number of buckets. 
      template<typename K>
      class SimpleHasher
      {
      public:
        size_t operator()(K const& a_k) const
        {
          return static_cast<size_t>(a_k);
        }
      };

      template<typename K>
      class EqualTo
      {
      public:
        bool operator()(K const& a_k0, K const& a_k1) const
        {
          return a_k0 == a_k1;
        }
      };

      double const loadFactorBounds[2] = {0.01, 1.0};
      double const defaultLoadFactor   = 0.75;
      size_t const defaultBucketCount  = 19;
      uint64_t const maxBucketCount = 0x7FFF'FFFF'FFFF'FFFE;
    }
  }

  template<typename K, 
           typename V, 
           class HASHER = impl::OpenHashMap::SimpleHasher<K>, 
           class EQUALTO = impl::OpenHashMap::EqualTo<K>>
  class OpenHashMap
  {
  private:

    typedef Pair<K const, V> ValueType;
    typedef impl::OpenHashMap::NodeIndex NodeIndex;

    struct BucketNode
    {
      NodeIndex   next;
    };

    struct DataNode
    {
      ValueType   kv;
      NodeIndex   next;
      NodeIndex   prev;
    };

  public:

    typedef uint64_t myInt;
    typedef double   myFloat;

  public:

    //Iterates through the map as sorted by the criterion
    class const_iterator
    {
      friend class OpenHashMap;
      friend class iterator;
    private:

      const_iterator(DataNode const*);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const& a_it);
      const_iterator& operator=(const_iterator const& a_other);

      bool operator==(const_iterator const& a_it) const;
      bool operator!=(const_iterator const& a_it) const;

      ValueType const* operator->() const;
      ValueType const& operator*() const;

      const_iterator operator+(size_t) const;
      const_iterator operator-(size_t) const;

      const_iterator& operator+=(size_t);
      const_iterator& operator-=(size_t);

      const_iterator& operator++();
      const_iterator operator++(int);
      const_iterator& operator--();
      const_iterator operator--(int);

    private:
      DataNode const* m_pNode;
    };

    //Iterates through the map as sorted by the criterion
    class iterator
    {
      friend class OpenHashMap;
    private:

      iterator(DataNode*);

    public:

      iterator();
      ~iterator();

      iterator(iterator const& a_it);
      iterator& operator=(iterator const& a_other);

      bool operator==(iterator const& a_it) const;
      bool operator!=(iterator const& a_it) const;

      ValueType* operator->();
      ValueType& operator*();

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
      DataNode* m_pNode;
    };

  public:

    OpenHashMap();
    explicit OpenHashMap(size_t nBuckets,
                         HASHER const & hasher = HASHER(),
                         EQUALTO const & equalTo = EQUALTO());
    ~OpenHashMap();

    OpenHashMap(OpenHashMap const&);
    OpenHashMap& operator=(OpenHashMap const&);

    OpenHashMap(OpenHashMap&&)  noexcept;
    OpenHashMap& operator=(OpenHashMap&&)  noexcept;

    //Returns ref to item if found, other creates a new object and returns ref.
    V & operator[](K const &);

    //Returns nullptr if key not found
    V * at(K const &);

    //Returns nullptr if key not found
    V const * at(K const &) const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    //Returns pointer to newly inserted item, or current item if it already exists.
    V* insert(K const&, V const&);
    //V* insert(K&&, V&&);//TODO
    void erase(K const&);
    iterator erase(iterator);
    size_t size() const;
    size_t bucket_count() const;

    void clear();
    bool empty() const;

    //! Returns the current load factor in the unordered_map container.
    //!
    //! The load factor is the ratio between the number of elements in the 
    //! container(its size) and the number of buckets(BucketCount) :
    //!
    //!     load_factor = size / BucketCount
    //!
    //! The load factor influences the probability of collision in the OpenHashTable
    //! (i.e., the probability of two elements being located in the same bucket).
    //!
    //! The container automatically increases the number of buckets to keep the 
    //! load factor below a specific threshold(its max_load_factor), causing a 
    //! rehash each time an expansion is needed.
    //!
    //! To retrieve or change this threshold, use member function max_load_factor.
    myFloat load_factor() const;

    myFloat max_load_factor() const;
    void set_max_load_factor(myFloat);

    //Will force a rehash.
    //Will choose the closest prime equal to or larger than input.
    void set_buckets(size_t bucketCount);

    //Debug
    void Print();

  private:

    void EraseAtIndex(size_t);
    static bool IsValidLoadFactor(myFloat);

    //Assumes valid bucketCount and loadFactor
    void Rehash(PoolSizeMngr_Prime, myFloat loadFactor);
    void DestructAll(); //Destructs all objects, retains memory
    void FreeMemory();  //Destructs and frees memory
    
    void AllocateMemory();  //Assumes no memory currently allocated. nodePoolSize and buckCountIndex need to be set
    void InitMemory(); //Set up default arrays
    void Init(OpenHashMap const &); //Assumes no allocated memory
    
    size_t Index(K const &) const;
    
    //bool: does node exist?
    //Node const *: existing node or the last in the bucket list
    Pair<bool, NodeIndex> FindNode(K const &) const;

    //Returns 0 on error
    static size_t DataPoolSize(size_t bucketCount, myFloat maxLoadFactor);
    size_t DataPoolSize() const;

  private:

    myFloat            m_maxLoadFactor;
    HASHER             m_hasher;
    EQUALTO            m_equalTo;

    PoolSizeMngr_Prime m_poolSizeMngr;
    DataNode *         m_pDataNodes;
    NodeIndex          m_nextFreeIndex;
                       
    size_t             m_nItems;           //Number of curent elements
    BucketNode *       m_pBuckets;
  };

  //------------------------------------------------------------------------------------------------
  // const_iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::const_iterator(DataNode const* a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::const_iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::~const_iterator()
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::const_iterator(const_iterator const& a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator=(const_iterator const& a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator==(const_iterator const& a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator!=(const_iterator const& a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator+(size_t a_val) const
  {
    DataNode pNode = m_pNode + a_val;
    return const_iterator(pNode);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator-(size_t a_val) const
  {
    DataNode pNode = m_pNode - a_val;
    return const_iterator(pNode);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator+=(size_t a_val)
  {
    m_pNode += a_val;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator-=(size_t a_val)
  {
    m_pNode -= a_val;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator++()
  {
    m_pNode++;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator--()
  {
    m_pNode--;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::ValueType const*
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator->() const
  {
    return &(m_pNode->kv);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::ValueType const&
    OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator::operator*() const
  {
    return m_pNode->kv;
  }

  //------------------------------------------------------------------------------------------------
  // iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::iterator::iterator(DataNode* a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::iterator::iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::iterator::~iterator()
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::iterator::iterator(iterator const& a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator=(iterator const& a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator==(iterator const& a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator!=(iterator const& a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator+(size_t a_val) const
  {
    DataNode * pNode = m_pNode + a_val;
    return iterator(pNode);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator-(size_t a_val) const
  {
    DataNode* pNode = m_pNode - a_val;
    return iterator(pNode);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator+=(size_t a_val)
  {
    m_pNode += a_val;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator-=(size_t a_val)
  {
    m_pNode -= a_val;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator++()
  {
    m_pNode++;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator--()
  {
    m_pNode--;
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::ValueType*
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator->()
  {
    return &(m_pNode->kv);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::ValueType&
    OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator*()
  {
    return m_pNode->kv;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::iterator::operator
    typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator() const
  {
    return const_iterator(m_pNode);
  }

  //------------------------------------------------------------------------------------------------
  // OpenHashMap
  //------------------------------------------------------------------------------------------------

  //! Default constructor.
  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::OpenHashMap()
    : m_maxLoadFactor(impl::OpenHashMap::defaultLoadFactor)
    , m_hasher()
    , m_equalTo()
    , m_poolSizeMngr(impl::OpenHashMap::defaultBucketCount)
    , m_pDataNodes(nullptr)
    , m_nextFreeIndex(NodeIndex::Data, 0)
    , m_nItems(0)
    , m_pBuckets(nullptr)
  {
    AllocateMemory();
    InitMemory();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::OpenHashMap(size_t a_nBuckets,
                                                  HASHER const& a_hasher,
                                                  EQUALTO const& a_equalTo)
    : m_maxLoadFactor(impl::OpenHashMap::defaultLoadFactor)
    , m_hasher(a_hasher)
    , m_equalTo(a_equalTo)
    , m_poolSizeMngr(impl::OpenHashMap::defaultBucketCount)
    , m_pDataNodes(nullptr)
    , m_nextFreeIndex(NodeIndex::Data, 0)
    , m_nItems(0)
    , m_pBuckets(nullptr)
  {
    PoolSizeMngr_Prime psm(a_nBuckets);
    set_buckets(psm, m_maxLoadFactor);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::~OpenHashMap()
  {
    DestructAll();
    FreeMemory();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::OpenHashMap(OpenHashMap const& a_other)
    : m_maxLoadFactor(impl::OpenHashMap::defaultLoadFactor)
    , m_hasher()
    , m_equalTo()
    , m_poolSizeMngr(impl::OpenHashMap::defaultBucketCount)
    , m_pDataNodes(nullptr)
    , m_nextFreeIndex(NodeIndex::Data, 0)
    , m_nItems(0)
    , m_pBuckets(nullptr)
  {
    Init(a_other);
  }


  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO> & OpenHashMap<K, V, HASHER, EQUALTO>::operator=(OpenHashMap const& a_other)
  {
    if (this != &a_other)
    {
      DestructAll();
      FreeMemory();
      Init(a_other);
    }
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>::OpenHashMap(OpenHashMap && a_other) noexcept
    : m_maxLoadFactor(a_other.m_maxLoadFactor)
    , m_hasher(a_other.m_hasher)
    , m_equalTo(a_other.m_equalTo)
    , m_poolSizeMngr(a_other.m_poolSizeMngr)
    , m_pDataNodes(a_other.m_pDataNodes)
    , m_nextFreeIndex(a_other.m_nextFreeIndex)
    , m_nItems(a_other.m_nItems)
    , m_pBuckets(a_other.m_pBuckets)
  {
    a_other.m_pDataNodes = nullptr;
    a_other.m_pBuckets = nullptr;
    a_other.m_nItems = 0;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  OpenHashMap<K, V, HASHER, EQUALTO>& OpenHashMap<K, V, HASHER, EQUALTO>::operator=(OpenHashMap && a_other) noexcept
  {
    if (this != &a_other)
    {
      DestructAll();
      FreeMemory();
      
      m_maxLoadFactor = a_other.m_maxLoadFactor;
      m_hasher = a_other.m_hasher;
      m_equalTo = a_other.m_equalTo;
      m_poolSizeMngr = a_other.m_poolSizeMngr;
      m_pDataNodes = a_other.m_pDataNodes;
      m_nextFreeIndex = a_other.m_nextFreeIndex;
      m_nItems = a_other.m_nItems;
      m_pBuckets = a_other.m_pBuckets;

      a_other.m_pDataNodes = nullptr;
      a_other.m_pBuckets = nullptr;
      a_other.m_nItems = 0;
    }
    return *this;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  V & OpenHashMap<K, V, HASHER, EQUALTO>::operator[](K const & a_key)
  {
    return *insert(a_key, V());
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  size_t OpenHashMap<K, V, HASHER, EQUALTO>::Index(K const& a_key) const
  {
    return m_hasher(a_key) % m_poolSizeMngr.GetSize();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  Pair<bool , typename OpenHashMap<K, V, HASHER, EQUALTO>::NodeIndex>
    OpenHashMap<K, V, HASHER, EQUALTO>::FindNode(K const& a_key) const
  {
    size_t index = Index(a_key);
    if (m_pBuckets[index].next.IsNull())
    {
      NodeIndex bucketIndex(NodeIndex::Bucket, index);
      return Pair<bool, NodeIndex>{false, bucketIndex};
    }

    NodeIndex dataIndex = m_pBuckets[index].next;
    bool found = false;

    while (true)
    {
      if (m_equalTo(m_pDataNodes[dataIndex].kv.first, a_key))
      {
        found = true;
        break;
      }

      if (m_pDataNodes[dataIndex].next.IsNull())
        break;

      dataIndex = m_pDataNodes[dataIndex].next;
    }
    return Pair<bool, NodeIndex>{found, dataIndex};
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  V * OpenHashMap<K, V, HASHER, EQUALTO>::at(K const & a_key)
  {
    Pair<bool, NodeIndex> result = FindNode(a_key);
    if (!result.first)
      return nullptr;
    return &(m_pDataNodes[result.second].kv.second);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  V const * OpenHashMap<K, V, HASHER, EQUALTO>::at(K const& a_key) const
  {
    Pair<bool, NodeIndex> result = FindNode(a_key);
    if (!result.first)
      return nullptr;
    return &(m_pDataNodes[result.second].kv.second);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::begin()
  {
    return iterator(m_pDataNodes);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::end()
  {
    return iterator(&m_pDataNodes[m_nItems]);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::cbegin() const
  {
    return const_iterator(m_pDataNodes);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::const_iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::cend() const
  {
    return const_iterator(&m_pDataNodes[m_nItems]);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::iterator
    OpenHashMap<K, V, HASHER, EQUALTO>::erase(iterator a_it)
  {
    EraseAtIndex(static_cast<size_t>(a_it.m_pNode - m_pDataNodes));
    return a_it;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  V* OpenHashMap<K, V, HASHER, EQUALTO>::insert(K const& a_key, V const& a_value)
   {
    Pair<bool, NodeIndex> result = FindNode(a_key);
    if (result.first)
      return &(m_pDataNodes[result.second].kv.second);

    if ((m_nItems + 1) >= DataPoolSize())
    {
      PoolSizeMngr_Prime psm(m_poolSizeMngr);
      psm.SetNextPoolSize();
      Rehash(psm, m_maxLoadFactor);
      result = FindNode(a_key);
    }

    //Get a free node
    NodeIndex newNode = m_nextFreeIndex;
    m_nextFreeIndex = m_pDataNodes[m_nextFreeIndex].next;
    m_pDataNodes[newNode].next.SetNull();

    //Insert into end of chain
    m_pDataNodes[newNode].prev = result.second;
    if (result.second.GetType() == NodeIndex::Data)
      m_pDataNodes[result.second].next = newNode;
    else
      m_pBuckets[result.second].next = newNode;

    new (&m_pDataNodes[newNode].kv) ValueType{a_key, a_value};
    m_nItems++;
    return &m_pDataNodes[newNode].kv.second;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::EraseAtIndex(size_t a_index)
  {
    m_pDataNodes[a_index].kv.~ValueType();

    DataNode* t = &m_pDataNodes[a_index];

    //Break node from the chain
    if (!t->next.IsNull())
      m_pDataNodes[t->next].prev = t->prev;

    //No need to check if prev exists. If the key was found, there
    //will always be a previous node, either another DataNode or the
    //BucketNode
    if (t->prev.GetType() == NodeIndex::Data)
      m_pDataNodes[t->prev].next = t->next;
    else
      m_pBuckets[t->prev].next = t->next;

    if (t != &m_pDataNodes[m_nItems - 1])
    {
      memcpy(t, &m_pDataNodes[m_nItems - 1], sizeof(DataNode));
      NodeIndex t_index(NodeIndex::Data, static_cast<uint64_t>(t - m_pDataNodes));
      if (!t->next.IsNull())
        m_pDataNodes[t->next].prev = t_index;

      if (t->prev.GetType() == NodeIndex::Data)
        m_pDataNodes[t->prev].next = t_index;
      else
        m_pBuckets[t->prev].next = t_index;
    }

    //Place node back on the free node chain
    NodeIndex newFree(NodeIndex::Data, (m_nItems - 1));
    m_pDataNodes[newFree].next = m_nextFreeIndex;
    m_nextFreeIndex = newFree;

    m_nItems--;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::erase(K const& a_key)
  {
    Pair<bool, NodeIndex> result = FindNode(a_key);
    if (!result.first)
      return;

    EraseAtIndex(static_cast<size_t>(result.second.GetIndex()));
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  size_t OpenHashMap<K, V, HASHER, EQUALTO>::size() const
  {
    return m_nItems;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  size_t OpenHashMap<K, V, HASHER, EQUALTO>::bucket_count() const
  {
    return m_poolSizeMngr.GetSize();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::clear()
  {
    DestructAll();
    InitMemory();
    m_nItems = 0;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::empty() const
  {
    return m_nItems == 0;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::myFloat
    OpenHashMap<K, V, HASHER, EQUALTO>::load_factor() const
  {
    return static_cast<myFloat>(m_nItems) / bucket_count();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  typename OpenHashMap<K, V, HASHER, EQUALTO>::myFloat
    OpenHashMap<K, V, HASHER, EQUALTO>::max_load_factor() const
  {
    return m_maxLoadFactor;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  bool OpenHashMap<K, V, HASHER, EQUALTO>::IsValidLoadFactor(myFloat a_lf)
  {
    return ((a_lf >= impl::OpenHashMap::loadFactorBounds[0]) 
         && (a_lf <= impl::OpenHashMap::loadFactorBounds[1]));
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::set_max_load_factor(myFloat a_loadFactor)
  {
    if (!IsValidLoadFactor(a_loadFactor))
      return;

    Rehash(m_poolSizeMngr, a_loadFactor);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::set_buckets(size_t a_bucketCount)
  {
    PoolSizeMngr_Prime psm(a_bucketCount);

    myFloat newLoadFactor = static_cast<myFloat>(m_nItems) / static_cast<myFloat>(psm.GetSize());
    if (newLoadFactor > m_maxLoadFactor)
      return;

    Rehash(psm, m_maxLoadFactor);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  size_t OpenHashMap<K, V, HASHER, EQUALTO>::DataPoolSize(size_t a_bucketCount, myFloat a_maxLoadFactor)
  {
    myFloat arraySize_float = static_cast<myFloat>(a_bucketCount) * a_maxLoadFactor;

    if (arraySize_float > static_cast<myFloat>(impl::OpenHashMap::maxBucketCount))
      return 0;

    size_t arraySize_int = 1 + static_cast<size_t>(arraySize_float);

    if (static_cast<myFloat>(arraySize_int) < arraySize_float //integer overflow
      || arraySize_float > static_cast<myFloat>(impl::OpenHashMap::maxBucketCount))
      return 0;
    return arraySize_int;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  size_t OpenHashMap<K, V, HASHER, EQUALTO>::DataPoolSize() const
  {
    return DataPoolSize(bucket_count(), max_load_factor());
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::Print()
  {
    std::cout << "item count: " << size() << '\n';
    std::cout << "bucket count: " << bucket_count() << "\n";
    std::cout << "item pool size: " << DataPoolSize() << '\n';

    for (size_t i = 0; i < bucket_count(); i++)
    {
      std::cout << "\n[" << i << "]";
      NodeIndex index = m_pBuckets[i].next;
      while (!index.IsNull())
      {
        std::cout << "(" << index.GetIndex()
          << ", " << m_pDataNodes[index].kv.first << "),";
        index = m_pDataNodes[index].next;
      }
    }

    //TODO Nodes in memory...
    std::cout << "\n\nKeys in memory\n";
    for (size_t i = 0; i < m_nItems; i++)
    {
      std::cout << "[" << i << "]: " << m_pDataNodes[i].kv.first << '\n';
    }

    NodeIndex index = m_nextFreeIndex;
    std::cout << "\nFree chain";
    while (!index.IsNull())
    {
      std::cout << ", " << index.GetIndex();
      index = m_pDataNodes[index].next;
    }
    std::cout << "\n\n";
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::AllocateMemory()
  {
    BucketNode* pNewBucketArray = static_cast<BucketNode*>(malloc(bucket_count() * sizeof(BucketNode)));
    DataNode* pNewDataArray = static_cast<DataNode*>(malloc(DataPoolSize() * sizeof(DataNode)));

    if (pNewBucketArray == nullptr || pNewDataArray == nullptr)
    {
      free(pNewBucketArray);
      free(pNewDataArray);
      throw std::exception("OpenHashMap::AllocateMemory(): Failed to allocate memory!");
    }

    m_pBuckets = pNewBucketArray;
    m_pDataNodes = pNewDataArray;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::Rehash(PoolSizeMngr_Prime a_bucketCount, myFloat a_maxLoadFactor)
  {
    size_t arraySize = DataPoolSize(a_bucketCount.GetSize(), a_maxLoadFactor);

    //If the new bucketCount/maxLoadFactor combination will not fit the current number of items.
    if (arraySize < m_nItems)
      return;

    //Save current state
    BucketNode* old_pBuckets = m_pBuckets;
    DataNode* old_pDataNodes = m_pDataNodes;
    myFloat old_maxLoadFactor = m_maxLoadFactor;
    PoolSizeMngr_Prime old_poolSizeMngr = m_poolSizeMngr;

    //Set new state
    m_pBuckets = nullptr;
    m_pDataNodes = nullptr;
    m_poolSizeMngr = a_bucketCount;
    m_maxLoadFactor = a_maxLoadFactor;

    try
    {
      AllocateMemory();
    }
    catch (std::exception e)
    {
      //restore state
      m_pBuckets = old_pBuckets;
      m_pDataNodes = old_pDataNodes;
      m_maxLoadFactor = old_maxLoadFactor;
      m_poolSizeMngr = old_poolSizeMngr;

      throw e;
    }

    //Don't need old buckets anymore;
    free(old_pBuckets);
    size_t n = m_nItems;
    m_nItems = 0;

    InitMemory();
    for (size_t i = 0; i < n; i++)
      insert(old_pDataNodes[i].kv.first, old_pDataNodes[i].kv.second);

    free(old_pDataNodes);
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::DestructAll()
  {
    for (size_t i = 0; i < m_nItems; i++)
      m_pDataNodes[i].kv.~ValueType();
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::FreeMemory()
  {
    free(m_pBuckets);
    free(m_pDataNodes);
    m_pBuckets = nullptr;
    m_pDataNodes = nullptr;
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::InitMemory()
  {
    if (DataPoolSize() > 0 && m_pDataNodes != nullptr)
    {
      for (size_t i = 0; (i + 1) < DataPoolSize(); i++)
      {
        m_pDataNodes[i].next = NodeIndex(NodeIndex::Data, static_cast<uint64_t>(i + 1));
        //m_pDataNodes[i + 1].prev = NodeIndex(NodeIndex::Data, static_cast<uint64_t>(i));
      }

      m_pDataNodes[DataPoolSize() - 1].next = NodeIndex(NodeIndex::Data, NodeIndex::NULLNODE);
      m_nextFreeIndex.Set(NodeIndex::Data, 0);
    }
    else
    {
      m_nextFreeIndex.Set(NodeIndex::Data, NodeIndex::NULLNODE);
    }

    if (m_pBuckets != nullptr)
    {
      for (size_t i = 0; i < m_poolSizeMngr.GetSize(); i++)
        m_pBuckets[i].next = NodeIndex(NodeIndex::Data, NodeIndex::NULLNODE);
    }
  }

  template<typename K, typename V, class HASHER, class EQUALTO>
  void OpenHashMap<K, V, HASHER, EQUALTO>::Init(OpenHashMap const & a_other)
  {
    m_maxLoadFactor = a_other.m_maxLoadFactor;
    m_poolSizeMngr = a_other.m_poolSizeMngr;

    try
    {
      AllocateMemory();
    }
    catch (std::exception e)
    {
      m_maxLoadFactor = impl::OpenHashMap::defaultLoadFactor;
      m_poolSizeMngr.SetSize(impl::OpenHashMap::defaultBucketCount);
      throw e;
    }

    m_hasher = a_other.m_hasher;
    m_equalTo = a_other.m_equalTo;
    m_nItems = a_other.m_nItems;
    m_nextFreeIndex = a_other.m_nextFreeIndex;

    memcpy(m_pBuckets, a_other.m_pBuckets, a_other.m_poolSizeMngr.GetSize() * sizeof(BucketNode));
    
    for (size_t i = 0 i < a_other.m_nItems; i++)
      new (&m_pDataNodes[i]) ValueType(a_other.m_pDataNodes[i]);

    for (size_t i = a_other.m_nItems; i < a_other.DataPoolSize(); i++)
    {
      m_pDataNodes[i].next = a_other.m_pDataNodes[i].next;
      m_pDataNodes[i].prev = a_other.m_pDataNodes[i].prev;
    }
  }
}

#endif