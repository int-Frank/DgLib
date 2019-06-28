//! @file DgR3Matrix_generic.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGR3MATRIX_GENERIC_H
#define DGR3MATRIX_GENERIC_H

#include "DgMath.h"
#include "../DgMatrix.h"
#include "../DgR3Quaternion.h"
#include "DgR3Vector_generic.h"

//--------------------------------------------------------------------------------
//	@	Matrix_generic
//--------------------------------------------------------------------------------
namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Matrix_generic
    //!
    //! @brief 4 by 4 matrix calss.
    //!
    //! Originally retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //! On Date: 2013
    //!
    //!     Memory layout: { x.x, x.y, x.z, 0, y.x, y.y, y.z, 0, z.x, z.y, z.z, 0, p.x, p.y, p.z, 1 }
    //!     Row major: [x.x x.y x.z 0]
    //!                [y.x y.y y.z 0]
    //!                [z.x z.y z.z 0]
    //!                [p.x p.y p.z 1]
    //!
    //! Matrix concatenation uses left-to-right convention. For example the follow lines are equivalent
    //!
    //!     m_final = m_0 * m_1 * m_2 ... * m_n;
    //!     m_final = ( ... ((m_0 * m_1) * m_2) ... * m_n);
    //!
    //! @author James M. Van Verth, Lars M. Bishop, modified by Frank B. Hart
    //! @date 4/10/2015
    template<typename Real>
    class Matrix_generic<Real, 3> : public Matrix<4, 4, Real>
    {
    public:
      //! Default constructor, initialized to identity matrix.
      Matrix_generic() { Identity(); }
      ~Matrix_generic() {}

      //! Copy Constructor
      Matrix_generic(Matrix<4, 4, Real> const & a_other) 
        : Matrix<4, 4, Real>(a_other) {}

      //! Assignment
      Matrix_generic& operator=(Matrix<4, 4, Real> const &);

      //! Set matrix by rows
      void SetRows(Matrix<1, 4, Real> const & row0, 
                   Matrix<1, 4, Real> const & row1,
                   Matrix<1, 4, Real> const & row2, 
                   Matrix<1, 4, Real> const & row3);

      //! Get rows of the matrix
      void GetRows(Matrix<1, 4, Real> & row0, 
                   Matrix<1, 4, Real> & row1,
                   Matrix<1, 4, Real> & row2, 
                   Matrix<1, 4, Real> & row3) const;

      //! Set matrix by columns
      void SetColumns(Matrix<4, 1, Real> const & col0, 
                      Matrix<4, 1, Real> const & col1,
                      Matrix<4, 1, Real> const & col2, 
                      Matrix<4, 1, Real> const & col3);

      //! Get matrix columns
      void GetColumns(Matrix<4, 1, Real> & col0, 
                      Matrix<4, 1, Real> & col1,
                      Matrix<4, 1, Real> & col2, 
                      Matrix<4, 1, Real> & col3) const;

      //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      Matrix_generic & SetAffineInverse();

      //! Get matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      Matrix_generic GetAffineInverse();

      //! Set as translation matrix based on vector
      Matrix_generic & Translation(Matrix<1, 4, Real> const &);

      //! Sets the matrix to a rotation matrix (by Euler angles).
      Matrix_generic & Rotation(Real xRotation, Real yRotation, Real zRotation, EulerOrder);

      //! Sets the matrix to a rotation matrix (by axis and angle).
      Matrix_generic & Rotation(Matrix<1, 4, Real> const & axis, Real angle);

      //! Set the matrix to rotation matrix based on quaternion
      Matrix_generic & Rotation(R3::Quaternion<Real> const &);

      //! Build a quaternion from the upper 3x3 matrix 
      //! (assumes upper 3x3 is a valid rotation matrix)
      R3::Quaternion<Real> GetQuaternion() const;

      //! Set as scaling matrix based on vector.
      Matrix_generic & Scaling(Matrix<1, 4, Real> const &);

      //! Uniform scaling.
      Matrix_generic & Scaling(Real);

      //! Set as rotation matrix, rotating by 'angle' radians around x-axis.
      Matrix_generic & RotationX(Real);

      //! Set as rotation matrix, rotating by 'angle' radians around y-axis.
      Matrix_generic & RotationY(Real);

      //! Set as rotation matrix, rotating by 'angle' radians around z-axis.
      Matrix_generic & RotationZ(Real);

      Matrix_generic & Perspective(Real a_fov,
                                   Real a_ar,
                                   Real a_near,
                                   Real a_far);

      Matrix_generic & LookAt(Vector_generic<Real, 3> const & a_origin,
                              Vector_generic<Real, 3> const & a_target,
                              Vector_generic<Real, 3> const & a_up);
    };


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::operator=(Matrix<4, 4, Real> const & a_other)
    {
      Matrix<4, 4, Real>::operator=(a_other);
      return *this;
    }

    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::SetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 3>::SetRows(Matrix<1, 4, Real> const & a_row0, 
                                          Matrix<1, 4, Real> const & a_row1,
                                          Matrix<1, 4, Real> const & a_row2, 
                                          Matrix<1, 4, Real> const & a_row3)
    {
      SetRow(0, a_row0);
      SetRow(1, a_row1);
      SetRow(2, a_row2);
      SetRow(3, a_row3);
    }	//End: Matrix_generic::SetRows()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::GetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 3>::GetRows(Matrix<1, 4, Real>& a_row0, 
                                          Matrix<1, 4, Real>& a_row1,
                                          Matrix<1, 4, Real>& a_row2, 
                                          Matrix<1, 4, Real>& a_row3) const
    {
      GetRow(0, a_row0);
      GetRow(1, a_row1);
      GetRow(2, a_row2);
      GetRow(3, a_row3);
    }	//End: Matrix_generic::GetRows()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::SetColumns()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 3>::SetColumns(Matrix<4, 1, Real> const & a_col0,
                                             Matrix<4, 1, Real> const & a_col1,
                                             Matrix<4, 1, Real> const & a_col2, 
                                             Matrix<4, 1, Real> const & a_col3)
    {
      SetColumn(0, a_col0);
      SetColumn(1, a_col1);
      SetColumn(2, a_col2);
      SetColumn(3, a_col3);
    }	//End: Matrix_generic::SetColumns()


      //--------------------------------------------------------------------------------
      //	@	Matrix_generic::GetColumns()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 3>::GetColumns(Matrix<4, 1, Real>& a_col0, 
                                             Matrix<4, 1, Real>& a_col1,
                                             Matrix<4, 1, Real>& a_col2, 
                                             Matrix<4, 1, Real>& a_col3) const
    {
      GetColumn(0, a_col0);
      GetColumn(1, a_col1);
      GetColumn(2, a_col2);
      GetColumn(3, a_col3);
    }	//End: Matrix_generic::GetColumns()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::SetAffineInverse()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3>& Matrix_generic<Real, 3>::SetAffineInverse()
    {
      *this = GetAffineInverse(*this);
      return *this;
    }	//End: Matrix_generic::SetAffineInverse


    //--------------------------------------------------------------------------------
    //	@	GetAffineInverse()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> Matrix_generic<Real, 3>::GetAffineInverse()
    {
      Matrix_generic<Real, 3> result;

      //compute upper left 3x3 matrix determinant
      Real cofactor0 = m_V[5] * m_V[10] - m_V[9] * m_V[6];
      Real cofactor1 = m_V[6] * m_V[8] - m_V[4] * m_V[10];
      Real cofactor2 = m_V[4] * m_V[9] - m_V[8] * m_V[5];
      Real det = m_V[0] * cofactor0 + m_V[1] * cofactor1 + m_V[2] * cofactor2;
      if (Dg::IsZero(det))
      {
        return result;
      }

      // create adjunct matrix and multiply by 1/det to get upper 3x3
      Real invDet = static_cast<Real>(1.0) / det;
      result.m_V[0] = invDet*cofactor0;
      result.m_V[4] = invDet*cofactor1;
      result.m_V[8] = invDet*cofactor2;

      result.m_V[1] = invDet*(m_V[2] * m_V[9] - m_V[1] * m_V[10]);
      result.m_V[5] = invDet*(m_V[0] * m_V[10] - m_V[2] * m_V[8]);
      result.m_V[9] = invDet*(m_V[1] * m_V[8] - m_V[0] * m_V[9]);

      result.m_V[2] = invDet*(m_V[1] * m_V[6] - m_V[2] * m_V[5]);
      result.m_V[6] = invDet*(m_V[2] * m_V[4] - m_V[0] * m_V[6]);
      result.m_V[10] = invDet*(m_V[0] * m_V[5] - m_V[1] * m_V[4]);

      // multiply -translation by inverted 3x3 to get its inverse

      result.m_V[12] = -result.m_V[0] * m_V[12] - result.m_V[1] * m_V[13] - result.m_V[2] * m_V[14];
      result.m_V[13] = -result.m_V[4] * m_V[12] - result.m_V[5] * m_V[13] - result.m_V[6] * m_V[14];
      result.m_V[14] = -result.m_V[8] * m_V[12] - result.m_V[9] * m_V[13] - result.m_V[10] * m_V[14];

      return result;
    }	//End: Matrix_generic::GetAffineInverse()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Translation()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3>& Matrix_generic<Real, 3>::Translation(Matrix<1, 4, Real> const & a_xlate)
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = static_cast<Real>(1.0);
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = static_cast<Real>(1.0);
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = a_xlate[0];
      m_V[13] = a_xlate[1];
      m_V[14] = a_xlate[2];
      m_V[15] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Translation()


    //----------------------------------------------------------------------------
    //	@	Matrix_generic::Rotation()
    //----------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Rotation(Real a_xRotation,
                                                                Real a_yRotation,
                                                                Real a_zRotation,
                                                                EulerOrder a_order)
    {
      Real Cx = static_cast<Real>(cos(a_xRotation));
      Real Sx = static_cast<Real>(sin(a_xRotation));

      Real Cy = static_cast<Real>(cos(a_yRotation));
      Real Sy = static_cast<Real>(sin(a_yRotation));

      Real Cz = static_cast<Real>(cos(a_zRotation));
      Real Sz = static_cast<Real>(sin(a_zRotation));


      m_V[12] = m_V[13] = m_V[14] = m_V[3] = m_V[7] = m_V[11] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      switch (a_order)
      {
      case EulerOrder::XYZ:
      {
        m_V[0] = Cy * Cz;
        m_V[1] = Cy * Sz;
        m_V[2] = -Sy;

        m_V[4] = (Sx * Sy * Cz) - (Cx * Sz);
        m_V[5] = (Sx * Sy * Sz) + (Cx * Cz);
        m_V[6] = Sx * Cy;

        m_V[8] = (Cx * Sy * Cz) + (Sx * Sz);
        m_V[9] = (Cx * Sy * Sz) - (Sx * Cz);
        m_V[10] = (Cx * Cy);
        break;
      }
      case EulerOrder::XZY:
      {
        m_V[0] = Cy * Cz;
        m_V[1] = Sz;
        m_V[2] = -(Sy * Cz);

        m_V[4] = (Sy * Sx) - (Sz * Cx * Cy);
        m_V[5] = Cx * Cz;
        m_V[6] = (Sz * Sy * Cx) + (Sx * Cy);

        m_V[8] = (Sy * Cx) + (Sz * Sx * Cy);
        m_V[9] = -(Sx * Cz);
        m_V[10] = (Cx * Cy) - (Sz * Sy * Sx);
        break;
      }
      case EulerOrder::YXZ:
      {
        m_V[0] = (Cy * Cz) - (Sx * Sy * Sz);
        m_V[1] = (Sz * Cy) + (Sx * Sy * Cz);
        m_V[2] = -(Sy * Cx);

        m_V[4] = -(Sz * Cx);
        m_V[5] = Cx * Cz;
        m_V[6] = Sx;

        m_V[8] = (Sz * Sx * Cy) + (Sy * Cz);
        m_V[9] = (Sz * Sy) - (Sx * Cy * Cz);
        m_V[10] = Cx * Cy;
        break;
      }
      case EulerOrder::YZX:
      {
        m_V[0] = Cy * Cz;
        m_V[1] = (Sy * Sx) + (Sz * Cx * Cy);
        m_V[2] = (Sz * Sx * Cy) - (Sy * Cx);

        m_V[4] = -Sz;
        m_V[5] = Cx * Cz;
        m_V[6] = Sx * Cz;

        m_V[8] = Sy * Cz;
        m_V[9] = (Sz * Sy * Cx) - (Sx * Cy);
        m_V[10] = (Sz * Sy * Sx) + (Cx * Cy);
        break;
      }
      case EulerOrder::ZYX:
      {
        m_V[0] = Cy * Cz;
        m_V[1] = (Sz * Cx) + (Sy * Sx * Cz);
        m_V[2] = (Sz * Sx) - (Sy * Cx * Cz);

        m_V[4] = -(Sz * Cy);
        m_V[5] = (Cx * Cz) - (Sz * Sy * Sx);
        m_V[6] = (Sz * Sy * Cx) + (Sx * Cz);

        m_V[8] = Sy;
        m_V[9] = -(Sx * Cy);
        m_V[10] = Cx * Cy;
        break;
      }
      case EulerOrder::ZXY:
      {
        m_V[0] = (Sz * Sy * Sx) + (Cy * Cz);
        m_V[1] = Sz * Cx;
        m_V[2] = (Sz * Sx * Cy) - (Sy * Cz);

        m_V[4] = (Sy * Sx * Cz) - (Sz * Cy);
        m_V[5] = Cx * Cz;
        m_V[6] = (Sz * Sy) + (Sx * Cy * Cz);

        m_V[8] = Sy * Cx;
        m_V[9] = -Sx;
        m_V[10] = Cx * Cy;
        break;
      }
      case EulerOrder::XYX:
      {
        m_V[0] = Cy;
        m_V[1] = Sy * Sx;
        m_V[2] = -Sy * Cx;

        m_V[4] = Sy * Sx;
        m_V[5] = (Cx * Cx) - (Sx * Sx * Cy);
        m_V[6] = (Sx * Cx) + (Sx * Cy * Cx);

        m_V[8] = Sy * Cx;
        m_V[9] = -(Sx * Cx) - (Sx * Cy * Cx);
        m_V[10] = (Cx * Cx * Cy) - (Sx * Sx);
        break;
      }
      case EulerOrder::XZX:
      {
        m_V[0] = Cy;
        m_V[1] = Sy * Cz;
        m_V[2] = Sz * Sy;

        m_V[4] = -(Sy * Cx);
        m_V[5] = (Cx * Cy * Cz) - (Sz * Sx);
        m_V[6] = (Sx * Cz) + (Sz * Cy * Cx);

        m_V[8] = (Sy * Sx);
        m_V[9] = -(Sz * Cx) - (Sx * Cz * Cy);
        m_V[10] = (Cx * Cz) - (Sz * Sx * Cy);
        break;
      }
      case EulerOrder::YXY:
      {
        m_V[0] = (Cx * Cz) - (Sz * Sx * Cy);
        m_V[1] = Sy * Sx;
        m_V[2] = -(Sz * Cx) - (Sx * Cy * Cz);

        m_V[4] = Sz * Sy;
        m_V[5] = Cy;
        m_V[6] = Sy * Cz;

        m_V[8] = (Sz * Cx * Cy) + (Sx * Cz);
        m_V[9] = -(Sy * Cx);
        m_V[10] = (Cx * Cy * Cz) - (Sz * Sx);
        break;
      }
      case EulerOrder::YZY:
      {
        m_V[0] = (Cx * Cy * Cz) - (Sz * Sx);
        m_V[1] = Sy * Cx;
        m_V[2] = -(Sz * Cx * Cy) - (Sx * Cz);

        m_V[4] = -(Sy * Cz);
        m_V[5] = Cy;
        m_V[6] = Sz * Sy;

        m_V[8] = (Sz * Cx) + (Sx * Cy * Cz);
        m_V[9] = Sy * Sx;
        m_V[10] = (Cx * Cz) - (Sz * Sx * Cy);
        break;
      }
      case EulerOrder::ZXZ:
      {
        m_V[0] = (Cx * Cz) - (Sz * Sx * Cy);
        m_V[1] = (Sz * Cx) + (Sx * Cy * Cz);
        m_V[2] = Sy * Sx;

        m_V[4] = -(Sz * Cx * Cy) - (Sx * Cz);
        m_V[5] = (Cx * Cy * Cz) - (Sz * Sx);
        m_V[6] = Sy * Cx;

        m_V[8] = Sz * Sy;
        m_V[9] = -(Sy * Cz);
        m_V[10] = Cy;
        break;
      }
      case EulerOrder::ZYZ:
      {
        m_V[0] = (Cx * Cy * Cz) - (Sz * Sx);
        m_V[1] = (Sz * Cx * Cy) + (Sx * Cz);
        m_V[2] = -(Sy * Cx);

        m_V[4] = -(Sz * Cx) - (Sx * Cy * Cz);
        m_V[5] = (Cx * Cz) - (Sz * Sx * Cy);
        m_V[6] = Sy * Sx;

        m_V[8] = Sy * Cz;
        m_V[9] = Sz * Sy;
        m_V[10] = Cy;
        break;
      }
      }

      return *this;

    }  // End: Matrix_generic::Rotation()


    //----------------------------------------------------------------------------
    //	@	Matrix_generic::Rotation()
    //----------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Rotation(Matrix<1, 4, Real> const & a_axis,
                                                                Real a_angle)
    {
      Real cs = cos(a_angle);
      Real sn = sin(a_angle);
      Real oneMinusCos = ((Real)1) - cs;
      Real x0sqr = a_axis.m_V[0] * a_axis.m_V[0];
      Real x1sqr = a_axis.m_V[1] * a_axis.m_V[1];
      Real x2sqr = a_axis.m_V[2] * a_axis.m_V[2];
      Real x0x1m = a_axis.m_V[0] * a_axis.m_V[1] * oneMinusCos;
      Real x0x2m = a_axis.m_V[0] * a_axis.m_V[2] * oneMinusCos;
      Real x1x2m = a_axis.m_V[1] * a_axis.m_V[2] * oneMinusCos;
      Real x0Sin = a_axis.m_V[0] * sn;
      Real x1Sin = a_axis.m_V[1] * sn;
      Real x2Sin = a_axis.m_V[2] * sn;

      m_V[0] = x0sqr*oneMinusCos + cs;
      m_V[4] = x0x1m - x2Sin;
      m_V[8] = x0x2m + x1Sin;
      m_V[1] = x0x1m + x2Sin;
      m_V[5] = x1sqr*oneMinusCos + cs;
      m_V[9] = x1x2m - x0Sin;
      m_V[2] = x0x2m - x1Sin;
      m_V[6] = x1x2m + x0Sin;
      m_V[10] = x2sqr*oneMinusCos + cs;

      m_V[3] = m_V[7] = m_V[11] = m_V[12] = m_V[13] = m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);
      return *this;

    }  // End: Matrix_generic::Rotation()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Scaling()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Scaling(Matrix<1, 4, Real> const & a_scaleFactors)
    {
      m_V[0] = a_scaleFactors.m_V[0];
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = a_scaleFactors.m_V[1];
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = a_scaleFactors.m_V[2];
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Scaling()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Scaling()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Scaling(Real a_val)
    {
      m_V[0] = a_val;
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = a_val;
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = a_val;
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Scaling()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::RotationX()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3>& Matrix_generic<Real, 3>::RotationX(Real a_angle)
    {
      Real sintheta = Real(sin(a_angle));
      Real costheta = Real(cos(a_angle));

      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = costheta;
      m_V[6] = sintheta;
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = -sintheta;
      m_V[10] = costheta;
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix_generic::RotationX()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::RotationY()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::RotationY(Real a_angle)
    {
      Real sintheta = Real(sin(a_angle));
      Real costheta = Real(cos(a_angle));

      m_V[0] = costheta;
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = -sintheta;
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = static_cast<Real>(1.0);
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = sintheta;
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = costheta;
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix_generic::RotationY()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::RotationZ()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::RotationZ(Real a_angle)
    {
      Real sintheta = Real(sin(a_angle));
      Real costheta = Real(cos(a_angle));

      m_V[0] = costheta;
      m_V[1] = sintheta;
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = -sintheta;
      m_V[5] = costheta;
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = static_cast<Real>(1.0);
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::RotationZ()


    //-------------------------------------------------------------------------------
    //	@	Rotation()
    //-------------------------------------------------------------------------------
    template<typename Real>
    R3::Quaternion<Real> Matrix_generic<Real, 3>::GetQuaternion() const
    {
      R3::Quaternion<Real> q;

      //Get trace
      Real tr = m_V[0] + m_V[5] + m_V[10];
      if (tr > static_cast<Real>(0.0))
      {
        Real S = sqrt(tr + static_cast<Real>(1.0)) * static_cast<Real>(2.0); // S=4*q.w() 
        q.w() = static_cast<Real>(0.25) * S;
        q.x() = (m_V[6] - m_V[9]) / S;
        q.y() = (m_V[8] - m_V[2]) / S;
        q.z() = (m_V[1] - m_V[4]) / S;
      }
      else if ((m_V[0] > m_V[5])&(m_V[0] > m_V[10]))
      {
        Real S = sqrt(static_cast<Real>(1.0) + m_V[0] - m_V[5] - m_V[10]) * static_cast<Real>(2.0); // S=4*q.x() 
        q.w() = (m_V[6] - m_V[9]) / S;
        q.x() = static_cast<Real>(0.25) * S;
        q.y() = (m_V[4] + m_V[1]) / S;
        q.z() = (m_V[8] + m_V[2]) / S;
      }
      else if (m_V[5] > m_V[10])
      {
        Real S = sqrt(static_cast<Real>(1.0) + m_V[5] - m_V[0] - m_V[10]) * static_cast<Real>(2.0); // S=4*q.y()
        q.w() = (m_V[8] - m_V[2]) / S;
        q.x() = (m_V[4] + m_V[1]) / S;
        q.y() = static_cast<Real>(0.25) * S;
        q.z() = (m_V[9] + m_V[6]) / S;
      }
      else
      {
        Real S = sqrt(static_cast<Real>(1.0) + m_V[10] - m_V[0] - m_V[5]) * static_cast<Real>(2.0); // S=4*q.z()
        q.w() = (m_V[1] - m_V[4]) / S;
        q.x() = (m_V[8] + m_V[2]) / S;
        q.y() = (m_V[9] + m_V[6]) / S;
        q.z() = static_cast<Real>(0.25) * S;
      }

      return q;
    }   // End of GetQuaternion()



    //-------------------------------------------------------------------------------
    //	@	Rotation()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Rotation(R3::Quaternion<Real> const & a_rotate)
    {
      //assert(a_rotate.IsUnit());

      Real xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

      xs = a_rotate.x() + a_rotate.x();
      ys = a_rotate.y() + a_rotate.y();
      zs = a_rotate.z() + a_rotate.z();
      wx = a_rotate.w() * xs;
      wy = a_rotate.w() * ys;
      wz = a_rotate.w() * zs;
      xx = a_rotate.x() * xs;
      xy = a_rotate.x() * ys;
      xz = a_rotate.x() * zs;
      yy = a_rotate.y() * ys;
      yz = a_rotate.y() * zs;
      zz = a_rotate.z() * zs;

      m_V[0] = static_cast<Real>(1.0) - (yy + zz);
      m_V[1] = xy + wz;
      m_V[2] = xz - wy;
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = xy - wz;
      m_V[5] = static_cast<Real>(1.0) - (xx + zz);
      m_V[6] = yz + wx;
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = xz + wy;
      m_V[9] = yz - wx;
      m_V[10] = static_cast<Real>(1.0) - (xx + yy);
      m_V[11] = static_cast<Real>(0.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = static_cast<Real>(0.0);
      m_V[15] = static_cast<Real>(1.0);

      return *this;
    }   // End of Matrix_generic::Rotation()


    //! Get a matrix orientated at the target
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::LookAt(
      Vector_generic<Real, 3> const & a_origin,
      Vector_generic<Real, 3> const & a_target,
      Vector_generic<Real, 3> const & a_up)
    {
      Vector_generic<Real, 3> forward = a_target - a_origin;
      if (Dg::IsZero(forward.LengthSquared()))
      {
        forward = Vector_generic<Real, 3>::xAxis();
      }

      Vector_generic<Real, 3> right = forward.Cross(a_up);
      if (Dg::IsZero(right.LengthSquared()))
      {
        right = forward.Perpendicular();
      }

      Vector_generic<Real, 3> camUp = forward.Cross(right);

      SetRows(right, camUp, forward, a_origin);
      return *this;
    }   // End: LookAt()


    //! Get perspctive projection matrix
    template<typename Real>
    Matrix_generic<Real, 3> & Matrix_generic<Real, 3>::Perspective(
      Real a_fov,
      Real a_ar,
      Real a_near,
      Real a_far)
    {
      Real d = static_cast<Real>(1.0) / tan(static_cast<Real>(0.5) * a_fov);
      Real A = d / a_ar;
      Real B = (a_near + a_far) / (a_near - a_far);
      Real C = (static_cast<Real>(2.0) * a_near * a_far) / (a_near - a_far);

      m_V[0] = A;
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(0.0);
      m_V[5] = d;
      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(0.0);
      m_V[9] = static_cast<Real>(0.0);
      m_V[10] = B;
      m_V[11] = static_cast<Real>(-1.0);
      m_V[12] = static_cast<Real>(0.0);
      m_V[13] = static_cast<Real>(0.0);
      m_V[14] = C;
      m_V[15] = static_cast<Real>(0.0);

      return *this;
    }	//End: Perspective()
  }
}
#endif