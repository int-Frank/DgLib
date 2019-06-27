//! @file DgR3Rectangle.h
//!
//! @author: Frank B. Hart
//! @date 22/04/2019
//!
//! Class declaration: Rectangle

#ifndef DGR3RECTANGLE_H
#define DGR3RECTANGLE_H

#include "DgR3Vector.h"

//TODO Queries
namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class Rectangle
    //!
    //! @author: Frank Hart
    //! @date 22/04/2019
    template<typename Real>
    class Rectangle
    {
    private:

    public:
      //! Default constructor.
      Rectangle()
        : m_center(Vector<Real>::Origin())
        , m_axis{Vector<Real>::xAxis(), Vector<Real>::yAxis()}
        , m_extents{Real(1), Real(1)}
      {}

      Rectangle(Vector<Real> const & a_center,
                Vector<Real> a_axis[2],
                Real a_extents[2])
        : m_ceneter(a_center)
        , m_axis({a_axis[0], a_axis[1]})
        , m_extents({a_extents[0], a_extents[1]})
      {}

      //! Construct Rectangle from origin and radius
      ~Rectangle() {}

      //! Copy constructor
      Rectangle(Rectangle const & a_other)
        : m_ceneter(a_other.m_center)
        , m_axis({a_other.m_axis[0], a_other.m_axis[1]})
        , m_extents({a_other.m_extents[0], a_other.m_extents[1]})
      {}

      //! Assignment
      Rectangle& operator= (Rectangle const & a_other)
      {
        m_center = a_other.m_center;
        m_axis[0] = a_other.m_axis[0];
        m_axis[1] = a_other.m_axis[1];
        m_extents[0] = a_other.m_extents[0];
        m_extents[1] = a_other.m_extents[1];

        return *this;
      }

      Vector<Real> const & GetCenter() const {return m_center;}
      Vector<Real> const & GetAxis(int a_index) const {return m_axis[a_index];}
      Real GetExtent(int a_index) const {return m_extents[a_index];}

      void SetCenter(Vector<Real> const & a_center){m_center = a_center;}
      void SetAxis(int a_index, Vector<Real> const & a_val) {m_axis[a_index] = a_val;}
      void SetExtent(int a_index, Real a_val) {m_extents[a_index] = a_val;}

      void GetVertices(Vector<Real> & a_v0,
                       Vector<Real> & a_v1,
                       Vector<Real> & a_v2,
                       Vector<Real> & a_v3) const
      {
        Vector<Real> axis0 = m_axis[0] * m_extents[0];
        Vector<Real> axis1 = m_axis[1] * m_extents[1];
        Vector<Real> diff = axis0 - axis1;
        Vector<Real> sum = axis0 + axis1;

        a_v0 = m_center - diff;
        a_v1 = m_center - sum;
        a_v2 = m_center + sum;
        a_v3 = m_center + diff;
      }

    private:
      Vector<Real>  m_center;
      Vector<Real>  m_axis[2];
      Real          m_extents[2];

    };
  }
}

#endif