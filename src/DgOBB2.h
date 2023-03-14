//@group Math/Geometry

#ifndef DGOBB2_H
#define DGOBB2_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  // The 'perp' vector is the vector perpeddicular to the axis, going CCW.
  template<typename Real>
  class OBB2
  {
  public:

    OBB2();
    OBB2(Vector2<Real> const &centre, Vector2<Real> const &axis, Real hlAxis, Real hlPerp);

    void GetBasis(Vector2<Real> &axis, Vector2<Real> &perp) const;
    void GetHalfLengths(Real &axis, Real &perp) const;
    Vector2<Real> const &GetCenter() const;

  private:
    Vector2<Real> m_center;
    Vector2<Real> m_axis;
    Vector2<Real> m_perp;
    Real          m_halfLengths[2];
  };

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real>
  OBB2<Real>::OBB2()
    : m_center(Zeros<Real, 2>())
    , m_axis(XAxis<Real, 2>())
    , m_perp(YAxis<Real, 2>())
    , m_halfLengths{ static_cast<Real>(1), static_cast<Real>(1) }
  {
  }

  template<typename Real>
  OBB2<Real>::OBB2(Vector2<Real> const &centre, Vector2<Real> const &axis, Real hlAxis, Real hlPerp)
    : m_center(centre)
    , m_axis(Dg::Normalize(axis))
    , m_perp(Vector2<Real>(-m_axis.y(), m_axis.x()))
    , m_halfLengths{ hlAxis, hlPerp }
  {
    
  }

  template<typename Real>
  void OBB2<Real>::GetBasis(Vector2<Real> &axis, Vector2<Real> &perp) const
  {
    axis = m_axis;
    perp = m_perp;
  }

  template<typename Real>
  void OBB2<Real>::GetHalfLengths(Real &axis, Real &perp) const
  {
    axis = m_halfLengths[0];
    perp = m_halfLengths[1];
  }

  template<typename Real>
  Vector2<Real> const &OBB2<Real>::GetCenter() const
  {
    return m_center;
  }
}

#endif