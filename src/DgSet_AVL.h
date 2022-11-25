//@group Collections

#ifndef DGSET_AVL_H
#define DGSET_AVL_H

#include "DgTree_AVL.h"

namespace Dg
{
  namespace impl
  {
    template <typename T, typename U>
    U _Set_AVL_GetKey(T const &k) { return k; }
  }

  template<typename KeyType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  using Set_AVL = Tree_AVL<KeyType, KeyType, impl::_Set_AVL_GetKey<KeyType, KeyType>, Compare>;
}

#endif