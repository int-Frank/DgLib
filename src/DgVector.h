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
  template<typename Real, int R> using Vector = ::Dg::Matrix<1, R, Real>;
  template<typename Real> using Vector2 = ::Dg::Vector<Real, 2>;
  template<typename Real> using Vector3 = ::Dg::Vector<Real, 3>;
  template<typename Real> using Vector4 = ::Dg::Vector<Real, 4>;

  template<typename Real, int R> Real MagSq(Vector<Real, R> const & a_vec);
  template<typename Real, int R> Real Mag(Vector<Real, R> const & a_vec);
  template<typename Real, int R> bool IsUnit(Vector<Real, R> const & a_vec);
  template<typename Real, int R> Vector<Real, R> XAxis();
  template<typename Real, int R> Vector<Real, R> YAxis();
  template<typename Real, int R> Vector<Real, R> ZAxis();
  template<typename Real, int R> Vector<Real, R> WAxis();
  template<typename Real, int R> Vector<Real, R> Zeros();
  template<typename Real, int R> Vector<Real, R> Normalize(Vector<Real, R> const & a_vec);

  template<size_t M, size_t N, typename Real> Real Dot(Matrix<M, N, Real> const &, Matrix<M, N, Real> const &);
  template<typename Real, int R> Vector<Real, R> Cross(Vector<Real, R> const & a_v0, Vector<Real, R> const & a_v1);

  template<typename Real> Real PerpDot(Vector2<Real> const & a_v0, Vector2<Real> const & a_v1);
  template<typename Real> Vector2<Real> Perpendicular(Vector2<Real> const & a_vec);
  template<typename Real> Vector3<Real> Perpendicular(Vector3<Real> const & a_vec);

  //------------------------------------------------------------------------------
  // Implementation
  //------------------------------------------------------------------------------

  template<typename Real, int R>
  Real MagSq(Vector<Real, R> const & a_vec)
  {
    Real result = static_cast<Real>(0);
    for (int i = 0; i < R; i++)
      result += (a_vec[i] * a_vec[i]);
    return result;
  }

  template<typename Real, int R>
  Real Mag(Vector<Real, R> const & a_vec)
  {
    return sqrt(MagSq(a_vec));
  }

  template<typename Real, int R>
  bool IsUnit(Vector<Real, R> const & a_vec)
  {
    return Dg::IsZero(static_cast<Real>(1) - MagSq(a_vec));
  }

  template<typename Real, int R>
  Vector<Real, R> XAxis()
  {
    static_assert(R > 0);

    Vector<Real, R> result;
    result[0] = static_cast<Real>(1);

    for (int i = 1; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> YAxis()
  {
    static_assert(R > 1);

    Vector<Real, R> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(1);

    for (int i = 2; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> ZAxis()
  {
    static_assert(R > 2);

    Vector<Real, R> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(0);
    result[2] = static_cast<Real>(1);

    for (int i = 3; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> WAxis()
  {
    static_assert(R > 3);

    Vector<Real, R> result;
    result[0] = static_cast<Real>(0);
    result[1] = static_cast<Real>(0);
    result[2] = static_cast<Real>(0);
    result[3] = static_cast<Real>(1);

    for (int i = 4; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> Zeros()
  {
    Vector<Real, R> result;

    for (int i = 0; i < R; i++)
      result[i] = static_cast<Real>(0);
    return result;
  }

  //! Make unit vector
  template<typename Real, int R>
  Vector<Real, R> Normalize(Vector<Real, R> const & a_vec)
  {
    Vector<Real, R> result;
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
  Real PerpDot(Vector2<Real> const & a_v0, Vector2<Real> const & a_v1)
  {
    return a_v0[0] * a_v1[1] - a_v0[1] * a_v1[0];
  }

  template<typename Real>
  Vector2<Real> Perpendicular(Vector2<Real> const & a_vec)
  {
    return Vector2<Real>(-a_vec[1], a_vec[0]);
  }

  template<typename Real>
  Vector3<Real> Perpendicular(Vector3<Real> const & a_vec)
  {
    Vector3<Real> result;

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

  template<size_t M, size_t N, typename Real>
  Real Dot(Matrix<M, N, Real> const & a_mat0, Matrix<M, N, Real> const & a_mat1)
  {
    Real result = static_cast<Real>(0);
    for (size_t i = 0; i < M * N; i++)
      result += (a_mat0[i] * a_mat1[i]);
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
  }
}

#endif