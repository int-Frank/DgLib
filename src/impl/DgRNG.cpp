//! @file RNG.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class definitions: RNG

#include "DgRNG_Base.h"
#include "../DgRNG_Local.h"
#include "../DgRNG_Global.h"

namespace Dg
{
  //--------------------------------------------------------------------------------
  //	@	RNG_Base
  //--------------------------------------------------------------------------------
   RNG_Base::~RNG_Base()
   {
   
   }

  bool RNG_Base::FlipCoin()
  {
    return (GetUint() & 1) == 1;
  }

  unsigned int RNG_Base::GetUint(unsigned int a, unsigned int b)
  {
    if (a >= b)
    {
      return a;
    }

    unsigned int val = GetUint();
    unsigned int range = b - a;
    return a + (val % range);
  }

  //--------------------------------------------------------------------------------
  //	@	RNG_Local
  //--------------------------------------------------------------------------------
  RNG_Local::RNG_Local()
    : m_w(521288629)
    , m_z(362436069)
  {
  
  }

  void RNG_Local::SetSeed(uint32_t seed)
  {
    m_w = seed;
  }

  void RNG_Local::SetSeed(uint32_t u, uint32_t v)
  {
    if (u != 0) m_w = u;
    if (v != 0) m_z = v;
  }

  uint32_t RNG_Local::GetUint()
  {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
  }

  //--------------------------------------------------------------------------------
  //	@	RNG_Global
  //--------------------------------------------------------------------------------

  uint32_t RNG_Global::s_w = 521288629;
  uint32_t RNG_Global::s_z = 362436069;

  void RNG_Global::SetSeed(uint32_t seed)
  {
    s_w = seed;
  }

  void RNG_Global::SetSeed(uint32_t u, uint32_t v)
  {
    if (u != 0) s_w = u;
    if (v != 0) s_z = v;
  }

  uint32_t RNG_Global::GetUint()
  {
    s_z = 36969 * (s_z & 65535) + (s_z >> 16);
    s_w = 18000 * (s_w & 65535) + (s_w >> 16);
    return (s_z << 16) + s_w;
  }
}