//@group Math/R3

//! @file DgR3Vector_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015

#ifndef DGGEOMETRYANCILLARY_H
#define DGGEOMETRYANCILLARY_H

#include <cmath>

#include "DgVector.h"
#include "impl/DgRNG_Base.h"

namespace Dg
{
  template<typename Real>
  Vector2<Real> GetRandomVector2(RNG_Base & a_rRNG)
  {
    Real theta = a_rRNG.GetUniform<Real>(static_cast<Real>(0), static_cast<Real>(2) * Dg::Constants<Real>::PI);

    Real x = cos(theta);
    Real y = sin(theta);

    return Vector2<Real>(x, y);
  }

  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomVector3(RNG_Base & a_rRNG)
  {
    Real theta = a_rRNG.GetUniform<Real>(static_cast<Real>(0), static_cast<Real>(2) * Dg::Constants<Real>::PI);
    Real rho = a_rRNG.GetUniform<Real>(static_cast<Real>(-1), static_cast<Real>(1));

    Real val = sqrt(static_cast<Real>(1) - rho * rho);

    Real x = val * cos(theta);
    Real y = val * sin(theta);
    Real z = rho;

    return Vector3<Real>({ x, y, z });
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomOrthonormalVector(Vector3<Real> const & a_axis, RNG_Base & a_rRNG)
  {
    Vector3<Real> perp = Perpendicular(a_axis);
    Vector3<Real> crs = Cross(a_axis, perp);
    Real phi = a_rRNG.GetUniform(static_cast<Real>(0), Dg::Constants<Real>::PI * static_cast<Real>(2));

    return (cos(phi) * perp + sin(phi) * crs);
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomVector(Vector3<Real> const & a_axis, Real theta, RNG_Base & a_rRNG)
  {
    Dg::WrapNumber(static_cast<Real>(0), Dg::Constants<Real>::PI, theta);

    Real bound = cos(Dg::Constants<Real>::PI - theta);
    Real x = a_rRNG.GetUniform(static_cast<Real>(-1), bound);
    Real phi = Dg::Constants<Real>::PI - acos(x);
    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis, a_rRNG));
  }	//End: GetRandomVector()

  //! Creates an orthogonal basis from two input vectors. 
  //!   - The vector 'A_in' is considered the principle axis.
  //!   - If A_in is a zero vector, A_out will be set to Vector<Real>::xAxis()
  //!   - If A_in and B_in are parallel, or B_in is a Zero vector, B_out will be chosen such that it is orthogonal to A_in
  //!   - Finally, C_out is created from A_out X B_out.
  template<typename Real>
  void GetBasis(Vector3<Real> const & a_A_in,
                Vector3<Real> const & a_B_in,
                Vector3<Real> & a_A_out,
                Vector3<Real> & a_B_out,
                Vector3<Real> & a_C_out)
  {
    if (a_A_in.IsZero())
    {
      a_A_out = XAxis<Real, 3>();
    }
    else
    {
      a_A_out = a_A_in;
      a_A_out = Normalize(a_A_out);
    }

    a_C_out = Cross(a_A_out, a_B_in);

    //Test to see if A_in and B_in are parallel
    if (IsZero(MagSq(a_C_out)))
    {
      a_B_out = Perpendicular(a_A_out);
      a_C_out = Cross(a_A_out, a_B_out);
    }
    else
    {
      a_C_out = Normalize(a_C_out);
      a_B_out = Cross(a_C_out, a_A_out);
    }
  }

  // Gram-Schmidt orthonormalization to generate orthonormal vectors from the
  // linearly independent inputs.  The function returns the smallest length of
  // the unnormalized vectors computed during the process.  If this value is
  // nearly zero, it is possible that the inputs are linearly dependent (within
  // numerical round-off errors).  On input, 1 <= numElements <= N and v[0]
  // through v[numElements-1] must be initialized.  On output, the vectors
  // v[0] through v[numElements-1] form an orthonormal set.
  template <typename Real>
  Real Orthonormalize(int a_numInputs, Vector3<Real> * a_v)
  {
    if (a_v && 1 <= a_numInputs && a_numInputs <= 3)
    {
      Real minLength = Mag(a_v[0]);
      a_v[0] /= minLength;
      for (int i = 1; i < a_numInputs; ++i)
      {
        for (int j = 0; j < i; ++j)
        {
          Real dot = Dot(a_v[i], a_v[j]);
          a_v[i] -= a_v[j] * dot;
        }
        Real length = Mag(a_v[i]);
        a_v[i] /= length;
        if (length < minLength)
          minLength = length;
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
  Real ComputeOrthogonalComplement(int a_numInputs, Vector3<Real> * a_v)
  {
    if (a_numInputs == 1)
    {
      if (abs(a_v[0][0]) > abs(a_v[0][1]))
        a_v[1] = Vector3<Real>(-a_v[0][2], static_cast<Real>(0), +a_v[0][0]);
      else
        a_v[1] = Vector3<Real>(static_cast<Real>(0), +a_v[0][2], -a_v[0][1]);
      a_numInputs = 2;
    }

    if (a_numInputs == 2)
    {
      a_v[2] = Cross(a_v[0], a_v[1]);
      return Orthonormalize<Real>(3, a_v);
    }

    return static_cast<Real>(0);
  }
}

#endif