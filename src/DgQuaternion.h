//@group Math

//! @file DgR3Quaternion.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Quaternion

#ifndef DGQUATERNION_H
#define DGQUATERNION_H

#include "DgMath.h"
#include "DgVector.h"

namespace Dg
{
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  //! Scalar multiplication
  template<typename Real>
  Quaternion<Real> operator*(Real, Quaternion<Real> const &);


  //! Element-wise dot product
  template<typename Real>
  Real Dot(Quaternion<Real> const &, Quaternion<Real> const &);

  //! The Conjugate of a quaternion <b>a = a<sub>1</sub> + a<sub>2</sub>i + a<sub>3</sub>j + a<sub>4</sub>k</b> 
  //! is defined by <b>&#257; = a<sub>1</sub> - a<sub>2</sub>i - a<sub>3</sub>j - a<sub>4</sub>k</b>
  //!
  //! @return Result
  template<typename Real>
  Quaternion<Real> Conjugate(Quaternion<Real> const &);

  //! Inverse of a quaternion
  template<typename Real>
  Quaternion<Real> Inverse(Quaternion<Real> const &);

  //! Linearly interpolate two quaternions.
  //! This will always take the shorter path between them.
  //!
  //! @param[out] a_result Output quaternion
  //! @param[in] a_start Start quaternion
  //! @param[in] a_end End quaternion
  //! @param[in] a_t Factor: 0 < t < 1
  template<typename Real>
  void Lerp(Quaternion<Real>& a_result,
    Quaternion<Real> const & a_start,
    Quaternion<Real> const & a_end,
    Real a_t);

  //! Spherical linearly interpolate two quaternions.
  //! This will always take the shorter path between them.
  //!
  //! @param[out] a_result Output quaternion
  //! @param[in] a_start Start quaternion
  //! @param[in] a_end End quaternion
  //! @param[in] a_t Factor: 0 < t < 1
  template<typename Real>
  void Slerp(Quaternion<Real>& a_result,
    Quaternion<Real> const & a_start,
    Quaternion<Real> const & a_end,
    Real a_t);


  //! Approximate spherical linear interpolation of two quaternions.
  //! Based on "Hacking Quaternions", Jonathan Blow, Game Developer, March 2002.
  //! See Game Developer, February 2004 for an alternate method.
  //!
  //! @param[out] a_result Output quaternion
  //! @param[in] a_start Start quaternion
  //! @param[in] a_end End quaternion
  //! @param[in] a_t Factor: 0 < t < 1
  template<typename Real>
  void ApproxSlerp(Quaternion<Real>& a_result,
    Quaternion<Real> const & a_start,
    Quaternion<Real> const & a_end,
    Real a_t);


