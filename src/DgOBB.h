//@group Math/Geometry

#ifndef DGR3OBB_H
#define DGR3OBB_H

#include "DgVector.h"
#include "DgVQS.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real>
  class OBB
  {
  public:

    OBB();
    OBB(Vector3<Real> const &forward, Vector3<Real> const &left, Vector3<Real> const &up
      , Real lf, Real ll, Real lu
      , Vector3<Real> const &center);

    OBB(OBB const &a_other);
    OBB &operator= (OBB const &a_other);

    void GetBasis(Vector3<Real> &a_forward, Vector3<Real> &a_left, Vector3<Real> &a_up) const;
    void GetHalfLengths(Real &a_forward, Real &a_left, Real &a_up) const;
    Vector3<Real> const &GetCenter() const { return m_center; }

    void SetBasis(Vector3<Real> const &a_forward, Vector3<Real> const &a_left, Vector3<Real> const &a_up);
    void SetHalfLengths(Real a_lf, Real a_ll, Real a_lu);
    void SetCenter(Vector3<Real> const &a_center) { m_center = a_center; }

  private:
    Vector3<Real> m_basis[3];       //[f, l, u]
    Vector3<Real> m_center;
    Real          m_halfLengths[3]; //[f, l, u]
  };

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real>
  OBB<Real>::OBB()
    : m_basis{ XAxis<Real, 3>(), YAxis<Real, 3>(), ZAxis<Real, 3>() }
    , m_center(Zeros<Real, 3>())
    , m_halfLengths{ static_cast<Real>(1), static_cast<Real>(1), static_cast<Real>(1) }
  {
  }

  template<typename Real>
  OBB<Real>::OBB(Vector3<Real> const &forward, Vector3<Real> const &left, Vector3<Real> const &up
               , Real lf, Real ll, Real lu
               , Vector3<Real> const &center)
    : m_basis{ forward, left, up }
    , m_center(center)
    , m_halfLengths{ lf, ll, lu }
  {}

  template<typename Real>
  OBB<Real>::OBB(OBB const &a_other)
    : m_basis{ a_other.m_basis[0], a_other.m_basis[1], a_other.m_basis[2] }
    , m_center(a_other.m_center)
    , m_halfLengths{ a_other.m_halfLengths[0], a_other.m_halfLengths[1], a_other.m_halfLengths[2] }
  {}

  template<typename Real>
  OBB<Real>& OBB<Real>::operator= (OBB const &a_other)
  {
    m_basis[0] = a_other.m_basis[0];
    m_basis[1] = a_other.m_basis[1];
    m_basis[2] = a_other.m_basis[2];

    m_center = a_other.m_center;

    m_halfLengths[0] = a_other.m_halfLengths[0];
    m_halfLengths[1] = a_other.m_halfLengths[1];
    m_halfLengths[2] = a_other.m_halfLengths[2];

    return *this;
  }

  template<typename Real>
  void OBB<Real>::GetBasis(Vector3<Real> &a_forward,
    Vector3<Real> &a_left,
    Vector3<Real> &a_up) const
  {
    a_forward = m_basis[0];
    a_left = m_basis[1];
    a_up = m_basis[2];
  }

  template<typename Real>
  void OBB<Real>::GetHalfLengths(Real &a_forward,
    Real &a_left,
    Real &a_up) const
  {
    a_forward = m_halfLengths[0];
    a_left = m_halfLengths[1];
    a_up = m_halfLengths[2];
  }

  template<typename Real>
  void OBB<Real>::SetBasis(Vector3<Real> const &a_forward, Vector3<Real> const &a_left, Vector3<Real> const &a_up)
  {
    m_basis[0] = a_forward;
    m_basis[1] = a_left;
    m_basis[2] = a_up;
  }

  template<typename Real>
  void OBB<Real>::SetHalfLengths(Real a_lf, Real a_ll, Real a_lu)
  {
    m_halfLengths[0] = a_lf;
    m_halfLengths[1] = a_ll;
    m_halfLengths[2] = a_lu;
  }
}

#endif