//@group Math/impl

//Copyright (c) 2015, James M. Van Verth and Lars M. Bishop.
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//* Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//* Neither the name of Essential Mathematics for Games and Interactive
//Applications nor the names of its contributors may be used to endorse
//or promote products derived from this software without specific prior
//written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL JAMES M. VAN VERTH OR LARS M. BISHOP BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DGVECTOR2_GENERIC_H
#define DGVECTOR2_GENERIC_H

#include "DgR2Vector_generic.h"
#include "../DgMatrix.h"

namespace Dg
{
  namespace impl
  {
    template<typename Real>
    class Vector2_generic : public Matrix<1, 2, Real>
    {
    public:

      //! Default constructor. Members not initialized.
      Vector2_generic() {}

      //! Construct vector from coefficients
      Vector2_generic(Real x, Real y);
      ~Vector2_generic() {}

      //! Copy constructor
      Vector2_generic(Matrix<1, 2, Real> const & a_other) : Matrix<1, 2, Real>(a_other) {}

      //! Assignment
      Vector2_generic& operator=(Matrix<1, 2, Real> const &);

      //! Determines if the vector is the unit vector within some tolerance.
      bool IsUnit() const;

      //! Set elements
      void Set(Real x, Real y);

      //! Make unit vector
      void Normalize();

      //! Vector2_generic length
      Real Length() const;

      //! Squared length
      Real LengthSquared() const;

      Real PerpDot(Vector2_generic const &) const;

      Real Distance(Vector2_generic const &) const;

      Real SquaredDistance(Vector2_generic const &) const;

      Vector2_generic Perpendicular() const;

      //! Access element-x by value
      Real x() const { return m_V[0]; }

      //! Access element-y by value
      Real y() const { return m_V[1]; }

      //! Access element-x by reference
      Real & x() { return m_V[0]; }

      //! Access element-y by reference
      Real & y() { return m_V[1]; }

    public:

      //! v = [0, 0]
      static Vector2_generic Origin();

      //! v = [0, 0]
      static Vector2_generic ZeroVector();

      //! v = [1, 0]
      static Vector2_generic xAxis();

      //! v = [0, 1]
      static Vector2_generic yAxis();

      //! v = [1, 1]
      static Vector2_generic Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::Origin()
    {
      return Vector2_generic(static_cast<Real>(0.0),
                             static_cast<Real>(0.0));
    }   // End:  Vector2_generic::Origin()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::ZeroVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::ZeroVector()
    {
      return Vector2_generic(static_cast<Real>(0.0),
                             static_cast<Real>(0.0));
    }   // End:  Vector2_generic::ZeroVector()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::xAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::xAxis()
    {
      return Vector2_generic(static_cast<Real>(1.0),
                             static_cast<Real>(0.0));
    }   // End:  Vector2_generic::xAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::yAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::yAxis()
    {
      return Vector2_generic(static_cast<Real>(0.0),
                             static_cast<Real>(1.0));
    }   // End:  Vector2_generic::yAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Ones()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::Ones()
    {
      return Vector2_generic(static_cast<Real>(1.0),
                             static_cast<Real>(1.0));
    }   // End:  Vector2_generic::Ones()


    //-------------------------------------------------------------------------------
    //  @	Vector2_generic::operator=()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real>& Vector2_generic<Real>::operator=(Dg::Matrix<1, 2, Real> const & a_other)
    {
      Matrix<1, 2, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector2_generic::operator=()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Vector2_generic()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector2_generic<Real>::Vector2_generic(Real a_x, Real a_y)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
    }   // End:  Vector2_generic::Vector2_generic()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Set()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector2_generic<Real>::Set(Real a_x, Real a_y)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
    }   // End: Vector2_generic::Set()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Length()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector2_generic<Real>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
                  m_V[1] * m_V[1]);
    }   // End:  Vector2_generic::Length()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::LengthSquared()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector2_generic<Real>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
              m_V[1] * m_V[1]);
    }   // End:  Vector2_generic::LengthSquared()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::IsUnit()
    //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector2_generic<Real>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());
    }   // End:  Vector2_generic::IsUnit()


    //-------------------------------------------------------------------------------
    //	@	Vector2_generic::Normalize()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector2_generic<Real>::Normalize()
    {
      Real lengthsq = LengthSquared();

      if (Dg::IsZero(lengthsq))
      {
        m_V[0] = static_cast<Real>(1.0);
        m_V[1] = static_cast<Real>(0.0);
      }
      else
      {
        Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
        m_V[0] *= factor;
        m_V[1] *= factor;
      }
    }   // End:  Vector2_generic::Normalize()


    //! Perpendicular dot product, assumes 2D vector
    template<typename Real>
    Real Vector2_generic<Real>::PerpDot(Vector2_generic<Real> const & a_v) const
    {
      return m_V[0] * a_v[1] - m_V[1] * a_v[0];
    }	//End: PerpDot()


    template<typename Real>
    Real Vector2_generic<Real>::SquaredDistance(Vector2_generic<Real> const & a_v) const
    {
      Real x = m_V[0] - a_v.m_V[0];
      Real y = m_V[1] - a_v.m_V[1];
      return (x*x + y*y);
    }


    template<typename Real>
    Real Vector2_generic<Real>::Distance(Vector2_generic<Real> const & a_v) const
    {
      return sqrt(SquaredDistance(a_v));
    }


    //! Returns a perpendicular vector.
    template<typename Real>
    Vector2_generic<Real> Vector2_generic<Real>::Perpendicular() const
    {
      return Vector2_generic<Real>(-m_V[1],
                                    m_V[0]);
    }	//End: Perpendicular()
  }
}

#endif