  //! @ingroup DgMath_types
  //!
  //! @class Quaternion
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z, w].
  //!
  //! Quaternion class with methods tailored for rotating points and vectors. The
  //! quaternion 4-tuple can theoretically take on any values, however for 
  //! orientation representation in 3D space, it is required the quaternion is 
  //! normalised. It is up to the user to ensure the 
  //! quaternion is normalised if setting each element itself, for eg
  //!
  //!	    Quaternion q(1.0, 2.3, -2.9, 4.6);  //Not a valid rotational quaternion
  //!	    q.Normalize();                      //Valid rotational quaternion
  //!
  //! Quaternions constructed or set by any other means will be valid for eg
  //!
  //!	    void Set( Real zRotation, Real yRotation, Real xRotation );
  //!
  //! Quaternion concatenation uses left-to-right convention. For example the follow lines are equivalent
  //!
  //!     q_final = q_0 * q_1 * q_2 ... * q_n;
  //!     q_final = ( ... ((q_0 * q_1) * q_2) ... * q_n);
  //!
  //! @author James M. Van Verth, Lars M. Bishop, modified by Frank B. Hart
  //! @date 4/10/2015
  template<typename Real>
  class Quaternion
  {
  public:

    //! Default constructed to the identity quaternion (w(1), x(0), y(0), z(0)).
    Quaternion() : w(static_cast<Real>(1.0)), x(static_cast<Real>(0.0)), y(static_cast<Real>(0.0)), z(static_cast<Real>(0.0)) {}

    //! Construct quaternion from coefficients.
    Quaternion(Real a_w, Real a_x, Real a_y, Real a_z) :
      w(a_w), x(a_x), y(a_y), z(a_z) {}

    ~Quaternion() {}

    //! Construct quaternion based on axis-angle.
    Quaternion(const Vector3<Real>& axis, Real angle);

    //! Construct quaternion based on start and end vectors.
    Quaternion(const Vector3<Real>& from, const Vector3<Real>& to);

    //! Construct quaternion from vector elements.
    explicit Quaternion(const Vector3<Real>&);

    //! Copy constructor
    Quaternion(Quaternion const &);

    //! Assignment
    Quaternion& operator=(Quaternion const &);

    //! Accessor member by index
    Real& operator[](unsigned int a_i) { return (&w)[a_i]; }

    //! Accessor member by index
    Real operator[](unsigned int a_i) const { return (&w)[a_i]; }

    //! Length.
    Real Magnitude() const;

    //! Squared length.
    Real Norm() const;

    //! Comparison
    bool operator==(Quaternion const &) const;

    //! Comparison
    bool operator!=(Quaternion const &) const;

    //! Determines if the quaternion is the zero quaternion within some tolerance.
    bool IsZero() const;

    //! Determines if the quaternion is the unit quaternion within some tolerance.
    bool IsUnit() const;

    //! Determines if the quaternion is the identity quaternion within some tolerance.
    bool IsIdentity() const;

    //! Set quaternion elements.
    void Set(Real a_w, Real a_x, Real a_y, Real a_z);

    //! Set quaternion based on rotation about the x axis.
    void SetRotationX(Real);

    //! Set quaternion based on rotation about the y axis.
    void SetRotationY(Real);

    //! Set quaternion based on rotation about the z axis.
    void SetRotationZ(Real);

    //! Set rotation quaternion.
    void SetRotation(Real rx, Real ry, Real rz, EulerOrder);

    //! Set quaternion based on axis-angle.
    void Set(const Vector3<Real>& axis, Real angle);

    //! Set quaternion based on start and end vectors.
    void Set(const Vector3<Real>& from, const Vector3<Real>& to);

    //! Ensures quaternion is a valid rotational quaternion.
    void MakeValid();

    //! Get axis-angle based on quaternion.
    void GetAxisAngle(Vector3<Real>& axis, Real& angle);

    //! Get the set of basis vectors associated with the quaternion
    void GetBasis(Vector3<Real>& a_x0,
                  Vector3<Real>& a_x1,
                  Vector3<Real>& a_x2) const;

    //! Sets near-zero elements to 0.
    void Clean();

    //! Sets all elements to 0.
    inline void Zero();

    //! Sets to unit quaternion.
    void Normalize();

    //! Sets to identity quaternion.
    void Identity();

    //! Set self to complex conjugate.
    Quaternion const & Conjugate();

    //! Set self to inverse.
    Quaternion const & Inverse();

    //! Quaternion addition
    Quaternion operator+(Quaternion const &) const;

    //! Quaternion addition, assign to self
    Quaternion& operator+=(Quaternion const &);

    //! Quaternion subtraction
    Quaternion operator-(Quaternion const &) const;

    //! Quaternion subtraction, assign to self
    Quaternion& operator-=(Quaternion const &);

    //! Negate quaternion
    Quaternion operator-() const;

    //! Scalar multiplication, assign to self
    Quaternion& operator*=(Real);

    //! Quaternion concatenation.
    Quaternion operator*(Quaternion const &) const;

    //! Quaternion concatenation.
    Quaternion& operator*=(Quaternion const &);

    //! Vector rotation.
    //! @pre Quaternion is normalized.
    //!
    //! @return Rotated vector
    Vector3<Real> Rotate(Vector3<Real> const &) const;

    //! Vector rotation. 
    //! @pre Quaternion is normalized.
    void RotateSelf(Vector3<Real>&) const;

  public:

    Real w, x, y, z;
  };


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(Real a_w, Real a_x, Real a_y, Real a_z)
  {
    w = a_w;
    x = a_x;
    y = a_y;
    z = a_z;

  }   // End of Quaternion::Set()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Zero()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Zero()
  {
    x = y = z = w = static_cast<Real>(0);
  }   // End of Quaternion::Zero()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Identity()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Identity()
  {
    x = y = z = static_cast<Real>(0);
    w = static_cast<Real>(1);
  }   // End of Quaternion::Identity

      //-------------------------------------------------------------------------------
      //	@	Quaternion::Quaternion()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector3<Real>& a_axis, Real a_angle)
  {
    Set(a_axis, a_angle);

  }   // End of Quaternion::Quaternion()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Quaternion()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector3<Real>& a_from, const Vector3<Real>& a_to)
  {
    Set(a_from, a_to);

  }   // End of Quaternion::Quaternion()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Quaternion()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector3<Real>& a_vector)
  {
    Set(static_cast<Real>(0), a_vector[0], a_vector[1], a_vector[2]);

  }   // End of Quaternion::Quaternion()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Quaternion()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(Quaternion<Real> const & a_other) :
    w(a_other.w), x(a_other.x), y(a_other.y), z(a_other.z)
  {

  }   // End of Quaternion::Quaternion()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator=(Quaternion<Real> const & a_other)
  {
    w = a_other.w;
    x = a_other.x;
    y = a_other.y;
    z = a_other.z;

    return *this;

  }   // End of Quaternion::operator=()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Magnitude()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Real Quaternion<Real>::Magnitude() const
  {
    return sqrt(w*w + x*x + y*y + z*z);

  }   // End of Quaternion::Magnitude()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Norm()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Real Quaternion<Real>::Norm() const
  {
    return (w*w + x*x + y*y + z*z);

  }   // End of Quaternion::Norm()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator==()
      //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::operator==(Quaternion<Real> const & a_other) const
  {
    return (Dg::IsZero(a_other.w - w)
      && Dg::IsZero(a_other.x - x)
      && Dg::IsZero(a_other.y - y)
      && Dg::IsZero(a_other.z - z));

  }   // End of Quaternion::operator==()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator!=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::operator!=(Quaternion<Real> const & a_other) const
  {
    return !(Dg::IsZero(a_other.w - w)
      && Dg::IsZero(a_other.x - x)
      && Dg::IsZero(a_other.y - y)
      && Dg::IsZero(a_other.z - z));
  }   // End of Quaternion::operator!=()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::IsZero()
      //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsZero() const
  {
    return Dg::IsZero(w*w + x*x + y*y + z*z);

  }   // End of Quaternion::IsZero()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::IsUnit()
      //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - w*w - x*x - y*y - z*z);

  }   // End of Quaternion::IsUnit()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::IsIdentity()
      //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsIdentity() const
  {
    return (Dg::IsZero(static_cast<Real>(1) - w)
      && Dg::IsZero(x)
      && Dg::IsZero(y)
      && Dg::IsZero(z));

  }   // End of Quaternion::IsIdentity()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::SetRotationX()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::SetRotationX(Real a_rx)
  {
    a_rx *= static_cast<Real>(0.5);

    w = static_cast<Real>(cos(a_rx));
    x = static_cast<Real>(sin(a_rx));
    y = static_cast<Real>(0);
    z = static_cast<Real>(0);

  }   // End of Quaternion::SetRotationX()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::SetRotationZ()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::SetRotationY(Real a_ry)
  {
    a_ry *= static_cast<Real>(0.5);

    w = static_cast<Real>(cos(a_ry));
    x = static_cast<Real>(0);
    y = static_cast<Real>(sin(a_ry));
    z = static_cast<Real>(0);

  }   // End of Quaternion::SetRotationZ()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::SetRotation()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::SetRotationZ(Real a_rz)
  {
    a_rz *= static_cast<Real>(0.5);

    w = static_cast<Real>(cos(a_rz));
    x = static_cast<Real>(0);
    y = static_cast<Real>(0);
    z = static_cast<Real>(sin(a_rz));

  }   // End of Quaternion::SetRotationZ()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::SetRotation()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::SetRotation(Real a_rx,
    Real a_ry,
    Real a_rz,
    EulerOrder a_order)
  {
    a_rx *= static_cast<Real>(0.5);
    a_ry *= static_cast<Real>(0.5);
    a_rz *= static_cast<Real>(0.5);

    // get sines and cosines of half angles
    Real Cx = Real(cos(a_rx));
    Real Sx = Real(sin(a_rx));

    Real Cy = Real(cos(a_ry));
    Real Sy = Real(sin(a_ry));

    Real Cz = Real(cos(a_rz));
    Real Sz = Real(sin(a_rz));

    switch (a_order)
    {
    case EulerOrder::ZYX:
    {
      w = Cx*Cy*Cz - Sx*Sy*Sz;
      x = Sx*Cy*Cz + Cx*Sy*Sz;
      y = Cx*Sy*Cz - Sx*Cy*Sz;
      z = Cx*Cy*Sz + Sx*Sy*Cz;
      break;
    }
    case EulerOrder::YZX:
    {
      w = Cx*Cy*Cz + Sx*Sy*Sz;
      x = Sx*Cy*Cz - Cx*Sy*Sz;
      y = Cx*Sy*Cz - Sx*Cy*Sz;
      z = Cx*Cy*Sz + Sx*Sy*Cz;
      break;
    }
    case EulerOrder::ZXY:
    {
      w = Cx*Cy*Cz + Sx*Sy*Sz;
      x = Sx*Cy*Cz + Cx*Sy*Sz;
      y = Cx*Sy*Cz - Sx*Cy*Sz;
      z = Cx*Cy*Sz - Sx*Sy*Cz;
      break;
    }
    case EulerOrder::XZY:
    {
      w = Cx*Cy*Cz - Sx*Sy*Sz;
      x = Sx*Cy*Cz + Cx*Sy*Sz;
      y = Cx*Sy*Cz + Sx*Cy*Sz;
      z = Cx*Cy*Sz - Sx*Sy*Cz;
      break;
    }
    case EulerOrder::XYZ:
    {
      w = Cx*Cy*Cz + Sx*Sy*Sz;
      x = Sx*Cy*Cz - Cx*Sy*Sz;
      y = Cx*Sy*Cz + Sx*Cy*Sz;
      z = Cx*Cy*Sz - Sx*Sy*Cz;
      break;
    }
    case EulerOrder::YXZ:
    {
      w = Cx*Cy*Cz - Sx*Sy*Sz;
      x = Sx*Cy*Cz - Cx*Sy*Sz;
      y = Cx*Sy*Cz + Sx*Cy*Sz;
      z = Cx*Cy*Sz + Sx*Sy*Cz;
      break;
    }
    case EulerOrder::XYX:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Cz*Cy*Sx + Sz*Cy*Cx;
      y = Cz*Sy*Cx + Sz*Sy*Sx;
      z = Sz*Sy*Cx - Cz*Sy*Sx;
      break;
    }
    case EulerOrder::XZX:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Cz*Cy*Sx + Sz*Cy*Cx;
      y = Cz*Sy*Sx - Sz*Sy*Cx;
      z = Cz*Sy*Cx + Sz*Sy*Sx;
      break;
    }
    case EulerOrder::YXY:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Sz*Sy*Sx + Cz*Sy*Cx;
      y = Cz*Cy*Sx + Sz*Cy*Cx;
      z = Cz*Sy*Sx - Sz*Sy*Cx;
      break;
    }
    case EulerOrder::YZY:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Sz*Sy*Cx - Cz*Sy*Sx;
      y = Cz*Cy*Sx + Sz*Cy*Cx;
      z = Cz*Sy*Cx + Sz*Sy*Sx;
      break;
    }
    case EulerOrder::ZXZ:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Cz*Sy*Cx + Sz*Sy*Sx;
      y = Sz*Sy*Cx - Cz*Sy*Sx;
      z = Cz*Cy*Sx + Sz*Cy*Cx;
      break;
    }
    case EulerOrder::ZYZ:
    {
      w = Cz*Cy*Cx - Sz*Cy*Sx;
      x = Cz*Sy*Sx - Sz*Sy*Cx;
      y = Cz*Sy*Cx + Sz*Sy*Sx;
      z = Cz*Cy*Sx + Sz*Cy*Cx;
      break;
    }
    }

  }   // End of Quaternion::SetRotation()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Set()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(const Vector3<Real>& a_axis, Real a_angle)
  {
    // if axis of rotation is zero vector, just set to identity quat
    Real length = MagSq(a_axis);
    if (Dg::IsZero(length))
    {
      Identity();
      return;
    }

    // take half-angle
    a_angle *= static_cast<Real>(0.5);

    Real sintheta = Real(sin(a_angle));
    Real costheta = Real(cos(a_angle));

    Real scaleFactor = sintheta / Real(sqrt(length));

    w = costheta;
    x = scaleFactor * a_axis[0];
    y = scaleFactor * a_axis[1];
    z = scaleFactor * a_axis[2];

  }   // End of Quaternion::Set()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(const Vector3<Real>& a_from, const Vector3<Real>& a_to)
  {
    // get axis of rotation
    Vector3<Real> axis = Cross(a_from, a_to);

    // get scaled cos of angle between vectors and set initial quaternion
    Set(Dot(a_from, a_to), axis[0], axis[1], axis[2]);
    // quaternion at this point is ||from||*||a_to||*( cos(theta), r*sin(theta) )

    // normalize a_to remove ||from||*||a_to|| factor
    Normalize();
    // quaternion at this point is ( cos(theta), r*sin(theta) )
    // what we want is ( cos(theta/2), r*sin(theta/2) )

    // set up for half angle calculation
    w += static_cast<Real>(1);

    // now when we normalize, we'll be dividing by sqrt(2*(1+cos(theta))), which is 
    // what we want for r*sin(theta) a_to give us r*sin(theta/2)  (see pages 487-488)
    // 
    // w will become 
    //                 1+cos(theta)
    //            ----------------------
    //            sqrt(2*(1+cos(theta)))        
    // which simplifies a_to
    //                cos(theta/2)

    // before we normalize, check if vectors are opposing
    if (w <= Dg::Constants<Real>::EPSILON)
    {
      // rotate pi radians around orthogonal vector
      // take cross product with x axis
      if (a_from[2] * a_from[2] > a_from[0] * a_from[0])
        Set(static_cast<Real>(0), static_cast<Real>(0), a_from[2], -a_from[1]);
      // or take cross product with z axis
      else
        Set(static_cast<Real>(0), a_from[1], -a_from[0], static_cast<Real>(0));
    }

    // normalize again a_to get rotation quaternion
    Normalize();

  }   // End of Quaternion::Set()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::MakeValid()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::MakeValid()
  {
    //Set near-zero elements to zero
    Clean();

    //Find Norm
    Real lengthsq = w*w + x*x + y*y + z*z;

    //Return if valid
    if (Dg::IsZero(static_cast<Real>(1) - lengthsq))
      return;

    //If zero quaternion
    if (Dg::IsZero(lengthsq))
    {
      w = static_cast<Real>(1);
      x = y = z = static_cast<Real>(0);
    }
    else
    {
      Real factor = static_cast<Real>(1) / Real(sqrt(lengthsq));
      w *= factor;
      x *= factor;
      y *= factor;
      z *= factor;
    }

  }   // End of Quaternion::Identity


      //-------------------------------------------------------------------------------
      //	@	Quaternion::GetAxisAngle()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::GetAxisAngle(Vector3<Real>& a_axis, Real& a_angle)
  {
    a_angle = static_cast<Real>(2) * Real(acos(w));
    Real length = sqrt(static_cast<Real>(1) - w * w);
    if (Dg::IsZero(length))
    {
      a_axis.Zero();
    }
    else
    {
      length = static_cast<Real>(1) / length;
      a_axis[0] = x * length;
      a_axis[1] = y * length;
      a_axis[2] = z * length;
    }

  }   // End of Quaternion::GetAxisAngle()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::GetBasis()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::GetBasis(Vector3<Real>& a_x0,
    Vector3<Real>& a_x1,
    Vector3<Real>& a_x2) const
  {
    Real xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    xs = x + x;
    ys = y + y;
    zs = z + z;
    wx = w * xs;
    wy = w * ys;
    wz = w * zs;
    xx = x * xs;
    xy = x * ys;
    xz = x * zs;
    yy = y * ys;
    yz = y * zs;
    zz = z * zs;

    a_x0[0] = static_cast<Real>(1) - (yy + zz);
    a_x0[1] = xy + wz;
    a_x0[2] = xz - wy;

    a_x1[0] = xy - wz;
    a_x1[1] = static_cast<Real>(1) - (xx + zz);
    a_x1[2] = yz + wx;

    a_x2[0] = xz + wy;
    a_x2[1] = yz - wx;
    a_x2[2] = static_cast<Real>(1) - (xx + yy);

  }   // End of Quaternion::GetGetBasis()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Clean()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Clean()
  {
    if (Dg::IsZero(w))
      w = static_cast<Real>(0);
    if (Dg::IsZero(x))
      x = static_cast<Real>(0);
    if (Dg::IsZero(y))
      y = static_cast<Real>(0);
    if (Dg::IsZero(z))
      z = static_cast<Real>(0);

  }   // End of Quaternion::Clean()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Normalize()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Normalize()
  {
    Real lengthsq = w * w + x * x + y * y + z * z;

    if (Dg::IsZero(lengthsq))
    {
      Zero();
    }
    else
    {
      Real factor = static_cast<Real>(1) / Real(sqrt(lengthsq));
      w *= factor;
      x *= factor;
      y *= factor;
      z *= factor;
    }

  }   // End of Quaternion::Normalize()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Conjugate()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Conjugate(Quaternion<Real> const & a_quat)
  {
    return Quaternion<Real>(a_quat.w, -a_quat.x, -a_quat.y, -a_quat.z);

  }   // End of Conjugate()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Conjugate()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> const & Quaternion<Real>::Conjugate()
  {
    x = -x;
    y = -y;
    z = -z;

    return *this;

  }   // End of Conjugate()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Inverse()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Inverse(Quaternion<Real> const & a_quat)
  {
    Quaternion<Real> q(a_quat);
    q.Inverse();
    return q;

  }   // End of Inverse()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Inverse()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> const & Quaternion<Real>::Inverse()
  {
    Real norm = (w * w) + (x * x) + (y * y) + (z * z);

    // if we're the zero quaternion, just return
    if (Dg::IsZero(norm))
      return *this;

    Real normRecip = static_cast<Real>(1) / norm;
    w = normRecip * w;
    x = -normRecip * x;
    y = -normRecip * y;
    z = -normRecip * z;

    return *this;

  }   // End of Inverse()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator+()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator+(Quaternion<Real> const & a_other) const
  {
    return Quaternion<Real>(w + a_other.w,
      x + a_other.x,
      y + a_other.y,
      z + a_other.z);

  }   // End of Quaternion::operator+()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator+=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator+=(Quaternion<Real> const & a_other)
  {
    w += a_other.w;
    x += a_other.x;
    y += a_other.y;
    z += a_other.z;

    return *this;

  }   // End of Quaternion::operator+=()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator-()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator-(Quaternion<Real> const & a_other) const
  {
    return Quaternion<Real>(w - a_other.w,
      x - a_other.x,
      y - a_other.y,
      z - a_other.z);

  }   // End of Quaternion::operator-()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator-=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator-=(Quaternion<Real> const & a_other)
  {
    w -= a_other.w;
    x -= a_other.x;
    y -= a_other.y;
    z -= a_other.z;

    return *this;

  }   // End of Quaternion::operator-=()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator-()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator-() const
  {
    return Quaternion<Real>(-w, -x, -y, -z);

  }    // End of Quaternion::operator-()


        //-------------------------------------------------------------------------------
        //	@	Quaternion::operator*()
        //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> operator*(Real a_scalar, Quaternion<Real> const & a_quat)
  {
    return Quaternion<Real>(a_scalar * a_quat.w,
      a_scalar * a_quat.x,
      a_scalar * a_quat.y,
      a_scalar * a_quat.z);

  }   // End of operator*()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator*=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator*=(Real a_scalar)
  {
    w *= a_scalar;
    x *= a_scalar;
    y *= a_scalar;
    z *= a_scalar;

    return *this;

  }   // End of Quaternion::operator*=()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator*()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator*(Quaternion<Real> const & a_other) const
  {
    return Quaternion<Real>(
      a_other.w*w - a_other.x*x - a_other.y*y - a_other.z*z,
      a_other.w*x + a_other.x*w + a_other.y*z - a_other.z*y,
      a_other.w*y + a_other.y*w + a_other.z*x - a_other.x*z,
      a_other.w*z + a_other.z*w + a_other.x*y - a_other.y*x);

  }   // End of Quaternion::operator*()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::operator*=()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator*=(Quaternion<Real> const & a_other)
  {
    Set(a_other.w*w - a_other.x*x - a_other.y*y - a_other.z*z,
      a_other.w*x + a_other.x*w + a_other.y*z - a_other.z*y,
      a_other.w*y + a_other.y*w + a_other.z*x - a_other.x*z,
      a_other.w*z + a_other.z*w + a_other.x*y - a_other.y*x);

    return *this;

  }   // End of Quaternion::operator*=()


      //-------------------------------------------------------------------------------
      //	@	Dot()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Real Dot(Quaternion<Real> const & a_quat1, Quaternion<Real> const & a_quat2)
  {
    return (a_quat1.w * a_quat2.w
      + a_quat1.x * a_quat2.x
      + a_quat1.y * a_quat2.y
      + a_quat1.z * a_quat2.z);

  }   // End of Dot()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Rotate()
      //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Quaternion<Real>::Rotate(const Vector3<Real>& a_vector) const
  {
    Real vMult = static_cast<Real>(2) * (x*a_vector[0] + y*a_vector[1] + z*a_vector[2]);
    Real crossMult = static_cast<Real>(2) * w;
    Real pMult = crossMult * w - static_cast<Real>(1);

    Vector3<Real> result;
    result[0] = pMult*a_vector[0] + vMult*x + crossMult*(y*a_vector[2] - z*a_vector[1]);
    result[1] = pMult*a_vector[1] + vMult*y + crossMult*(z*a_vector[0] - x*a_vector[2]);
    result[2] = pMult*a_vector[2] + vMult*z + crossMult*(x*a_vector[1] - y*a_vector[0]);

    return result;

  }   // End of Quaternion::Rotate()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::RotateSelf()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::RotateSelf(Vector3<Real>& a_vector) const
  {
    Real vMult = static_cast<Real>(2) * (x*a_vector[0] + y*a_vector[1] + z*a_vector[2]);
    Real crossMult = static_cast<Real>(2) * w;
    Real pMult = crossMult * w - static_cast<Real>(1);

    Real _x = pMult*a_vector[0] + vMult*x + crossMult*(y*a_vector[2] - z*a_vector[1]);
    Real _y = pMult*a_vector[1] + vMult*y + crossMult*(z*a_vector[0] - x*a_vector[2]);
    Real _z = pMult*a_vector[2] + vMult*z + crossMult*(x*a_vector[1] - y*a_vector[0]);

    a_vector[0] = _x;
    a_vector[1] = _y;
    a_vector[2] = _z;
  }   // End of Quaternion::RotateSelf()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Lerp()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Lerp(Quaternion<Real>& a_result, Quaternion<Real> const & a_start, Quaternion<Real> const & a_end, Real a_t)
  {
    // get cos of "angle" between quaternions
    Real cosTheta = Dot(a_start, a_end);

    // initialize a_result
    a_result = a_t * a_end;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= Dg::Constants<Real>::EPSILON)
    {
      // use standard interpolation
      a_result += (static_cast<Real>(1) - a_t) * a_start;
    }
    else
    {
      // otherwise, take the shorter path
      a_result += (a_t - static_cast<Real>(1)) * a_start;
    }

  }   // End of Lerp()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::Slerp()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void Slerp(Quaternion<Real>& a_result, Quaternion<Real> const & a_start, Quaternion<Real> const & a_end, Real a_t)
  {
    // get cosine of "angle" between quaternions
    Real cosTheta = Dot(a_start, a_end);
    Real startInterp, endInterp;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= Dg::Constants<Real>::EPSILON)
    {
      // if angle is greater than zero
      if ((static_cast<Real>(1) - cosTheta) > Dg::Constants<Real>::EPSILON)
      {
        // use standard slerp
        Real theta = Real(acos(cosTheta));
        Real recipSinTheta = static_cast<Real>(1) / Real(sin(theta));

        startInterp = Real(sin((static_cast<Real>(1) - a_t) * theta)) * recipSinTheta;
        endInterp = Real(sin(a_t*theta)) * recipSinTheta;
      }
      // angle is close to zero
      else
      {
        // use linear interpolation
        startInterp = static_cast<Real>(1) - a_t;
        endInterp = a_t;
      }
    }
    // otherwise, take the shorter route
    else
    {
      // if angle is less than 180 degrees
      if ((static_cast<Real>(1) + cosTheta) > Dg::Constants<Real>::EPSILON)
      {
        // use slerp w/negation of a_start quaternion
        Real theta = Real(acos(-cosTheta));
        Real recipSinTheta = static_cast<Real>(1) / Real(sin(theta));

        startInterp = Real(sin((a_t - static_cast<Real>(1)) * theta)) * recipSinTheta;
        endInterp = Real(sin(a_t * theta)) * recipSinTheta;
      }
      // angle is close to 180 degrees
      else
      {
        // use lerp w/negation of a_start quaternion
        startInterp = a_t - static_cast<Real>(1);
        endInterp = a_t;
      }
    }

    a_result = startInterp * a_start + endInterp * a_end;

  }   // End of Slerp()


      //-------------------------------------------------------------------------------
      //	@	Quaternion::ApproxSlerp()
      //-------------------------------------------------------------------------------
  template<typename Real>
  void ApproxSlerp(Quaternion<Real>& a_result, Quaternion<Real> const & a_start, Quaternion<Real> const & a_end, Real a_t)
  {
    Real cosTheta = Dot(a_start, a_end);

    // correct time by using cosine of angle between quaternions
    Real factor = static_cast<Real>(1) - static_cast<Real>(0.7878088) * cosTheta;
    Real k = static_cast<Real>(0.5069269);
    factor *= factor;
    k *= factor;

    Real b = static_cast<Real>(2) * k;
    Real c = static_cast<Real>(-3) * k;
    Real d = static_cast<Real>(1) + k;

    a_t = a_t * (b * a_t + c) + d;

    // initialize a_result
    a_result = a_t * a_end;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= Dg::Constants<Real>::EPSILON)
    {
      // use standard interpolation
      a_result += (static_cast<Real>(1) - a_t) * a_start;
    }
    else
    {
      // otherwise, take the shorter path
      a_result += (a_t - static_cast<Real>(1)) * a_start;
    }

  }   // a_end of ApproxSlerp()
}

#endif
