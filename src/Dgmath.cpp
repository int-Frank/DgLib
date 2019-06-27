//! @file dgmath.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//! 
//! Definitions from dgmath.cpp

#include "dgmath.h"

namespace Dg
{
  float const Constants<float>::PI          = 3.141592653589793238462643383279f;
  float const Constants<float>::INVPI       = 0.31830988618379067153776752674503f;
  float const Constants<float>::EPSILON     = 0.0001f;
  float const Constants<float>::SQRT2       = 1.4142135623730950488016887242097f;
  float const Constants<float>::INVSQRT2    = 0.70710678118654752440084436210485f;

  double const Constants<double>::PI        = 3.141592653589793238462643383279;
  double const Constants<double>::INVPI     = 0.31830988618379067153776752674503;
  double const Constants<double>::EPSILON   = 0.00001;
  double const Constants<double>::SQRT2     = 1.4142135623730950488016887242097;
  double const Constants<double>::INVSQRT2  = 0.70710678118654752440084436210485;

	namespace impl
	{
		static int const TAB32[32] = {
		  0, 9, 1, 10, 13, 21, 2, 29,
		  11, 14, 16, 18, 22, 25, 3, 30,
		  8, 12, 20, 28, 15, 17, 24, 7,
		  19, 27, 23, 6, 26, 5, 4, 31 };

    static int const TAB64[64] = {
      63,  0, 58,  1, 59, 47, 53,  2,
      60, 39, 48, 27, 54, 33, 42,  3,
      61, 51, 37, 40, 49, 18, 28, 20,
      55, 30, 34, 11, 43, 14, 22,  4,
      62, 57, 46, 52, 38, 26, 32, 41,
      50, 36, 17, 19, 29, 10, 13, 21,
      56, 45, 25, 31, 35, 16,  9, 12,
      44, 24, 15,  8, 23,  7,  6,  5 };
	}


	//--------------------------------------------------------------------------------
	//	@	Log2_32()
	//--------------------------------------------------------------------------------
  uint32_t Log2_32(uint32_t input)
	{
		input |= (input >> 1);
		input |= (input >> 2);
		input |= (input >> 4);
		input |= (input >> 8);
		input |= (input >> 16);
		return impl::TAB32[(input * 0x07C4ACDD) >> 27];

	}	//End: Log2()


  //--------------------------------------------------------------------------------
	//	@	Log2_64()
	//--------------------------------------------------------------------------------
  uint64_t log2_64(uint64_t value)
  {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return impl::TAB64[((uint64_t)((value - (value >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
  }
}
