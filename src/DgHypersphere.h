//@group Math/Geometry

#ifndef DGHYPERSPHERE_H
#define DGHYPERSPHERE_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real, int R>
  class Hypersphere
  {
  public:

    Hypersphere();
    Hypersphere(Vector<Real, R> const &origin, Real radius);
    Hypersphere(Hypersphere const &a_other);
    Hypersphere& operator= (Hypersphere const &);

    Vector<Real, R> const & Center() const { return m_origin; }
    Real Radius() const { return m_radius; }
    void SetCenter(Vector<Real, R> const &);
    void SetRadius(Real);
    void Set(Vector<Real, R> const & origin, Real radius);

  private:
    Vector<Real, R>   m_origin;
    Real              m_radius;
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using Circle2 = Hypersphere<Real, 2>;

  // The area enclosed by the circle
  template<typename Real>
  using Disk2 = Hypersphere<Real, 2>;

  template<typename Real>
  using Sphere = Hypersphere<Real, 3>;

  // The volume enclosed by the sphere
  template<typename Real>
  using Ball = Hypersphere<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  Hypersphere<Real, R>::Hypersphere()
    : m_origin(Zeros<Real, R>())
    , m_radius(static_cast<Real>(1))
  {
  }

  template<typename Real, int R>
  Hypersphere<Real, R>::Hypersphere(Vector<Real, R> const &a_center, Real a_radius)
  {
    Set(a_center, a_radius);
  }

  template<typename Real, int R>
  Hypersphere<Real, R>::Hypersphere(Hypersphere const &a_other)
    : m_origin(a_other.m_origin)
    , m_radius(a_other.m_radius)
  {
  }

  template<typename Real, int R>
  Hypersphere<Real, R> & Hypersphere<Real, R>::operator=(Hypersphere<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_radius = a_other.m_radius;
  }

  template<typename Real, int R>
  void Hypersphere<Real, R>::Set(Vector<Real, R> const & a_center, Real a_radius)
  {
    m_origin = a_center;
    m_radius = a_radius;
  }

  template<typename Real, int R>
  void Hypersphere<Real, R>::SetCenter(Vector<Real, R> const & a_center)
  {
    m_origin = a_center;
  }

  template<typename Real, int R>
  void Hypersphere<Real, R>::SetRadius(Real a_radius)
  {
    m_radius = (a_radius < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_radius;
  }
}

#endif