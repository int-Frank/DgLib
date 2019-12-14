//@group Math

//! @file DgRNG.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: RNG

#ifndef DGRNG_GLOBAL_H
#define DGRNG_GLOBAL_H

#include "impl/DgRNG_Base.h"

namespace Dg
{
  //Typically seeded once on application startup.
  //NOT THREAD SAFE!
  class RNG_Global : public RNG_Base
  {
  public:

    //! Seed the internal generator from one value.
    void SetSeed(uint32_t seed);

    //! Seed the internal generator from two values.
    void SetSeed(uint32_t seed1, uint32_t seed2);

    //! Get random unsigned integer.
    //! This is the heart of the RNG.
    uint32_t GetUint();

  private:

    static uint32_t s_w;
    static uint32_t s_z;
  };
}


#endif