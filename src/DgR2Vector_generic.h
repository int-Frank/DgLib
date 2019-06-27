#ifndef DGR2VECTOR_IMPL_H
#define DGR2VECTOR_IMPL_H

#include "DgVector_generic.h"
#include "DgR3Vector_generic.h"
#include "../DgMatrix.h"

namespace Dg
{
  namespace impl
  {
    template<typename Real>
    class Vector_generic<Real, 2> : public Matrix<1, 3, Real>
    {
    public:

      //! Default constructor. Members not initialized.
      Vector_generic() {}

      //! Construct vector from coefficients
      Vector_generic(Real x, Real y, Real w);
      Vector_generic(Vector_generic<Real, 3> const &);
      ~Vector_generic() {}

      //! Copy constructor
      Vector_generic(Matrix<1, 3, Real> const & a_other) : Matrix<1, 3, Real>(a_other) {}

      //! Assignment
      Vector_generic& operator=(Matrix<1, 3, Real> const &);

      //! Determines if the vector is the unit vector within some tolerance.
      bool IsUnit() const;

      //! Set elements
      void Set(Real x, Real y, Real w);

      //! Make unit vector
      void Normalize();

      //! Vector_generic length
      Real Length() const;

      //! Squared length
      Real LengthSquared() const;

      Real PerpDot(Vector_generic const &) const;

      Real Distance(Vector_generic const &) const;

      Real SquaredDistance(Vector_generic const &) const;

      Vector_generic Perpendicular() const;

      //! Access element-x by value
      Real x() const { return m_V[0]; }

      //! Access element-y by value
      Real y() const { return m_V[1]; }

      //! Access element-w by value
      Real w() const { return m_V[2]; }

      //! Access element-x by reference
      Real & x() { return m_V[0]; }

      //! Access element-y by reference
      Real & y() { return m_V[1]; }

      //! Access element-w by reference
      Real & w() { return m_V[2]; }

    public:

      //! v = [0, 0, 1]
      static Vector_generic Origin();

      //! v = [0, 0, 0]
      static Vector_generic ZeroVector();

      //! v = [1, 0, 0]
      static Vector_generic xAxis();

      //! v = [0, 1, 0]
      static Vector_generic yAxis();

      //! v = [1, 1, 1]
      static Vector_generic Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::Origin()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(1.0));
    }   // End:  Vector_generic::Origin()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::ZeroVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::ZeroVector()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::ZeroVector()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::xAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::xAxis()
    {
      return Vector_generic(static_cast<Real>(1.0),
                            static_cast<Real>(0.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::xAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::yAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::yAxis()
    {
      return Vector_generic(static_cast<Real>(0.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(0.0));
    }   // End:  Vector_generic::yAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Ones()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::Ones()
    {
      return Vector_generic(static_cast<Real>(1.0),
                            static_cast<Real>(1.0),
                            static_cast<Real>(1.0));
    }   // End:  Vector_generic::Ones()


    //-------------------------------------------------------------------------------
    //  @	Vector_generic::operator=()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2>& Vector_generic<Real, 2>::operator=(Dg::Matrix<1, 3, Real> const & a_other)
    {
      Matrix<1, 3, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector_generic::operator=()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Vector_generic()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2>::Vector_generic(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;
    }   // End:  Vector_generic::Vector_generic()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Vector_generic()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector_generic<Real, 2>::Vector_generic(Vector_generic<Real, 3> const & a_v)
    {
      m_V[0] = a_v.x();
      m_V[1] = a_v.y();
      m_V[2] = a_v.w();
    }   // End:  Vector_generic::Vector_generic()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Set()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector_generic<Real, 2>::Set(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;
    }   // End: Vector_generic::Set()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Length()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector_generic<Real, 2>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
                  m_V[1] * m_V[1] +
                  m_V[2] * m_V[2]);
    }   // End:  Vector_generic::Length()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::LengthSquared()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector_generic<Real, 2>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
              m_V[1] * m_V[1] +
              m_V[2] * m_V[2]);
    }   // End:  Vector_generic::LengthSquared()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::IsUnit()
    //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector_generic<Real, 2>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());
    }   // End:  Vector_generic::IsUnit()


    //-------------------------------------------------------------------------------
    //	@	Vector_generic::Normalize()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector_generic<Real, 2>::Normalize()
    {
      Real lengthsq = LengthSquared();

      if (Dg::IsZero(lengthsq))
      {
        m_V[0] = static_cast<Real>(1.0);
        m_V[1] = static_cast<Real>(0.0);
        m_V[2] = static_cast<Real>(0.0);
      }
      else
      {
        Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
        m_V[0] *= factor;
        m_V[1] *= factor;
        m_V[2] *= factor;
      }
    }   // End:  Vector_generic::Normalize()


    //! Perpendicular dot product, assumes 2D vector
    template<typename Real>
    Real Vector_generic<Real, 2>::PerpDot(Vector_generic<Real, 2> const & a_v) const
    {
      return m_V[0] * a_v[1] - m_V[1] * a_v[0];
    }	//End: PerpDot()


    template<typename Real>
    Real Vector_generic<Real, 2>::SquaredDistance(Vector_generic<Real, 2> const & a_v) const
    {
      Real x = m_V[0] - a_v.m_V[0];
      Real y = m_V[1] - a_v.m_V[1];
      return (x*x + y*y);
    }


    template<typename Real>
    Real Vector_generic<Real, 2>::Distance(Vector_generic<Real, 2> const & a_v) const
    {
      return sqrt(SquaredDistance(a_v));
    }


    //! Returns a perpendicular vector.
    template<typename Real>
    Vector_generic<Real, 2> Vector_generic<Real, 2>::Perpendicular() const
    {
      return Vector_generic<Real, 2>(-m_V[1],
                                      m_V[0],
                                      static_cast<Real>(0.0));
    }	//End: Perpendicular()
  }
}

#endif