//@group Math

//! @file DgR3VQS.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: VQS

#ifndef DGR3VQS_H
#define DGR3VQS_H

#include "DgVector.h"
#include "DgQuaternion.h"
#include "DgMatrix44.h"
#include "DgMath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class VQS
  //!
  //! @brief Vector, quaternion, scalar structure
  //!
  //! This is an alternate to matrices to represent
  //! transformations in 3D space. In this class we have a translation component
  //! (Vector), orientation (Quaternion) and scaling component (Scalar). The main
  //! advantage over a 4x4 matrix is using a quaternion for orientation, arguably
  //! a more complete and robust method for orientation representation. 
  //!
  //! Some disatvantages include: we are restricted to the types of transformations
  //! we can do. We cannot shear or reflect for example. Also We must use uniform
  //! scaling.
  //!
  //! @author Frank B. Hart
  //! @date 4/10/2015
  template<typename Real>
  class VQS
  {
  public:
    //! Default constructor set to identity
    VQS() : m_v(Zeros<Real, 3>()),
            m_q(static_cast<Real>(1),
                static_cast<Real>(0),
                static_cast<Real>(0),
                static_cast<Real>(0)),
            m_s(static_cast<Real>(1))
     {}

    //! Construct vqs from vector, quaternion and scalar
    VQS(Vector3<Real> const & a_v, Quaternion<Real> const & a_q, Real a_s)
      : m_v(a_v)
      , m_q(a_q)
      , m_s(a_s)
    {
    }

    ~VQS() {}

    //! Copy constructor
    VQS(VQS<Real> const & a_other) 
      : m_v(a_other.m_v)
      , m_q(a_other.m_q)
      , m_s(a_other.m_s) 
    {}

    //! Assignment
    VQS<Real>& operator=(VQS<Real> const &);

    //! Comparison
    bool operator==(VQS<Real> const & a_other) const;

    //! Comparison
    bool operator!=(VQS<Real> const & a_other) const;

    //! Ensure a valid VQS.
    void MakeValid();

    //! Make identity.
    void Identity();

    //! Set VQS based on an affine matrix
    void Set(Matrix44<Real> const &);

    //! Set VQS from vector, quaternion and scalar
    //! Inputs are NOT validated
    void Set(Vector3<Real> const &, Quaternion<Real> const &, Real);

    //! vqs-vqs concatenation. VQS structures concatenate left to right.
    VQS<Real> operator* (VQS<Real> const &) const;

    //! vqs-vqs concatenation, assign to self. 
    //! VQS structures concatenate left to right.
    //! @return Reference to self
    VQS<Real>& operator*= (VQS<Real> const &);

    //! Point transformations also apply translation.
    Vector3<Real> TransformPoint(Vector3<Real> const &) const;

    //! Vector transformations do not apply translation.
    Vector3<Real> TransformVector(Vector3<Real> const &) const;

    //! Apply translation to Vector.
    Vector3<Real> Translate(Vector3<Real> const &) const;

    //! Apply rotation to Vector.
    Vector3<Real> Rotate(Vector3<Real> const &) const;

    //! Apply scale to Vector.
    Vector3<Real> Scale(Vector3<Real> const &) const;

    //! Inverse.
    VQS & SetInverse();

    //! Inverse.
    VQS GetInverse() const;

    //! Conversion to Matrix.
    void GetMatrix(Matrix44<Real> & a_out) const;
    
    Vector3<Real> const & V()	const { return m_v; }
    Quaternion<Real> const & Q()  const { return m_q; }
    Real S()	const { return m_s; }

    Vector3<Real> & V()	{ return m_v; }
    Quaternion<Real> & Q()  { return m_q; }
    Real & S()	{ return m_s; }

  private:

    Vector3<Real>		  m_v;		//translation
    Quaternion<Real>	m_q;		//rotation
    Real              m_s;		//scale
  };


  template<typename Real>
  void VQS<Real>::Set(Matrix44<Real> const & a_m)
  {
    m_v = a_m.GetTranslation()
    m_q = a_m.GetQuaternion();

    //Just use matrix x-scaling
    m_s = sqrt((a_m[0] * a_m[0]) + (a_m[4] * a_m[4]) + (a_m[8] * a_m[8]));
  }

  template<typename Real>
  bool VQS<Real>::operator==(VQS<Real> const & a_other) const
  {
    return !((a_other.m_v != m_v)
      || (a_other.m_q != m_q)
      || (a_other.m_s != m_s));
  }


  template<typename Real>
  bool VQS<Real>::operator!=(VQS<Real> const & a_other) const
  {
    return ((a_other.m_v != m_v)
      || (a_other.m_q != m_q)
      || (a_other.m_s != m_s));
  }


  template<typename Real>
  VQS<Real>& VQS<Real>::operator=(VQS<Real> const & a_other)
  {
    m_v = a_other.m_v;
    m_q = a_other.m_q;
    m_s = a_other.m_s;

    return *this;
  }


  template<typename Real>
  void VQS<Real>::MakeValid()
  {
    //Clean quaternion
    m_q.MakeValid();

    //Clean scale;
    if (m_s < Dg::Constants<Real>::EPSILON)
      m_s = static_cast<Real>(0);
  }


  template<typename Real>
  void VQS<Real>::Identity()
  {
    m_v.Zero();
    m_q.Identity();
    m_s = static_cast<Real>(1);
  }


  template<typename Real>
  Vector3<Real> VQS<Real>::TransformPoint(Vector3<Real> const & a_v) const
  {
    return m_q.Rotate(a_v * m_s) + m_v;
  }


  template<typename Real>
  Vector3<Real> VQS<Real>::TransformVector(Vector3<Real> const & a_v) const
  {
    return m_q.Rotate(a_v * m_s);
  }
  

  template<typename Real>
  Vector3<Real> VQS<Real>::Translate(Vector3<Real> const & a_v) const
  {
    return m_v + a_v;
  }


  template<typename Real>
  Vector3<Real> VQS<Real>::Rotate(Vector3<Real> const & a_v) const
  {
    return m_q.Rotate(a_v);
  }


  template<typename Real>
  Vector3<Real> VQS<Real>::Scale(Vector3<Real> const & a_v) const
  {
    return a_v * m_s;
  }
  

  template<typename Real>
  void VQS<Real>::Set(Vector3<Real>    const & a_v,
                      Quaternion<Real> const & a_q,
                      Real                     a_s)
  {
    m_v = a_v;
    m_q = a_q;
    m_s = a_s;
  }

  template<typename Real>
  void VQS<Real>::GetMatrix(Matrix44<Real> & a_out) const
  {
    a_out.Rotation(m_q);
    a_out[0] *= m_s;
    a_out[1] *= m_s;
    a_out[2] *= m_s;
    a_out[4] *= m_s;
    a_out[5] *= m_s;
    a_out[6] *= m_s;
    a_out[8] *= m_s;
    a_out[9] *= m_s;
    a_out[10] *= m_s;

    a_out[12] = m_v.x();
    a_out[13] = m_v.y();
    a_out[14] = m_v.z();
  }


  template<typename Real>
  VQS<Real> VQS<Real>::operator*(VQS<Real> const & a_rhs) const
  {
    return VQS(a_rhs.m_q.Rotate(m_v) * a_rhs.m_s + a_rhs.m_v,
                m_q * a_rhs.m_q,
                m_s * a_rhs.m_s);
  }


  template<typename Real>
  VQS<Real>& VQS<Real>::operator*=(VQS<Real> const & a_rhs)
  {
    m_v = a_rhs.m_q.Rotate(m_v) * a_rhs.m_s + a_rhs.m_v;
    m_q = m_q * a_rhs.m_q;
    m_s *= a_rhs.m_s;
    return *this;
  }


  template<typename Real>
  VQS<Real> & VQS<Real>::SetInverse()
  {
    //The method to find a VQS inverse
    //[1/m_s*(m_q-1*(-m_v)*m_q), m_q-1, 1/m_s]

    //Inverse scale
    m_s = static_cast<Real>(1) / m_s;

    //Inverse quaternion
    m_q.Inverse();

    //Inverse vector
    m_v = m_q.Rotate(-m_v) * m_s;

    return *this;
  }


  template<typename Real>
  VQS<Real> VQS<Real>::GetInverse() const
  {
    VQS<Real> temp(*this);
    temp.SetInverse();
    return temp;
  }
}

#endif