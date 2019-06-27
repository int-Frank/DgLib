//! @file RNG.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class definitions: RNG

#include "DgRNG.h"

namespace Dg
{
  namespace RNG
  {
    //--------------------------------------------------------------------------------
    //	@	Statics
    //--------------------------------------------------------------------------------
    static unsigned int s_w = 521288629;
    static unsigned int s_z = 362436069;


    //--------------------------------------------------------------------------------
    //	@	RNG::SetSeed()
    //--------------------------------------------------------------------------------
    void RNG::SetSeed(unsigned int seed)
    {
      s_w = seed;

    }	//End: RNG::SetSeed()


    //--------------------------------------------------------------------------------
    //	@	RNG::SetSeed()
    //--------------------------------------------------------------------------------
    void RNG::SetSeed(unsigned int u, unsigned int v)
    {
      if (u != 0) s_w = u;
      if (v != 0) s_z = v;

    }	//End: RNG::SetSeed()


    unsigned int RNG::GetUint()
    {
      s_z = 36969 * (s_z & 65535) + (s_z >> 16);
      s_w = 18000 * (s_w & 65535) + (s_w >> 16);
      return (s_z << 16) + s_w;

    }	//End: RNG::GetUint()

    bool RNG::FlipCoin()
    {
      return (GetUint() & 1) == 1;
    }

    //--------------------------------------------------------------------------------
    //	@	RNG::GetUint()
    //--------------------------------------------------------------------------------
    unsigned int RNG::GetUint(unsigned int a, unsigned int b)
    {
      if (a >= b)
      {
        return a;
      }

      unsigned int val = GetUint();
      unsigned int range = b - a;
      return a + (val % range);

    }	//End: RNG::GetUint()
  }

}