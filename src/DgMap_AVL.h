//@group Collections

#ifndef DGMAP_AVL_H
#define DGMAP_AVL_H

#include "DgPair.h"
#include "DgTree_AVL.h"

namespace Dg
{
  template<typename KeyType, typename ValueType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  class _Map_AVL : public Tree_AVL<KeyType, ValueType, Compare>
  {
  public:

    decltype(ValueType::second) & operator[](KeyType const & a_key)
    {
      // We might not need to expand if the key already exists, but so just in case.
      if ((m_nItems + 1) == m_poolSize.GetSize())
        Extend();

      Node *pNode = nullptr;
      m_pRoot = __Insert(m_pRoot, nullptr, ValueType(a_key), pNode);
      return pNode->data.second;
    }

    decltype(ValueType::second) & at(KeyType const & a_key)
    {
      Node * pResult;
      if (!ValueExists(a_key, pResult))
        throw std::out_of_range("Invalid key!");
      return pResult->data.second;
    }

    decltype(ValueType::second) const & at(KeyType const & a_key) const
    {
      Node * pResult;
      if (!ValueExists(a_key, pResult))
        throw std::out_of_range("Invalid key!");
      return pResult->data.second;
    }

  private:

    KeyType const & GetKeyType(ValueType const & a_val) const override
    {
      return a_val.first;
    }
  };

  template<typename KeyType, typename ValueType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  using Map_AVL = _Map_AVL<KeyType, ::Dg::Pair<KeyType, ValueType>, Compare>;
}

#endif