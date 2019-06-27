//! @file DgR3VQS.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: VQS

#ifndef DGR3VQS_H
#define DGR3VQS_H

#include "DgR3Vector.h"
#include "DgR3Quaternion.h"
#include "DgR3Matrix.h"
#include "DgMath.h"

namespace Dg
{
  namespace R3
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
      VQS() : m_q(static_cast<Real>(1.0),
                  static_cast<Real>(0.0),
                  static_cast<Real>(0.0),
                  static_cast<Real>(0.0)),
                  m_s(static_cast<Real>(1.0)),
                  m_v(Vector<Real>::ZeroVector()) {}

      //! Construct vqs from vector, quaternion and scalar
      VQS(Vector<Real> const & a_v, Quaternion<Real> const & a_q, Real a_s) 
        : m_v(a_v)
        , m_q(a_q)
        , m_s(a_s) 
      {}

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
      void Set(Matrix<Real> const &);

      //! Set VQS from vector, quaternion and scalar
      //! Inputs are NOT validated
      void Set(Vector<Real> const &, Quaternion<Real> const &, Real);

      //! Set vector of the vqs
      void SetV(Vector<Real> const &);

      //! Set quaternion of the vqs
      void SetQ(Quaternion<Real> const &);

      //! Set scalar of the vqs
      void SetS(Real);

      //! Translation update data
      void UpdateV(Vector<Real> const &);

      //! Quaternion update data
      void UpdateQ(Quaternion<Real> const &);

      //! Scalar update data
      void UpdateS(Real);

      //! vqs-vqs concatenation. VQS structures concatenate left to right.
      VQS<Real> operator* (VQS<Real> const &) const;

      //! vqs-vqs concatenation, assign to self. 
      //! VQS structures concatenate left to right.
      //! @return Reference to self
      VQS<Real>& operator*= (VQS<Real> const &);

      //! Point transformations also apply translation.
      Vector<Real> TransformPoint(Vector<Real> const &) const;

      //! Vector transformations do not apply translation.
      Vector<Real> TransformVector(Vector<Real> const &) const;

      //! Point transformations also apply translation.
      Vector<Real> & TransformPointSelf(Vector<Real> &) const;

      //! Vector transformations do not apply translation.
      Vector<Real> & TransformVectorSelf(Vector<Real> &) const;

      //! Apply translation to Vector.
      Vector<Real> Translate(Vector<Real> const &) const;

      //! Apply rotation to Vector.
      Vector<Real> Rotate(Vector<Real> const &) const;

      //! Apply scale to Vector.
      Vector<Real> Scale(Vector<Real> const &) const;

      //! Apply translation to Vector.
      Vector<Real> & TranslateSelf(Vector<Real>&) const;

      //! Apply rotation to Vector.
      Vector<Real> & RotateSelf(Vector<Real>&) const;

      //! Apply scale to Vector.
      Vector<Real> & ScaleSelf(Vector<Real>&) const;

      //! Inverse.
      VQS & SetInverse();

      //! Inverse.
      VQS GetInverse() const;

      //! Get the vector, quaternion and scalar
      void Get(Vector<Real>& a_v, Quaternion<Real>& a_q, Real& a_s) const;

      //! Conversion to Matrix.
      void GetMatrix(Matrix<Real>&) const;

      //! Access vector
      Vector<Real> const & V()	    const { return m_v; }

      //! Access quaternion
      Quaternion<Real> const & Q()  const { return m_q; }

      //! Access scalar
      Real S()	                    const { return m_s; }

