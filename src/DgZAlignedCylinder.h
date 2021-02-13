//@group Math/Geometry

#ifndef DGZALIGNEDCYLINDER_H
#define DGZALIGNEDCYLINDER_H

#include "DgVector.h"
#include "DgHypersphere.h"

namespace Dg
{
  template<typename Real>
  class ZAlignedCylinder
  {
  private:

  public:

    ZAlignedCylinder()
      : m_bottomCentre(Vector3<Real>::Origin())
      , m_height(Real(1))
      , m_radius(Real(1))
    {}

    ZAlignedCylinder(Vector3<Real> const & a_bottom, Real a_radius, Real a_height)
      : m_bottomCentre(a_bottom)
      , m_height(a_height)
      , m_radius(a_radius)
    {

    }

    ~ZAlignedCylinder()
    {}

    ZAlignedCylinder(ZAlignedCylinder const & a_other)
      : m_bottomCentre(a_other.m_bottomCentre)
      , m_height(a_other.m_height)
      , m_radius(a_other.m_radius)
    {}

    ZAlignedCylinder& operator= (ZAlignedCylinder const & a_other)
    {
      m_bottomCentre = a_other.m_bottomCentre;
      m_height = a_other.m_height;
      m_radius = a_other.m_radius;

      return *this;
    }

    void SetBottom(Vector3<Real> const & a_bottom)
    {
      m_bottomCentre = a_bottom;
    }

    void SetRadius(Real a_radius)
    {
      m_radius = a_radius;
    }

    void SetHeight(Real a_height)
    {
      m_height = a_height;
    }

    Disk2<Real> GetDisk2D() const
    {
      return Disk2<Real>(Vector3<Real>(m_bottomCentre[0], m_bottomCentre[1]), m_radius);
    }

    Real GetHeight() const {return m_height;}
    Real GetRadius() const {return m_radius;}
    Vector3<Real> const & GetBottomCenter() const {return m_bottomCentre;}

  private:
    Vector3<Real>  m_bottomCentre;
    Real          m_height;
    Real          m_radius;
  };
}

#endif