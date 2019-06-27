//! @file DgR2Matrix_generic.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix_generic

#ifndef DGR2MATRIX_GENERIC_H
#define DGR2MATRIX_GENERIC_H

#include "DgMatrix_generic.h"
#include "../DgMath.h"
#include "../DgMatrix.h"

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
    //!     Memory layout: { x.x, x.y, 0, y.x, y.y 0, p.x, p.y, 1 }
    //!     Row major: [x.x x.y 0]
    //!                [y.x y.y 0]
    //!                [p.x p.y 1]
    //!
    //! Matrix_generic concatenation uses left-to-right convention. For example the follow lines are equivalent
    //!
    //!     m_final = m_0 * m_1 * m_2 ... * m_n;
    //!     m_final = ( ... ((m_0 * m_1) * m_2) ... * m_n);
    //!
    //! @author James M. Van Verth, Lars M. Bishop, modified by Frank B. Hart
    //! @date 4/10/2015
    template<typename Real>
    class Matrix_generic<Real, 2> : public Matrix <3, 3, Real>
    {
    public:
      //! Default constructor, initialized to identity matrix.
      Matrix_generic() { Identity(); }
      ~Matrix_generic() {}

      //! Copy Constructor
      Matrix_generic(Matrix<3, 3, Real> const & a_other) 
        : Matrix<3, 3, Real>(a_other) {}

      //! Assignment
      Matrix_generic & operator=(Matrix <3, 3, Real> const &);

      //! Set matrix by rows
      void SetRows(Matrix<1, 3, Real> const & row0, 
                   Matrix<1, 3, Real> const & row1,
                   Matrix<1, 3, Real> const & row2);

      //! Get rows of the matrix
      void GetRows(Matrix<1, 3, Real> & row0, 
                   Matrix<1, 3, Real> & row1,
                   Matrix<1, 3, Real> & row2) const;

      //! Set matrix by columns
      void SetColumns(Matrix<3, 1, Real> const & col0, 
                      Matrix<3, 1, Real> const & col1,
                      Matrix<3, 1, Real> const & col2);

      //! Get matrix columns
      void GetColumns(Matrix<3, 1, Real> & col0, 
                      Matrix<3, 1, Real> & col1,
                      Matrix<3, 1, Real> & col2) const;

      //! Sets self to inverse
      //!
      //! @return Self if no inverse exists.
      Matrix_generic & SetInverse();

      //! Returns inverse of this matrix
      //!
      //! @return Self if no inverse exists.
      Matrix_generic GetInverse() const;

      //! Set as translation matrix based on vector
      Matrix_generic & Translation(Matrix<1, 3, Real> const &);

      //! Sets the matrix to a rotation matrix.
      Matrix_generic & Rotation(Real);

      //! Set as scaling matrix based on vector.
      Matrix_generic & Scaling(Matrix<1, 3, Real> const &);

      //! Uniform scaling.
      Matrix_generic & Scaling(Real);
    };


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> & Matrix_generic<Real, 2>::operator=(Matrix<3, 3, Real> const & a_other)
    {
      Matrix<3, 3, Real>::operator=(a_other);
      return *this;
    }

    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::SetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 2>::SetRows(Matrix<1, 3, Real> const & a_row0,
                                          Matrix<1, 3, Real> const & a_row1,
                                          Matrix<1, 3, Real> const & a_row2)
    {
      SetRow(0, a_row0);
      SetRow(1, a_row1);
      SetRow(2, a_row2);
    }	//End: Matrix_generic::SetRows()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::GetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 2>::GetRows(Matrix<1, 3, Real> & a_row0, 
                                          Matrix<1, 3, Real> & a_row1,
                                          Matrix<1, 3, Real> & a_row2) const
    {
      GetRow(0, a_row0);
      GetRow(1, a_row1);
      GetRow(2, a_row2);
    }	//End: Matrix_generic::GetRows()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::SetColumns()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 2>::SetColumns(Matrix<3, 1, Real> const & a_col0, 
                                             Matrix<3, 1, Real> const & a_col1,
                                             Matrix<3, 1, Real> const & a_col2)
    {
      SetColumn(0, a_col0);
      SetColumn(1, a_col1);
      SetColumn(2, a_col2);
    }	//End: Matrix_generic::SetColumns()


    //--------------------------------------------------------------------------------
    //	@	Matrix_generic::GetColumns()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix_generic<Real, 2>::GetColumns(Matrix<3, 1, Real> & a_col0,
                                             Matrix<3, 1, Real> & a_col1,
                                             Matrix<3, 1, Real> & a_col2) const
    {
      GetColumn(0, a_col0);
      GetColumn(1, a_col1);
      GetColumn(2, a_col2);
    }	//End: Matrix_generic::GetColumns()


    //--------------------------------------------------------------------------------
    //	@	SetInverse()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> & Matrix_generic<Real, 2>::SetInverse()
    {
      *this = GetInverse(*this);
      return *this;
    }	//End: ::SetInverse()


    //--------------------------------------------------------------------------------
    //	@	GetInverse()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> Matrix_generic<Real, 2>::GetInverse() const
    {
      Matrix_generic<Real, 2> result;

      //compute upper left 3x3 matrix determinant
      Real cofactor0 = m_V[4] * m_V[8] - m_V[7] * m_V[5];
      Real cofactor1 = m_V[5] * m_V[6] - m_V[3] * m_V[8];
      Real cofactor2 = m_V[3] * m_V[7] - m_V[6] * m_V[4];
      Real det = m_V[0] * cofactor0 + m_V[1] * cofactor1 + m_V[2] * cofactor2;
      if (Dg::IsZero(det))
      {
        return result;
      }

      // create adjunct matrix and multiply by 1/det to get upper 3x3
      Real invDet = static_cast<Real>(1.0) / det;
      result.m_V[0] = invDet*cofactor0;
      result.m_V[3] = invDet*cofactor1;
      result.m_V[6] = invDet*cofactor2;

      result.m_V[1] = invDet*(m_V[2] * m_V[7] - m_V[1] * m_V[8]);
      result.m_V[4] = invDet*(m_V[0] * m_V[8] - m_V[2] * m_V[6]);
      result.m_V[7] = invDet*(m_V[1] * m_V[6] - m_V[0] * m_V[7]);

      result.m_V[2] = invDet*(m_V[1] * m_V[5] - m_V[2] * m_V[4]);
      result.m_V[5] = invDet*(m_V[2] * m_V[3] - m_V[0] * m_V[5]);
      result.m_V[8] = invDet*(m_V[0] * m_V[4] - m_V[1] * m_V[3]);

      return result;

    }	//End: ::GetInverse()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Translation()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> & Matrix_generic<Real, 2>::Translation(Matrix<1, 3, Real> const & a_xlate)
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(1.0);
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = a_xlate[0];
      m_V[7] = a_xlate[1];
      m_V[8] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Translation()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Scaling()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> & Matrix_generic<Real, 2>::Scaling(Matrix<1, 3, Real> const & a_scaleFactors)
    {
      m_V[0] = a_scaleFactors.m_V[0];
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = a_scaleFactors.m_V[1];
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Scaling()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Scaling()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2> & Matrix_generic<Real, 2>::Scaling(Real a_val)
    {
      m_V[0] = a_val;
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = a_val;
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Scaling()


    //-------------------------------------------------------------------------------
    //	@	Matrix_generic::Rotation()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix_generic<Real, 2>& Matrix_generic<Real, 2>::Rotation(Real a_angle)
    {
      Real sintheta = Real(sin(a_angle));
      Real costheta = Real(cos(a_angle));

      m_V[0] = costheta;
      m_V[1] = sintheta;
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = -sintheta;
      m_V[4] = costheta;
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;
    }   // End: Matrix_generic::Rotation()
  }
}
#endif