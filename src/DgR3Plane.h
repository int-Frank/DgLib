//! @file DgR3Plane.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Plane

#ifndef DGR3PLANE_H
#define DGR3PLANE_H

#include "DgR3Vector.h"
#include "dgmath.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class Plane
    //!
    //! A Plane is infinite flat surface. It is implemented by using the generalized
    //! plane equation: Ax + By + Cz + D = 0, or a vector normal to the plane
    //! and an offset. The plane normal will always be a unit vector.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real>
    class Plane
    {
    public:
      //! Default constructor.
      Plane() : m_normal(Vector<Real>::xAxis()),
        m_offset(static_cast<Real>(0.0)) {}

      //! Construct plane form coefficients of the equation:
      //!     Ax + By + Cz + D = 0
      //!
      //! @param[in] A x-coefficient
      //! @param[in] B y-coefficient
      //! @param[in] C z-coefficient
      //! @param[in] D constant 
      Plane(Real A, Real B, Real C, Real D);

      //! Construct plane from three points that lie on the plane.
      Plane(Vector<Real> const & p0, Vector<Real> const & p1, Vector<Real> const & p2);

      //! Construct plane from normal vector and offset.
      Plane(Vector<Real> const & n, Real d);

      //! Construct plane from a normal vector and a point which lies on the plane.
      Plane(Vector<Real> const & n, Vector<Real> const & p);
      ~Plane() {}

      //! Copy constructor
      Plane(Plane const &);

      //! Assignment
      Plane& operator= (Plane const &);

      //! Get the plane normal.
      Vector<Real> const & Normal() const { return m_normal; }

      //! Get the plane offset.
      Real Offset() const { return m_offset; }

      //! Get the plane normal and offset.
      void Get(Vector<Real>& normal, Real& offset) const;

      //! Are two planes equal?
      bool operator== (Plane const &) const;

      //! Are two planes not equal?
      bool operator!= (Plane const &) const;

      //! Set the plane form coefficients of the equation:
      //!     Ax + By + Cz + D = 0
      //!
      //! @param[in] A x-coefficient
      //! @param[in] B y-coefficient
      //! @param[in] C z-coefficient
      //! @param[in] D constant 
      void Set(Real A, Real B, Real C, Real D);

      //! Set the plane from normal vector and offset.
      void Set(Vector<Real> const & n, Real d);

      //! Set the plane from a normal vector and a point which lies on the plane.
      void Set(Vector<Real> const & n, Vector<Real> const & p);

      //! Set the plane from three points that lie on the plane.
      void Set(Vector<Real> const & p0, Vector<Real> const & p1, Vector<Real> const & p2);

      //! Signed distance from point to plane.
      //! @return Negative if behind the plane.
      //!
      //! @param[in] a_point Input point
      Real SignedDistance(Vector<Real> const & a_point) const;

      //! Distance from point to plane.
      Real Distance(Vector<Real> const & a_point) const;

      //! Dot procuct of a vector against the plane normal.
      Real NormalDot(Vector<Real> const & a_v) const;

    private:
      Vector<Real>   m_normal;
      Real            m_offset;
    };

    //--------------------------------------------------------------------------------
    //		@ Plane::SignedDistance()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Real Plane<Real>::SignedDistance(Vector<Real> const & a_point) const
    {
      return a_point.Dot(m_normal) + m_offset;

    }	//End: Plane::SignedDistance()


      //--------------------------------------------------------------------------------
      //		@ Plane::Distance()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Real Plane<Real>::Distance(Vector<Real> const & a_point) const
    {
      return abs(a_point.Dot(m_normal) + m_offset);

    }	//End: Plane::Distance()


      //--------------------------------------------------------------------------------
      //		@ Plane::NormalDot()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Real Plane<Real>::NormalDot(Vector<Real> const & a_v) const
    {
      return m_normal.Dot(a_v);

    }	//End: Plane::NormalDot()


      //--------------------------------------------------------------------------------
      //		@ Plane::Plane()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>::Plane(Real a, Real b, Real c, Real d)
    {
      Set(a, b, c, d);

    }	//End: Plane::Plane()


      //--------------------------------------------------------------------------------
      //		@ Plane::Plane()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>::Plane(Vector<Real> const & n, Real d)
    {
      Set(n.x(), n.y(), n.z(), d);

    }	//End: Plane::Plane()


      //--------------------------------------------------------------------------------
      //		@ Plane::Plane()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>::Plane(Vector<Real> const & p0,
      Vector<Real> const & p1,
      Vector<Real> const & p2)
    {
      Set(p0, p1, p2);

    }	//End: Plane::Plane()


      //--------------------------------------------------------------------------------
      //	@	Plane::Plane<Real>()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>::Plane(Vector<Real> const & n, Vector<Real> const & p)
    {
      Set(n, p);
    }	//End: Plane4::Plane<Real>()


      //--------------------------------------------------------------------------------
      //		@ Plane::Plane()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>::Plane(Plane<Real> const & a_other) :
      m_normal(a_other.m_normal), m_offset(a_other.m_offset)
    {
    }	//End: Plane::Plane()


      //--------------------------------------------------------------------------------
      //		@ Plane::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Plane<Real>& Plane<Real>::operator=(Plane<Real> const & a_other)
    {
      m_normal = a_other.m_normal;
      m_offset = a_other.m_offset;

      return *this;
    }	//End: Plane::operator=()


      //--------------------------------------------------------------------------------
      //		@ Plane::Get()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Plane<Real>::Get(Vector<Real> & normal, Real & offset) const
    {
      normal = m_normal;
      offset = m_offset;
    }	//End: Plane::Get()


      //--------------------------------------------------------------------------------
      //		@ Plane::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Plane<Real>::operator==(Plane<Real> const & a_other) const
    {
      return (m_normal == a_other.m_normal && m_offset == a_other.m_offset);
    }	//End: Plane::operator==()


      //--------------------------------------------------------------------------------
      //		@ Plane::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Plane<Real>::operator!=(Plane<Real> const & a_other) const
    {
      return (m_normal != a_other.m_normal || m_offset != a_other.m_offset);
    }	//End: Plane::operator!=()


      //--------------------------------------------------------------------------------
      //	@	Plane::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Plane<Real>::Set(Real a, Real b, Real c, Real d)
    {
      //normalise for cheap distance checks
      Real lensq = a*a + b*b + c*c;

      //recover gracefully
      if (Dg::IsZero(lensq))
      {
        m_normal = Vector<Real>::xAxis();
        m_offset = static_cast<Real>(0.0);
      }
      else
      {
        Real recip = static_cast<Real>(1.0) / sqrt(lensq);
        m_normal.Set(a*recip, b*recip, c*recip, static_cast<Real>(0.0));
        m_offset = d*recip;
      }

    }	//End: Plane4::Set()


      //--------------------------------------------------------------------------------
      //	@	Plane::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Plane<Real>::Set(Vector<Real> const & n, Real d)
    {
      //normalise for cheap distance checks
      Real lensq = n.x()*n.x() + n.y()*n.y() + n.z()*n.z();

      //recover gracefully
      if (Dg::IsZero(lensq))
      {
        m_normal = Vector<Real>::xAxis();
        m_offset = static_cast<Real>(0.0);
      }
      else
      {
        Real recip = static_cast<Real>(1.0) / sqrt(lensq);
        m_normal.Set(n.x()*recip, n.y()*recip, n.z()*recip, static_cast<Real>(0.0));
        m_offset = d*recip;
      }

    }	//End: Plane4::Set()


      //--------------------------------------------------------------------------------
      //	@	Plane::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Plane<Real>::Set(Vector<Real> const & n, Vector<Real> const & p)
    {
      //normalise for cheap distance checks
      Real lensq = n.x()*n.x() + n.y()*n.y() + n.z()*n.z();

      //recover gracefully
      if (Dg::IsZero(lensq))
      {
        m_normal = Vector<Real>::xAxis();
        m_offset = static_cast<Real>(0.0);
      }
      else
      {
        Real recip = static_cast<Real>(1.0) / sqrt(lensq);
        m_normal.Set(n.x()*recip, n.y()*recip, n.z()*recip, static_cast<Real>(0.0));
        m_offset = -(n.x()*p.x() + n.y()*p.y() + n.z()*p.z())*recip;
      }

    }	//End: Plane4::Set()


      //--------------------------------------------------------------------------------
      //	@	Plane::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Plane<Real>::Set(Vector<Real> const & p0,
      Vector<Real> const & p1,
      Vector<Real> const & p2)
    {
      //Get plane vector
      Vector<Real> u = p1 - p0;
      Vector<Real> v = p2 - p0;
      Vector<Real> w = u.Cross(v);

      //normalise for cheap distance checks
      Real lensq = w.x()*w.x() + w.y()*w.y() + w.z()*w.z();

      //recover gracefully
      if (Dg::IsZero(lensq))
      {
        m_normal = Vector<Real>::xAxis();
        m_offset = static_cast<Real>(0.0);
      }
      else
      {
        Real recip = static_cast<Real>(1.0) / sqrt(lensq);
        m_normal.Set(w.x()*recip, w.y()*recip, w.z()*recip, static_cast<Real>(0.0));
        m_offset = -p0.Dot(m_normal);
      }

    }	//End: Plane4::Set()
  }
}

#endif