//@group Math

//! @file DgR2Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGVECTOR_H
#define DGVECTOR_H

#include "DgMatrix.h"

#define Zeros2f ::Dg::Zeros<float, 2>
#define Zeros3f ::Dg::Zeros<float, 3>
#define Zeros4f ::Dg::Zeros<float, 4>

#define Zeros2d ::Dg::Zeros<double, 2>
#define Zeros3d ::Dg::Zeros<double, 3>
#define Zeros4d ::Dg::Zeros<double, 4>

#define xAxis2f ::Dg::XAxis<float, 2>
#define yAxis2f ::Dg::YAxis<float, 2>

#define xAxis3f ::Dg::XAxis<float, 3>
#define yAxis3f ::Dg::YAxis<float, 3>
#define zAxis3f ::Dg::ZAxis<float, 3>

#define xAxis4f ::Dg::XAxis<float, 4>
#define yAxis4f ::Dg::YAxis<float, 4>
#define zAxis4f ::Dg::ZAxis<float, 4>
#define wAxis4f ::Dg::WAxis<float, 4>

#define xAxis2d ::Dg::XAxis<double, 2>
#define yAxis2d ::Dg::YAxis<double, 2>

#define xAxis3d ::Dg::XAxis<double, 3>
#define yAxis3d ::Dg::YAxis<double, 3>
#define zAxis3d ::Dg::ZAxis<double, 3>

#define xAxis4d ::Dg::XAxis<double, 4>
#define yAxis4d ::Dg::YAxis<double, 4>
#define zAxis4d ::Dg::ZAxis<double, 4>
#define wAxis4d ::Dg::WAxis<double, 4>

namespace Dg
{
  template<typename Real, int R>
  using Vector = ::Dg::Matrix<1, R, Real>;

  template<typename Real>
  using Vector2 = ::Dg::Matrix<1, 2, Real>;

  template<typename Real>
  using Vector3 = ::Dg::Matrix<1, 3, Real>;

  template<typename Real>
  using Vector4 = ::Dg::Matrix<1, 4, Real>;

  template<typename Real, int R>
  Real MagSq(Matrix<1, R, Real> const & a_vec)
  {
    Real result = static_cast<Real>(0);
    for (int i = 0; i < R; i++)
      result += (a_vec[i] * a_vec[i]);
    return result;
  }

  template<typename Real, int R>
  Real Mag(Matrix<1, R, Real> const & a_vec)
  {
    return sqrt(MagSq(a_vec));
  }

  template<typename Real, int R>
  bool IsUnit(Matrix<1, R, Real> const & a_vec)
  {
    return Dg::IsZero(static_cast<Real>(1) - MagSq(a_vec));
  }

  template<typename Real, int R>
  Matrix<1, R, Real> XAxis()
  {
    static_assert(R > 0);

    Matrix<1, R, Real> result;
    result[0] = static_cast<Real>(1);

    for (int i = 1; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Matrix<1, R, Real> YAxis()
  {
    static_assert(R > 1);

    Matrix<1, R, Real> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(1);

    for (int i = 2; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Matrix<1, R, Real> ZAxis()
  {
    static_assert(R > 2);

    Matrix<1, R, Real> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(0);
    result[2] = static_cast<Real>(1);

    for (int i = 3; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Matrix<1, R, Real> WAxis()
  {
    static_assert(R > 3);

    Matrix<1, R, Real> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(0);
    result[2] = static_cast<Real>(0);
    result[3] = static_cast<Real>(1);

    for (int i = 4; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Matrix<1, R, Real> Zeros()
  {
    Matrix<1, R, Real> result;

    for (int i = 0; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  //! Make unit vector
  template<typename Real, int R>
  Matrix<1, R, Real> Normalize(Matrix<1, R, Real> const & a_vec)
  {
    Matrix<1, R, Real> result;
    Real magSq = MagSq(a_vec);

    if (Dg::IsZero(magSq))
    {
      result[0] = static_cast<Real>(1);
      for (int i = 1; i < R; i++)
        result[i] = static_cast<Real>(0);
    }
    else
    {
      Real factor = static_cast<Real>(1) / sqrt(magSq);
      for (int i = 0; i < R; i++)
        result[i] = a_vec[i] * factor;
    }
    return result;
  }

  template<typename Real>
  Real PerpDot(Matrix<1, 2, Real> const & a_v0, Matrix<1, 2, Real> const & a_v1)
  {
    return a_v0[0] * a_v1[1] - a_v0[1] * a_v1[0];
  }

  template<typename Real>
  Matrix<1, 2, Real> Perpendicular(Matrix<1, 2, Real> const & a_vec)
  {
    return Matrix<1, 2, Real>(-a_vec[1], a_vec[0]);
  }

  template<typename Real>
  Matrix<1, 3, Real> Perpendicular(Matrix<1, 3, Real> const & a_vec)
  {
    Matrix<1, 3, Real> result;

    if (!Dg::IsZero(a_vec[0]) || !Dg::IsZero(a_vec[1]))
    {
      result[0] = -a_vec[1];
      result[1] = a_vec[0];
      result[2] = static_cast<Real>(0);
    }
    else
    {
      result[0] = -a_vec[2];
      result[1] = static_cast<Real>(0);
      result[2] = a_vec[0];
    }
    
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> Cross(Vector<Real, R> const & a_v0, Vector<Real, R> const & a_v1)
  {
    static_assert(R == 3 || R == 4);

    Vector<Real, R> result;
    result[0] = a_v0[1] * a_v1[2] - a_v0[2] * a_v1[1];
    result[1] = a_v0[2] * a_v1[0] - a_v0[0] * a_v1[2];
    result[2] = a_v0[0] * a_v1[1] - a_v0[1] * a_v1[0];

    if constexpr (R == 4)
      result[3] = static_cast<Real>(0);

    return result;
  }	//End: Cross()
}

#endif