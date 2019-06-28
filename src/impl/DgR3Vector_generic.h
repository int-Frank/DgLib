//! @file DgR3Vector_generic.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector_generic

#ifndef DGR3VECTOR_GENERIC_H
#define DGR3VECTOR_GENERIC_H

#include "DgVector_generic.h"
#include "../DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Vector_generic
    //!
    //! @brief Three dimensional homogeneous vector class [x, y, z, w].
    //!
    //! @author Frank Hart
    //! @date 4/10/2015
    template<typename Real>
    class Vector_generic<Real, 3> : public Dg::Matrix<1, 4, Real>
    {
    public:

      //! Default constructor. Members not initialized.
      Vector_generic() {}

      //! Construct vector from coefficients
      Vector_generic(Real x, Real y, Real z, Real w);
      ~Vector_generic() {}

      //! Copy constructor
      Vector_generic(Dg::Matrix<1, 4, Real> const & a_other) : Dg::Matrix<1, 4, Real>(a_other) {}

      //! Assignment
      Vector_generic& operator=(Dg::Matrix<1, 4, Real> const &);

      //! Determines if the vector is the unit vector within some tolerance.
      bool IsUnit() const;

      //! Set elements
      void Set(Real x, Real y, Real z, Real w);

      //! Make unit vector
      void Normalize();

      //! Vector_generic length
      Real Length() const;

      //! Squared length
      Real LengthSquared() const;

      Vector_generic Cross(Vector_generic const &) const;

      Real Distance(Vector_generic const &) const;

      Real SquaredDistance(Vector_generic const &) const;

      Vector_generic Perpendicular() const;

      //! Access element-x by value
      Real x() const { return m_V[0]; }

      //! Access element-y by value
      Real y() const { return m_V[1]; }

      //! Access element-z by value
      Real z() const { return m_V[2]; }

      //! Access element-w by value
      Real w() const { return m_V[3]; }

      //! Access element-x by reference
      Real & x() { return m_V[0]; }

      //! Access element-y by reference
      Real & y() { return m_V[1]; }

      //! Access element-z by reference
      Real & z() { return m_V[2]; }

      //! Access element-w by reference
      Real & w() { return m_V[3]; }

    public:

      //! v = [0, 0, 0, 1]
      static Vector_generic Origin();

      //! v = [0, 0, 0, 0]
      static Vector_generic ZeroVector();

      //! v = [1, 0, 0, 0]
      static Vector_generic xAxis();

      //! v = [0, 1, 0, 0]
      static Vector_generic yAxis();

      //! v = [0, 0, 1, 0]
      static Vector_generic zAxis();

      //! v = [1, 1, 1, 1]
      static Vector_generic Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::Origin()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(1.0));
    }   // End:  Vector_generic::Origin()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::ZeroVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::ZeroVector()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::ZeroVector()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::xAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::xAxis()
    {
      return Vector_generic(static_cast<Real>(1.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::xAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::yAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::yAxis()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::yAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::zAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::zAxis()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::zAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Ones()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::Ones()
    {
      return Vector_generic(static_cast<Real>(1.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(1.0));
    }   // End:  Vector_generic::Ones()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::operator=()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3>& Vector_generic<Real, 3>::operator=(Dg::Matrix<1, 4, Real> const & a_other)
    {
      Matrix<1, 4, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector_generic::operator=()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Vector_generic()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 3>::Vector_generic(Real a_x, Real a_y, Real a_z, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_z;
      m_V[3] = a_w;
    }   // End:  Vector_generic::Vector_generic()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Set()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector_generic<Real, 3>::Set(Real a_x, Real a_y, Real a_z, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_z;
      m_V[3] = a_w;
    }   // End: Vector_generic::Set()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Length()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector_generic<Real, 3>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
                  m_V[1] * m_V[1] +
                  m_V[2] * m_V[2] +
                  m_V[3] * m_V[3]);
    }   // End:  Vector_generic::Length()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::LengthSquared()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector_generic<Real, 3>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
              m_V[1] * m_V[1] +
              m_V[2] * m_V[2] +
              m_V[3] * m_V[3]);
    }   // End:  Vector_generic::LengthSquared()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::IsUnit()
    //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector_generic<Real, 3>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());
    }   // End:  Vector_generic::IsUnit()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Normalize()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector_generic<Real, 3>::Normalize()
    {
      Real lengthsq = LengthSquared();

      if (Dg::IsZero(lengthsq))
      {
        m_V[0] = static_cast<Real>(1.0);
        m_V[1] = static_cast<Real>(0.0);
        m_V[2] = static_cast<Real>(0.0);
        m_V[3] = static_cast<Real>(0.0);
      }
      else
      {
        Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
        m_V[0] *= factor;
        m_V[1] *= factor;
        m_V[2] *= factor;
        m_V[3] *= factor;
      }
    }   // End:  Vector_generic::Normalize()


    //! Cross product
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::Cross(Vector_generic<Real, 3> const & a_v) const
    {
      Vector_generic<Real, 3> result;
      result[0] = m_V[1] * a_v[2] - m_V[2] * a_v[1];
      result[1] = m_V[2] * a_v[0] - m_V[0] * a_v[2];
      result[2] = m_V[0] * a_v[1] - m_V[1] * a_v[0];
      result[3] = static_cast<Real>(0.0);

      return result;
    }	//End: Cross()


    template<typename Real>
    Real Vector_generic<Real, 3>::SquaredDistance(Vector_generic<Real, 3> const & a_v) const
    {
      Real x = m_V[0] - a_v.m_V[0];
      Real y = m_V[1] - a_v.m_V[1];
      Real z = m_V[2] - a_v.m_V[2];
      return (x*x + y*y + z*z);
    }


    template<typename Real>
    Real Vector_generic<Real, 3>::Distance(Vector_generic<Real, 3> const & a_v) const
    {
      return sqrt(SquaredDistance(a_v));
    }


    //! Returns a perpendicular vector.
    template<typename Real>
    Vector_generic<Real, 3> Vector_generic<Real, 3>::Perpendicular() const
    {
      Vector_generic<Real, 3> result;

      if (!Dg::IsZero(m_V[0]) || !Dg::IsZero(m_V[1]))
      {
        result[0] = -m_V[1];
        result[1] = m_V[0];
        result[2] = static_cast<Real>(0.0);
        result[3] = static_cast<Real>(0.0);
      }
      else
      {
        result[0] = -m_V[2];
        result[1] = static_cast<Real>(0.0);
        result[2] = m_V[0];
        result[3] = static_cast<Real>(0.0);
      }

      result.Normalize();
      return result;
    }	//End: Perpendicular()
  }
}


#endif