    private:
      //Data members
      Vector<Real>		  m_v;		//translation
      Quaternion<Real>	m_q;		//rotation
      Real		          m_s;		//scale
    };

    //--------------------------------------------------------------------------------
    //	@	VQS<Real>::Set()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::Set(Matrix<Real> const & a_m)
    {
      m_v.x() = a_m[12];
      m_v.y() = a_m[13];
      m_v.z() = a_m[14];

      a_m.GetQuaternion(m_q);

      //Just use matrix x-scaling
      m_s = sqrt((a_m[0] * a_m[0]) + (a_m[4] * a_m[4]) + (a_m[8] * a_m[8]));

    }	//End: VQS<Real>::Set()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::SetV(Vector<Real> const & a_v)
    {
      m_v = a_v;

    }	//End: VQS<Real>::Set()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::SetQ()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::SetQ(Quaternion<Real> const & a_q)
    {
      m_q = a_q;

    }	//End: VQS<Real>::Set()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::SetS()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::SetS(Real a_s)
    {
      m_s = a_s;

    }	//End: VQS<Real>::Set()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::UpdateV()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::UpdateV(Vector<Real> const & a_v)
    {
      m_v += a_v;

    }	//End: VQS<Real>::UpdateV()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::UpdateQ()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::UpdateQ(Quaternion<Real> const & a_q)
    {
      m_q *= a_q;

    }	//End: VQS<Real>::UpdateQ()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::UpdateS()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::UpdateS(Real a_s)
    {
      m_s *= a_s;

    }	//End: VQS<Real>::UpdateS()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool VQS<Real>::operator==(VQS<Real> const & a_other) const
    {
      return !((a_other.m_v != m_v)
        || (a_other.m_q != m_q)
        || (a_other.m_s != m_s));

    }	//End: VQS<Real>::operator==();


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool VQS<Real>::operator!=(VQS<Real> const & a_other) const
    {
      return ((a_other.m_v != m_v)
        || (a_other.m_q != m_q)
        || (a_other.m_s != m_s));

    }	//End: VQS<Real>::operator!=();


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    VQS<Real>& VQS<Real>::operator=(VQS<Real> const & a_other)
    {
      m_v = a_other.m_v;
      m_q = a_other.m_q;
      m_s = a_other.m_s;

      return *this;

    }	//End: VQS<Real>::operator=();


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::MakeValid
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::MakeValid()
    {
      //Clean vector
      m_v[3] = static_cast<Real>(0.0);

      //Clean quaternion
      m_q.MakeValid();

      //Clean scale;
      if (m_s < Dg::Constants<Real>::EPSILON)
        m_s = static_cast<Real>(0.0);

    }	//End: VQS<Real>::MakeValid()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Identity()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::Identity()
    {
      m_v.Zero();

      m_q.Identity();

      m_s = static_cast<Real>(1.0);

    }	//End: VQS<Real>::Identity()


      //--------------------------------------------------------------------------------
      //	@	TransformPoint()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> VQS<Real>::TransformPoint(Vector<Real> const & a_v) const
    {
      Vector<Real> result(a_v);

      //Scale
      result[0] *= m_s;
      result[1] *= m_s;
      result[2] *= m_s;

      //Rotate;
      m_q.RotateSelf(result);

      //Translate
      result += m_v;

      return result;

    }	//End: TransformPoint()


      //--------------------------------------------------------------------------------
      //	@	TransformPointSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>& VQS<Real>::TransformPointSelf(Vector<Real> & a_v) const
    {
      //Scale
      a_v[0] *= m_s;
      a_v[1] *= m_s;
      a_v[2] *= m_s;

      //Rotate;
      m_q.RotateSelf(a_v);

      //Translate
      a_v += m_v;

      return a_v;

    }	//End: TransformPointSelf()


      //--------------------------------------------------------------------------------
      //	@	TransformVector()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> VQS<Real>::TransformVector(Vector<Real> const & a_v) const
    {
      Vector<Real> result(a_v);

      //Scale
      result[0] *= m_s;
      result[1] *= m_s;
      result[2] *= m_s;

      //Rotate;
      m_q.RotateSelf(result);

      return result;

    }	//End: TransformVector()


      //--------------------------------------------------------------------------------
      //	@	TransformVectorSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>& VQS<Real>::TransformVectorSelf(Vector<Real> & a_v) const
    {
      //Scale
      a_v[0] *= m_s;
      a_v[1] *= m_s;
      a_v[2] *= m_s;

      //Rotate;
      m_q.RotateSelf(a_v);

      return a_v;

    }	//End: TransformVectorSelf()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Translate()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> VQS<Real>::Translate(Vector<Real> const & a_v) const
    {
      Vector<Real> result(a_v);

      result += m_v;

      return result;

    }	//End: VQS<Real>::Translate()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Rotate()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> VQS<Real>::Rotate(Vector<Real> const & a_v) const
    {
      Vector<Real> result(a_v);

      m_q.RotateSelf(result);

      return result;

    }	//End: VQS<Real>::Rotate()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Scale()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> VQS<Real>::Scale(Vector<Real> const & a_v) const
    {
      Vector<Real> result(a_v);

      result.m_x *= m_s;
      result.m_y *= m_s;
      result.m_z *= m_s;

      return result;

    }	//End: VQS<Real>::Scale()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::TranslateSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> & VQS<Real>::TranslateSelf(Vector<Real>& a_v) const
    {
      a_v.x += m_v.x;
      a_v.y += m_v.y;
      a_v.z += m_v.z;

      return a_v;

    }	//End: VQS<Real>::TranslateSelf()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::RotateSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> & VQS<Real>::RotateSelf(Vector<Real>& a_v) const
    {
      //Rotate;
      m_q.RotateSelf(a_v);

      return a_v;

    }	//End: VQS<Real>::RotateSelf()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::ScaleSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> & VQS<Real>::ScaleSelf(Vector<Real>& a_v) const
    {
      //Scale
      a_v.x *= m_s;
      a_v.y *= m_s;
      a_v.z *= m_s;

      return a_v;

    }	//End: VQS<Real>::ScaleSelf()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::Set(Vector<Real>    const & a_v,
      Quaternion<Real> const & a_q,
      Real                     a_s)
    {
      SetV(a_v);
      SetQ(a_q);
      SetS(a_s);

    }	//End: VQS<Real>::Set()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Get()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::Get(Vector<Real>&    a_v,
      Quaternion<Real>& a_q,
      Real&             a_s) const
    {
      a_v = m_v;
      a_q = m_q;
      a_s = m_s;

    }	//End: VQS<Real>::Get()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Get()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void VQS<Real>::GetMatrix(Matrix<Real> & a_out) const
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

    }	//End: VQS<Real>::Get()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::operator*()
      //--------------------------------------------------------------------------------
    template<typename Real>
    VQS<Real> VQS<Real>::operator*(VQS<Real> const & a_rhs) const
    {
      return VQS(a_rhs.m_q.Rotate(m_v)*a_rhs.m_s + a_rhs.m_v,
                 m_q * a_rhs.m_q,
                 m_s * a_rhs.m_s);
    }	//End: VQS<Real>::operator*()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Operator*=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    VQS<Real>& VQS<Real>::operator*=(VQS<Real> const & a_rhs)
    {
      m_v = a_rhs.m_q.Rotate(m_v)*a_rhs.m_s + a_rhs.m_v;
      m_q = m_q * a_rhs.m_q;
      m_s *= a_rhs.m_s;
      return *this;

    }	//End: VQS<Real>::operator*=()


      //--------------------------------------------------------------------------------
      //	@	VQS<Real>::Inverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    VQS<Real> & VQS<Real>::SetInverse()
    {
      //The method to find a VQS inverse
      //[1/m_s*(m_q-1*(-m_v)*m_q), m_q-1, 1/m_s]

      //Inverse scale
      m_s = static_cast<Real>(1.0) / m_s;

      //Inverse quaternion
      m_q.Inverse();

      //Inverse vector
      m_v = m_q.Rotate(-m_v) * m_s;

      return *this;

    }	//End: Inverse()


      //--------------------------------------------------------------------------------
      //	@	Inverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    VQS<Real> VQS<Real>::GetInverse() const
    {
      VQS<Real> temp;

      //Inverse scale
      temp.m_s = static_cast<Real>(1.0) / m_s;

      //Inverse quaternion
      temp.m_q = Inverse(m_q);

      //Inverse vector
      temp.m_v = temp.m_q.Rotate(-m_v) * temp.m_s;

      return temp;

    }	//End: Inverse()
  }
}

#endif