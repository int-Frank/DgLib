//@group Collections

#ifndef DGPAIR_H
#define DGPAIR_H

namespace Dg
{
  template<typename T0, typename T1>
  struct Pair
  {
    Pair(): first() {}

    Pair(T0 const & t0)
      : first(t0)
    {
    }

    Pair(T0 const & t0, T1 const & t1)
      : first(t0)
      , second(t1)
    {
    
    }

    T0 const first;
    T1 second;
  };
}

#endif