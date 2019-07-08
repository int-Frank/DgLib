//! @file DgRNG.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: RNG

#ifndef DGRNG_LOCAL_H
#define DGRNG_LOCAL_H

#include "impl/DgRNG_Base.h"

namespace Dg
{
  //Seeds belong to the object. You will need to seed every time a RNG_Local
  //is created. Use when thread safety is needed.
  class RNG_Local : public RNG_Base
  {
  public:

    RNG_Local();

    //! Seed the internal generator from one value.
    void SetSeed(uint32_t seed);

    //! Seed the internal generator from two values.
    void SetSeed(uint32_t seed1, uint32_t seed2);

    //! Get random unsigned integer.
    //! This is the heart of the RNG.
    uint32_t GetUint();

  private:

    uint32_t m_w;
    uint32_t m_z;
  };
}


#endif