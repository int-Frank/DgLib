//! @file DgR3Vector_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015

#ifndef DGR3VECTOR_ANCILLARY_H
#define DGR3VECTOR_ANCILLARY_H

#include <cmath>

#include "DgR3Vector.h"
#include "DgRNG.h"

namespace Dg
{
  namespace R3
  {
    //-------------------------------------------------------------------------------
    //		@	GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomVector()
    {
      Real theta = RNG::GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * Dg::Constants<Real>::PI);
      Real rho = RNG::GetUniform<Real>(static_cast<Real>(-1.0), static_cast<Real>(1.0));

      Real val = sqrt(static_cast<Real>(1.0) - rho * rho);

      Real x = val * cos(theta);
      Real y = val * sin(theta);
      Real z = rho;

      return Vector<Real>({ x, y, z, static_cast<Real>(0.0) });
    }	//End: GetRandomVector()


    //-------------------------------------------------------------------------------
    //		@ GetRandomOrthonormalVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomOrthonormalVector(Vector<Real> const & a_axis)
    {
      Vector<Real> perp = a_axis.Perpendicular();
      Vector<Real> crs = a_axis.Cross(perp);
      Real phi = RNG::GetUniform(static_cast<Real>(0.0)
        , Dg::Constants<Real>::PI * static_cast<Real>(2.0));

      return (cos(phi) * perp + sin(phi) * crs);
    }	//End: GetRandomOrthonormalVector()


    //-------------------------------------------------------------------------------
    //		@ GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomVector(Vector<Real> const & a_axis, Real theta)
    {
      Dg::WrapNumber(static_cast<Real>(0.0)
        , Dg::Constants<Real>::PI
        , theta);

      Real bound = cos(Dg::Constants<Real>::PI - theta);
      Real x = RNG::GetUniform(static_cast<Real>(-1.0), bound);
      Real phi = Dg::Constants<Real>::PI - acos(x);
      return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
    }	//End: GetRandomVector()

    // Gram-Schmidt orthonormalization to generate orthonormal vectors from the
    // linearly independent inputs.  The function returns the smallest length of
    // the unnormalized vectors computed during the process.  If this value is
    // nearly zero, it is possible that the inputs are linearly dependent (within
    // numerical round-off errors).  On input, 1 <= numElements <= N and v[0]
    // through v[numElements-1] must be initialized.  On output, the vectors
    // v[0] through v[numElements-1] form an orthonormal set.
    template <typename Real>
    Real Orthonormalize(int a_numInputs, Vector<Real> * a_v)
    {
      if (a_v && 1 <= a_numInputs && a_numInputs <= 3)
      {
        Real minLength = a_v[0].Length();
        a_v[0] /= minLength;
        for (int i = 1; i < a_numInputs; ++i)
        {
          for (int j = 0; j < i; ++j)
          {
            Real dot = a_v[i].Dot(a_v[j]);
            a_v[i] -= a_v[j] * dot;
          }
          Real length = a_v[i].Length();
          a_v[i] /= length;
          if (length < minLength)
          {
            minLength = length;
          }
        }
        return minLength;
      }
      return static_cast<Real>(0);
    }

    // Compute a right-handed orthonormal basis for the orthogonal complement
    // of the input vectors.  The function returns the smallest length of the
    // unnormalized vectors computed during the process.  If this value is nearly
    // zero, it is possible that the inputs are linearly dependent (within
    // numerical round-off errors).  On input, numInputs must be 1 or 2 and
    // v[0] through v[numInputs-1] must be initialized.  On output, the
    // vectors v[0] through v[2] form an orthonormal set.
    template <typename Real>
    Real ComputeOrthogonalComplement(int a_numInputs, Vector<Real> * a_v)
    {
      if (a_numInputs == 1)
      {
        if (abs(a_v[0][0]) > abs(a_v[0][1]))
        {
          a_v[1].Set(-a_v[0][2], static_cast<Real>(0), +a_v[0][0], static_cast<Real>(0));
        }
        else
        {
          a_v[1].Set(static_cast<Real>(0), +a_v[0][2], -a_v[0][1], static_cast<Real>(0));
        }
        a_numInputs = 2;
      }

      if (a_numInputs == 2)
      {
        a_v[2] = a_v[0].Cross(a_v[1]);
        return Orthonormalize<Real>(3, a_v);
      }

      return static_cast<Real>(0);
    }
  }
}

#endif