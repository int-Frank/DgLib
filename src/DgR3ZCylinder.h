//! @file DgR3AACylinder.h
//!
//! @author: Frank B. Hart
//! @date 22/04/2019
//!
//! Class declaration: ZCylinder

#ifndef DGR3ZCYLINDER_H
#define DGR3ZCYLINDER_H

#include "DgR3Vector.h"
#include "DgR2Disk.h"

//TODO Queries
namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class ZCylinder
    //!
    //! @author: Frank Hart
    //! @date 22/04/2019
    template<typename Real>
    class ZCylinder
    {
    private:

    public:
      //! Default constructor.
      ZCylinder()
        : m_bottomCentre(Vector<Real>::Origin())
        , m_height(Real(1))
        , m_radius(Real(1))
      {}

      ZCylinder(Vector<Real> const & a_bottom, Real a_radius, Real a_height)
        : m_bottomCentre(a_bottom)
        , m_height(a_height)
        , m_radius(a_radius)
      {
        m_bottomCentre[3] = static_cast<Real>(1);
      }

      ~ZCylinder()
      {}

      //! Copy constructor
      ZCylinder(ZCylinder const & a_other)
        : m_bottomCentre(a_other.m_bottomCentre)
        , m_height(a_other.m_height)
        , m_radius(a_other.m_radius)
      {}

      //! Assignment
      ZCylinder& operator= (ZCylinder const & a_other)
      {
        m_bottomCentre = a_other.m_bottomCentre;
        m_height = a_other.m_height;
        m_radius = a_other.m_radius;

        return *this;
      }

      void SetBottom(Vector<Real> const & a_bottom)
      {
        m_bottomCentre = a_bottom;
        m_bottomCentre[3] = static_cast<Real>(1);
      }

      void SetRadius(Real a_radius)
      {
        m_radius = a_radius;
      }

      void SetHeight(Real a_height)
      {
        m_height = a_height;
      }

      R2::Disk<Real> GetDisk2D() const
      {
        return R2::Disk<Real>(R2::Vector<Real>(m_bottomCentre[0], m_bottomCentre[1], static_cast<Real>(1)), m_radius);
      }

      Real GetHeight() const {return m_height;}
      Real GetRadius() const {return m_radius;}
      Vector<Real> const & GetBottomCenter() const {return m_bottomCentre;}

    private:
      Vector<Real>  m_bottomCentre;
      Real          m_height;
      Real          m_radius;
    };
  }
}

#endif