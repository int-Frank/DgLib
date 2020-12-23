//@group Collections

#ifndef DGSET_AVL_H
#define DGSET_AVL_H

#include "DgTree_AVL.h"

namespace Dg
{
  template<typename KeyType, bool (*Compare)(KeyType const &, KeyType const &) = impl::Less<KeyType>>
  using Set_AVL = Tree_AVL<KeyType, KeyType, Compare>;
}

#endif