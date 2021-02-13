//@group Math/Geometry

#ifndef DGRECTANGLE_H
#define DGRECTANGLE_H

#include "DgVector.h"

namespace Dg
{
  template<typename Real>
  class Rectangle3
  {
  private:

  public:
    //! Default constructor.
    Rectangle3()
      : m_center(Zeros<Real, 3>())
      , m_axis{XAxis<Real, 3>(), YAxis<Real, 3>()}
      , m_extents{static_cast<Real>(1), static_cast<Real>(1)}
    {}

    Rectangle3(Vector3<Real> const & a_center,
              Vector3<Real> a_axis[2],
              Real a_extents[2])
      : m_ceneter(a_center)
      , m_axis({a_axis[0], a_axis[1]})
      , m_extents({a_extents[0], a_extents[1]})
    {}

    //! Construct Rectangle3 from origin and radius
    ~Rectangle3() {}

    //! Copy constructor
    Rectangle3(Rectangle3 const & a_other)
      : m_ceneter(a_other.m_center)
      , m_axis({a_other.m_axis[0], a_other.m_axis[1]})
      , m_extents({a_other.m_extents[0], a_other.m_extents[1]})
    {}

    //! Assignment
    Rectangle3& operator= (Rectangle3 const & a_other)
    {
      m_center = a_other.m_center;
      m_axis[0] = a_other.m_axis[0];
      m_axis[1] = a_other.m_axis[1];
      m_extents[0] = a_other.m_extents[0];
      m_extents[1] = a_other.m_extents[1];

      return *this;
    }

    Vector3<Real> const & GetCenter() const {return m_center;}
    Vector3<Real> const & GetAxis(int a_index) const {return m_axis[a_index];}
    Real GetExtent(int a_index) const {return m_extents[a_index];}

    void SetCenter(Vector3<Real> const & a_center){m_center = a_center;}
    void SetAxis(int a_index, Vector3<Real> const & a_val) {m_axis[a_index] = a_val;}
    void SetExtent(int a_index, Real a_val) {m_extents[a_index] = a_val;}

    void GetVertices(Vector3<Real> & a_v0,
                     Vector3<Real> & a_v1,
                     Vector3<Real> & a_v2,
                     Vector3<Real> & a_v3) const
    {
      Vector3<Real> axis0 = m_axis[0] * m_extents[0];
      Vector3<Real> axis1 = m_axis[1] * m_extents[1];
      Vector3<Real> diff = axis0 - axis1;
      Vector3<Real> sum = axis0 + axis1;

      a_v0 = m_center - diff;
      a_v1 = m_center - sum;
      a_v2 = m_center + sum;
      a_v3 = m_center + diff;
    }

  private:
    Vector3<Real> m_center;
    Vector3<Real> m_axis[2];
    Real          m_extents[2];

  };
}

#endif