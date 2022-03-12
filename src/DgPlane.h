//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGPLANE_H
#define DGPLANE_H

#include "DgVector.h"
#include "DgMath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Plane
  //!
  //! A Plane is infinite flat surface. It is implemented by using the generalized
  //! plane equation: Ax + By + Cz + D = 0, or a vector normal to the plane
  //! and an offset. The plane normal will always be a unit vector.
  //! 
  template<typename Real>
  class Plane
  {
  public:

    Plane();

    //! Construct plane form coefficients of the equation: Ax + By + Cz + D = 0
    Plane(Real A, Real B, Real C, Real D);

    //! Construct plane from three points that lie on the plane.
    Plane(Vector3<Real> const & p0, Vector3<Real> const & p1, Vector3<Real> const & p2);

    //! Construct plane from normal vector and offset.
    Plane(Vector3<Real> const & n, Real d);

    //! Construct plane from a normal vector and a point which lies on the plane.
    Plane(Vector3<Real> const & n, Vector3<Real> const & p);

    Plane(Plane const &);
    Plane& operator= (Plane const &);

    Vector3<Real> const & Normal() const { return m_normal; }
    Real Offset() const { return m_offset; }

    //! Set the plane form coefficients of the equation: Ax + By + Cz + D = 0
    void Set(Real A, Real B, Real C, Real D);

    //! Set the plane from normal vector and offset.
    void Set(Vector3<Real> const & n, Real d);

    //! Set the plane from a normal vector and a point which lies on the plane.
    void Set(Vector3<Real> const & n, Vector3<Real> const & p);

    //! Set the plane from three points that lie on the plane.
    void Set(Vector3<Real> const & p0, Vector3<Real> const & p1, Vector3<Real> const & p2);

    //! Signed distance from point to plane.
    //! @return Negative if behind the plane.
    Real SignedDistance(Vector3<Real> const & a_point) const;

    //! Distance from point to plane.
    Real Distance(Vector3<Real> const & a_point) const;

  private:
    Vector3<Real>   m_normal;
    Real            m_offset;
  };

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real>
  Plane<Real>::Plane()
    : m_normal{ static_cast<Real>(1), static_cast<Real>(0), static_cast<Real>(0) }
    , m_offset(static_cast<Real>(0)) 
  {
  }

  template<typename Real>
  Plane<Real>::Plane(Plane<Real> const &a_other)
    : m_normal(a_other.m_normal)
    , m_offset(a_other.m_offset)
  {
  }

  template<typename Real>
  Plane<Real>::Plane(Real a, Real b, Real c, Real d)
  {
    Set(a, b, c, d);
  }

  template<typename Real>
  Plane<Real>::Plane(Vector3<Real> const &n, Real d)
  {
    Set(n[0], n[1], n[2], d);
  }

  template<typename Real>
  Plane<Real>::Plane(Vector3<Real> const &p0,
    Vector3<Real> const &p1,
    Vector3<Real> const &p2)
  {
    Set(p0, p1, p2);
  }

  template<typename Real>
  Plane<Real>::Plane(Vector3<Real> const &n, Vector3<Real> const &p)
  {
    Set(n, p);
  }

  template<typename Real>
  Plane<Real> &Plane<Real>::operator=(Plane<Real> const &a_other)
  {
    m_normal = a_other.m_normal;
    m_offset = a_other.m_offset;

    return *this;
  }

  template<typename Real>
  Real Plane<Real>::SignedDistance(Vector3<Real> const & a_point) const
  {
    return Dot(a_point, m_normal) + m_offset;
  }

  template<typename Real>
  Real Plane<Real>::Distance(Vector3<Real> const & a_point) const
  {
    return abs(Dot(a_point, m_normal) + m_offset);
  }

  template<typename Real>
  void Plane<Real>::Set(Real a, Real b, Real c, Real d)
  {
    //normalise for cheap distance checks
    Real lensq = a*a + b*b + c*c;

    //recover gracefully
    if (Dg::IsZero(lensq))
    {
      m_normal = XAxis<Real, 3>();
      m_offset = static_cast<Real>(0);
    }
    else
    {
      Real recip = static_cast<Real>(1) / sqrt(lensq);
      m_normal = Vector3<Real>(a*recip, b*recip, c*recip);
      m_offset = d*recip;
    }
  }

  template<typename Real>
  void Plane<Real>::Set(Vector3<Real> const & n, Real d)
  {
    //normalise for cheap distance checks
    Real lensq = MagSq(n);

    //recover gracefully
    if (Dg::IsZero(lensq))
    {
      m_normal = XAxis<Real, 3>();
      m_offset = static_cast<Real>(0);
    }
    else
    {
      Real recip = static_cast<Real>(1) / sqrt(lensq);
      m_normal = n * recip;
      m_offset = d * recip;
    }
  }

  template<typename Real>
  void Plane<Real>::Set(Vector3<Real> const & n, Vector3<Real> const & p)
  {
    //normalise for cheap distance checks
    Real lensq = MagSq(n);

    //recover gracefully
    if (Dg::IsZero(lensq))
    {
      m_normal = XAxis<Real, 3>();
      m_offset = static_cast<Real>(0);
    }
    else
    {
      Real recip = static_cast<Real>(1) / sqrt(lensq);
      m_normal = n * recip;
      m_offset = -Dot(n, p) * recip;
    }
  }

  template<typename Real>
  void Plane<Real>::Set(Vector3<Real> const & p0, Vector3<Real> const & p1, Vector3<Real> const & p2)
  {
    //Get plane vector
    Vector3<Real> u = p1 - p0;
    Vector3<Real> v = p2 - p0;
    Vector3<Real> w = Cross(u, v);

    //normalise for cheap distance checks
    Real lensq = MagSq(w);

    //recover gracefully
    if (Dg::IsZero(lensq))
    {
      m_normal = XAxis<Real, 3>();
      m_offset = static_cast<Real>(0);
    }
    else
    {
      Real recip = static_cast<Real>(1) / sqrt(lensq);
      m_normal = w * recip;
      m_offset = -Dot(p0, m_normal);
    }
  }
}

#endif