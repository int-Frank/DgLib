//! @file DgRNG.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: RNG

#ifndef DGRNG_BASE_H
#define DGRNG_BASE_H

#include <stdint.h>

namespace Dg
{

  //! @ingroup DgMath_types
  //!
  //!
  //! @brief A simple random number generator.
  //! Original code by John D. Cook
  //!
  //! @author Frank Hart
  //! @date 4/8/2015
  class RNG_Base
  {
  public:
    //! Seed the internal generator from one value.
    virtual void SetSeed(uint32_t seed) =0;

    //! Seed the internal generator from two values.
    virtual void SetSeed(uint32_t seed1, uint32_t seed2) =0;

    //! Get random unsigned integer.
    //! This is the heart of the RNG.
    virtual uint32_t GetUint() =0;

    //! Produce a uniform random sample from the open interval (0, 1).
    //! The method will not return either end point.
    template<class Real>
    Real GetUniform()
    {
      // 0 <= u < 2^32
      uint32_t u = GetUint();
      // The magic number below is 1/(2^32 + 2).
      // The result is strictly between 0 and 1.
      return static_cast<Real>(u + 1) * static_cast<Real>(2.328306435454494e-10);
    }

    //! Produce a uniform random sample from the open interval (a, b).
    template<class Real>
    Real GetUniform(Real a, Real b)
    {
      if (b < a)
        return a;

      Real range = b - a;
      return GetUniform<Real>() * range + a;
    }

    //! Get random unsigned integer within the interval [a, b].
    uint32_t GetUint(unsigned int a, unsigned int b);

    bool FlipCoin();

    //! Get a Gaussian random sample with mean 0 and std 1.
    template<class Real>
    Real GetNormal()
    {
      // Use Box-Muller algorithm
      Real u1 = GetUniform<Real>();
      Real u2 = GetUniform<Real>();
      Real r = sqrt(static_cast<Real>(-2.0)*log(u1));
      Real theta = static_cast<Real>(6.283185307179586476925286766559)*u2;
      return r*sin(theta);
    }

      //! Get a Gaussian random sample with specified mean and standard deviation.
    template<class Real>
    Real GetNormal(Real mean, Real std)
    {
      if (std <= Real(0.0))
        return mean;
      return mean + std * GetNormal<Real>();
    }

    //! Get a Gamma random sample with specified shape and scale.
    template<class Real>
    Real GetGamma(Real shape, Real scale)
    {
      // Implementation based on "A Simple Method for Generating Gamma Variables"
      // by George Marsaglia and Wai Wan Tsang.  ACM Transactions on Mathematical Software
      // Vol 26, No 3, September 2000, pages 363-372.

      Real d, c, x, xsquared, v, u;

      if (shape >= static_cast<Real>(1.0))
      {
        d = shape - static_cast<Real>(0.33333333333333333333333333333333);
        c = static_cast<Real>(1.0) / sqrt(static_cast<Real>(9.0)*d);
        for (;;)
        {
          do
          {
            x = GetNormal<Real>();
            v = static_cast<Real>(1.0) + c*x;
          } while (v <= static_cast<Real>(0.0));
          v = v*v*v;
          u = GetUniform<Real>();
          xsquared = x*x;
          if (u < static_cast<Real>(1.0) - static_cast<Real>(0.0331)*xsquared*xsquared || log(u) < static_cast<Real>(0.5)*xsquared + d*(static_cast<Real>(1.0) - v + log(v)))
            return scale*d*v;
        }
      }
      else if (shape <= static_cast<Real>(0.0))
      {
        /*string msg = string.Format("Shape must be positive. Received {0}.", shape);
        throw new ArgumentOutOfRangeException(msg)*/
        return static_cast<Real>(-1.0);
      }
      else
      {
        Real g = GetGamma(shape + static_cast<Real>(1.0), static_cast<Real>(1.0));
        Real w = GetUniform<Real>();
        return scale*g*std::pow(w, static_cast<Real>(1.0) / shape);
      }
    }
  };
}


#endif