//@group Collections

#ifndef DGMAP_AVL_H
#define DGMAP_AVL_H

#include <stdexcept>

#include "DgPair.h"
#include "DgTree_AVL.h"

namespace Dg
{
  namespace impl
  {
    template <typename T, typename U>
    U _Map_AVL_GetKey(T const &kv) { return kv.first; }
  }

  template<typename KeyType, typename ValueType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  class _Map_AVL : public Tree_AVL<KeyType, ValueType, impl::_Map_AVL_GetKey<ValueType, KeyType>, Compare>
  {
  public:

    decltype(ValueType::second) & operator[](KeyType const & a_key)
    {
      // We might not need to expand if the key already exists, but so just in case.
      if ((m_nItems + 1) == m_poolSize.GetSize())
        Extend();

      Node *pNode = nullptr;
      m_pRoot = __Insert(m_pRoot, nullptr, ValueType(a_key, decltype(ValueType::second)()), pNode);
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

    auto insert(KeyType const &key, decltype(ValueType::second) const &value)
    {
      return Tree_AVL::insert(Dg::Pair<KeyType const, decltype(ValueType::second)>(key, value));
    }
  };

  template<typename KeyType, typename ValueType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  using Map_AVL = _Map_AVL<KeyType, ::Dg::Pair<KeyType const, ValueType>, Compare>;
}

#endif