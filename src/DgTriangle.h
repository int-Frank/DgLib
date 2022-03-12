//@group Math/Geometry

#ifndef DGTRIANGLE_H
#define DGTRIANGLE_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real, int R>
  class Triangle
  {
  public:

    Triangle();
    Triangle(Vector<Real, R> const & p0, Vector<Real, R> const & p1, Vector<Real, R> const & p2);
    Triangle(Triangle const &);
    Triangle& operator=(Triangle const &);

    void Set(Vector<Real, R> const & p0, Vector<Real, R> const & p1, Vector<Real, R> const & p2);

    Vector<Real, R> const & P0() const { return m_points[0]; }
    Vector<Real, R> const & P1() const { return m_points[1]; }
    Vector<Real, R> const & P2() const { return m_points[2]; }

    Vector<Real, R> Centroid() const;

  private:

    //Data members
    Vector<Real, R> m_points[3];
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using Triangle2 = Triangle<Real, 2>;

  template<typename Real>
  using Triangle3 = Triangle<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  Triangle<Real, R>::Triangle()
    : m_points{XAxis<Real, 3>(), YAxis<Real, 3>(), ZAxis<Real, 3>()}
  {
  }

  template<typename Real, int R>
  Triangle<Real, R>::Triangle(Vector<Real, R> const & p0, 
                              Vector<Real, R> const & p1,
                              Vector<Real, R> const & p2)
    : m_points{ p0, p1, p2 }
  {
  }

  template<typename Real, int R>
  Triangle<Real, R>::Triangle(Triangle<Real, R> const & a_other)
    : m_points{ a_other.m_points[0], a_other.m_points[1], a_other.m_points[2]}
  {
  }

  template<typename Real, int R>
  Triangle<Real, R> & Triangle<Real, R>::operator=(Triangle<Real, R> const & a_other)
  {
    for (int i = 0; i < R; i++)
      m_points[i] = a_other.m_points[i];

    return *this;
  }

  template<typename Real, int R>
  void Triangle<Real, R>::Set(Vector<Real, R> const & a_p0, 
                              Vector<Real, R> const & a_p1,
                              Vector<Real, R> const & a_p2)
  {
    m_points[0] = a_p0;
    m_points[1] = a_p1;
    m_points[2] = a_p2;
  }

  template<typename Real, int R>
  Vector<Real, R> Triangle<Real, R>::Centroid() const
  {
    return (m_points[0] + m_points[1] + m_points[2]) / static_cast<Real>(3);
  }
}
#endif