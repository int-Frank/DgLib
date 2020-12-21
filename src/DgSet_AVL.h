//@group Collections

#ifndef DGSET_AVL_H
#define DGSET_AVL_H

#include "DgTree_AVL.h"

namespace Dg
{
  template<typename ValueType, bool (*Compare)(ValueType const &, ValueType const &) = impl::Less<ValueType>>
  class Set_AVL : public Tree_AVL<ValueType, ValueType, Compare>
  {
  private:

    ValueType const & GetKeyType(ValueType const & a_val) const override
    {
      return a_val;
    }
  };
}

#endif