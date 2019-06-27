//! @file DgMatrix.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGMATRIX_H
#define DGMATRIX_H

#include "DgMath.h"

namespace Dg
{
  namespace impl
  {
    template<typename Real, int R>
    class Matrix_generic;

    template<typename Real, int R>
    class Vector_generic;
  }

  template<size_t M, size_t N, typename Real> class Matrix;

  //! Compute the transpose matrix.
  template<size_t M, size_t N, typename Real>
  Matrix<N, M, Real> Transpose(Matrix<M, N, Real> const &);

  //! Element-wise dot product.
  template<size_t M, size_t N, typename Real>
  Real Dot(Matrix<M, N, Real> const &, Matrix<M, N, Real> const &);

  //! Scalar multiplication
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> operator*(Real, Matrix<M, N, Real> const &);

  namespace impl
  {
    //! Recursive function which finds the determinant of a matrix.
    //! Not the most efficient algorithm O(n!).
    //!
    //! @return value
    template<size_t M, size_t N, typename Real>
    Real Determinant(Matrix<M, N, Real> const &);

    //!Finds the determinant of a 2x2 matrix.
    template<typename Real>
    Real Determinant(Matrix<2, 2, Real> const &);

    //!Finds the determinant of a 1x1 matrix.
    template<typename Real>
    Real Determinant(Matrix<1, 1, Real> const &);
  }

  //! @ingroup DgMath_types
  //!
  //! @class Matrix
  //!
  //! @brief Generic two dimension matrix class.
  //!
  //! @author Frank B. Hart
  //! @date 4/10/2015
  template<size_t M, size_t N, typename Real>
  class Matrix
  {
    static_assert(M > 0 && N > 0, "Matrix cannot have a zero dimension.");

    template<size_t _M, size_t _N, typename _Real> friend class Matrix;
    template<typename _Real, int R> friend class impl::Matrix_generic;
    template<typename _Real, int R> friend class impl::Vector_generic;

  public:
    //! Default constructor. Elements are not initialised.
    Matrix();
    ~Matrix() {}

    //! Copy constructor
    Matrix(Matrix const &);

    //! Assignment
    Matrix& operator=(Matrix const &);

    //! Accessor m: row, n:column.
    Real& operator()(size_t m, size_t n);

    //! Accessor m: row, n:column.
    Real operator()(size_t m, size_t n) const;

    //! Accessor element by index.
    Real & operator[](size_t i) { return m_V[i]; }

    //! Accessor element by index.
    Real operator[](size_t i) const { return m_V[i]; }

    //! Comparison
    bool operator== (Matrix const &) const;

    //! Comparison
    bool operator!= (Matrix const &) const;

    //! Checks if all elements are below the tolerance.
    bool IsZero() const;

    //! Set all elements to zero.
    void Zero();

    //! Checks if matrix is equal to the identity, within some tolerance.
    bool IsIdentity() const;

    //! Directly set the elements.
    void Set(Real const [M * N]);

    //! Get a submatrix
    template<size_t _M, size_t _N>
    Matrix<_M, _N, Real> GetSubMatrix(size_t m0, size_t n0) const;

    //! Set a row in the matrix
    void SetRow(size_t m, Matrix<1, N, Real> const &);

    //! Get a row in the matrix
    void GetRow(size_t m, Matrix<1, N, Real>&) const;

    //! Set a column in the matrix
    void SetColumn(size_t n, Matrix<M, 1, Real> const &);

    //! Get a column in the matrix
    void GetColumn(size_t n, Matrix<M, 1, Real>&) const;

    //! Sets near-zero elements to 0.
    void Clean();

    //! Set to identity matrix. For aquare martices only.
    void Identity();

    //! Compute the transpose matrix.
    template<size_t _M, size_t _N, typename _Real>
    friend Matrix<_N, _M, _Real> Transpose(Matrix<_M, _N, _Real> const &);

    //! Set matrix to its transpose. For square matrices only.
    Matrix& Transpose();

    //! Uses recursive algorithm.
    Real Determinant() const;

    //! Element-wise dot product.
	  Real Dot(Matrix const &) const;

    //! Matrix-matrix addition
    Matrix operator+ (Matrix const &) const;

    //! Matrix-matrix addition, assign to self
    Matrix& operator+= (Matrix const &);

    //! Matrix-matrix subtraction
    Matrix operator- (Matrix const &) const;

    //! Matrix-matrix subtraction, assign to self
    Matrix& operator-= (Matrix const &);

    //! Negate matrix.
    Matrix operator-() const;

    //! Matrix-matrix multiplication, assign to self
    Matrix& operator*= (Matrix const &);

    //! Matrix-matrix multiplication
    template<size_t _M>
    Matrix<M, _M, Real> operator* (Matrix<N, _M, Real> const &) const;

    //! Matrix-scalar multiplication, assign to self
    Matrix& operator*= (Real);

    //! Matrix-scalar multiplication
    Matrix operator* (Real) const;

    //! Matrix-scalar division, assign to self
    Matrix& operator/= (Real);

    //! Matrix-scalar division
    Matrix operator/ (Real) const;

    //! Scalar multiplication
    friend Matrix operator*<M, N, Real> (Real, Matrix const &);

    //! Element-wise product
    Matrix ElementwiseProduct(Matrix const &) const;

    //! Element-wise product, asign to self
    Matrix & ElementwiseProductSelf(Matrix const &);

    //! Get pointer to internal data
    Real * GetData() { return &m_V[0]; }

    //! Get pointer to internal data
    Real const * GetData() const { return &m_V[0]; }

  protected:

    //! Data
    Real m_V[M * N];

  };

  //--------------------------------------------------------------------------------
  //	@	Matrix::Matrix()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>::Matrix()
  {
  }	//End: Matrix::Matrix()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real& Matrix<M, N, Real>::operator()(size_t a_m, size_t a_n)
  {
    return m_V[a_m * N + a_n];

  }	//End: Matrix::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real Matrix<M, N, Real>::operator()(size_t a_m, size_t a_n) const
  {
    return m_V[a_m * N + a_n];

  }	//End: Matrix::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator=()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator= (Matrix<M, N, Real>const & a_other)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] = a_other.m_V[i];
    }

    return *this;

  }	//End: Matrix::operator=()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Matrix()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>::Matrix(Matrix<M, N, Real> const & a_other)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] = a_other.m_V[i];
    }

  }	//End: Matrix::Matrix()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator==()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  bool Matrix<M, N, Real>::operator== (Matrix<M, N, Real> const & a_other) const
  {
    for (size_t i = 0; i < M * N; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix::operator==()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator!=()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  bool Matrix<M, N, Real>::operator!= (Matrix<M, N, Real> const & a_other) const
  {
    for (size_t i = 0; i < M * N; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return true;
    }

    return false;

  }	//End: Matrix::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44Dg::Zero()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::Zero()
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] = static_cast<Real>(0.0);
    }

  }	//End: Matrix::Zero()


  //--------------------------------------------------------------------------------
  //	@	Matrix44Dg::IsZero()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  bool Matrix<M, N, Real>::IsZero() const
  {
    for (size_t i = 0; i < M * N; i++)
    {
      if (!Dg::IsZero(m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix::IsZero()


  //--------------------------------------------------------------------------------
  //	@	Matrix::IsIdentity()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  bool Matrix<M, N, Real>::IsIdentity() const
  {
    static_assert(M == N, "Only square matrices can be identity");

    for (size_t m = 0; m < M - 1; ++m)
    {
      for (size_t n = m + 1; n < N; ++n)
      {
        if (!Dg::IsZero(m_V[m * N + n]) || !Dg::IsZero(m_V[n * M + m]))
        {
          return false;
        }
      }
    }

    for (size_t i = 0; i < M * N; i += (N + 1))
    {
      if (!Dg::IsZero(static_cast<Real>(1.0) - m_V[i]))
      {
        return false;
      }
    }

    return true;

  }	//End: Matrix::IsIdentity()


  //--------------------------------------------------------------------------------
  //	@	Matrix::GetSubMatrix()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  template<size_t _M, size_t _N>
  Matrix<_M, _N, Real> Matrix<M, N, Real>::GetSubMatrix(size_t m0,
                                                        size_t n0) const
  {
    Matrix<_M, _N, Real> result;
    result.Zero();

    size_t Mend = ((m0 + _M) < M) ? (m0 + _M) : M;
    size_t Nend = ((n0 + _N) < N) ? (n0 + _N) : N;

    size_t _m = 0;
    for (size_t m = m0; m < Mend; ++m, ++_m)
    {
      size_t _n = 0;
      for (size_t n = n0; n < Nend; ++n, ++_n)
      {
        result.m_V[_m * _N + _n] = m_V[m * N + n];
      }
    }

    return result;
  }	//End: Matrix::GetSubMatrix()


  //--------------------------------------------------------------------------------
  //	@	Determinant()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real Matrix<M, N, Real>::Determinant() const
  {
    return impl::Determinant(*this);
  }	//End: Determinant()


  //--------------------------------------------------------------------------------
  //	@	Determinant()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real impl::Determinant(Matrix<M, N, Real> const & a_matrix)
  {
    static_assert(M == N, "Can only find determinant of a square matrix");

    Real result = 0;
    Real sign = static_cast<Real>(1.0);

    for (size_t i = 0; i < M; i++)
    {
      Matrix<M - 1, M - 1, Real> subMatrix;

      size_t n_out = 0;
      for (size_t n_in = 0; n_in < M; ++n_in)
      {
        if (n_in == i)
        {
          continue;
        }

        for (size_t m_out = 0; m_out + 1 < M; ++m_out)
        {
          subMatrix[m_out * (M - 1) + n_out] = a_matrix[(m_out + 1) * M + n_in];
        }
        n_out++;
      }

      result += (Determinant(subMatrix) * sign * a_matrix[i]);

      sign *= static_cast<Real>(-1.0);
    }

    return result;
  }	//End: Determinant()


  //--------------------------------------------------------------------------------
  //	@	Determinant()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real impl::Determinant(Matrix<2, 2, Real> const & a_matrix)
  {
    return a_matrix[0] * a_matrix[3] - a_matrix[1] * a_matrix[2];
  }	//End: Determinant()


  //--------------------------------------------------------------------------------
  //	@	Determinant()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real impl::Determinant(Matrix<1, 1, Real> const & a_matrix)
  {
    return a_matrix[0];
  }	//End: Determinant()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Set()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::Set(Real const a_data[M * N])
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] = a_data[i];
    }
  }// End: Matrix<M, N, Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	Matrix::SetRow()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::SetRow(size_t a_m, Matrix<1, N, Real> const & a_row)
  {
    a_m *= N;

    for (size_t i = 0; i < N; i++)
    {
      m_V[a_m +i] = a_row.m_V[i];
    }

  }	//End: Matrix::SetRow()


  //--------------------------------------------------------------------------------
  //	@	Matrix::GetRow()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::GetRow(size_t a_m, Matrix<1, N, Real>& a_out) const
  {
    a_m *= N;

    for (size_t i = 0; i < N; i++)
    {
      a_out.m_V[i] = m_V[a_m + i];
    }

  }	//End: Matrix::GetRow()


  //--------------------------------------------------------------------------------
  //	@	Matrix::SetColumn()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::SetColumn(size_t a_n, Matrix<M, 1, Real> const & a_col)
  {
    for (size_t i = 0; i < M; i++)
    {
      m_V[i * N + a_n] = a_col.m_V[i];
    }

  }	//End: Matrix::SetColumn()


  //--------------------------------------------------------------------------------
  //	@	Matrix::GetColumn()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::GetColumn(size_t a_n, Matrix<M, 1, Real>& a_out) const
  {
    for (size_t i = 0; i < M; i++)
    {
      a_out.m_V[i] = m_V[i * N + a_n];
    }

  }	//End: Matrix::GetColumn()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Clean()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::Clean()
  {
    for (size_t i = 0; i < M * N; i++)
    {
      if (Dg::IsZero(m_V[i]))
        m_V[i] = static_cast<Real>(0.0);
    }

  }	//End: Matrix::Clean()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Indentity()
  //--------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  void Matrix<M, N, Real>::Identity()
  {
    static_assert(M == N, "Only square matrices can be identity");

    for (size_t m = 0; m < M - 1; ++m)
    {
      for (size_t n = m + 1; n < N; ++n)
      {
        m_V[m * N + n] = m_V[n * M + m] = static_cast<Real>(0.0);
      }
    }

    for (size_t i = 0; i < M * N; i += (N + 1))
    {
      m_V[i] = static_cast<Real>(1.0);
    }

  }	//End: Matrix::Identity()


  //--------------------------------------------------------------------------------
  //	@	Transpose()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<N, M, Real> Transpose(Matrix<M, N, Real> const & a_mat)
  {
    Matrix<N, M, Real> result;

    for (size_t m = 0; m < M; ++m)
    {
      for (size_t n = 0; n < N; ++n)
      {
        result.m_V[n * M + m] = a_mat.m_V[m * N + n];
      }
    }

    return result;

  }   // End: Transpose()


  //--------------------------------------------------------------------------------
  //	@	Transpose()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::Transpose()
  {
    static_assert(M == N, "Can only transpose a square matrix");

    for (size_t m = 0; m < M - 1; ++m)
    {
      for (size_t n = m + 1; n < N; ++n)
      {
        Real temp;
        temp = m_V[m * N + n];
        m_V[m * N + n] = m_V[n * N + m];
        m_V[n * N + m] = temp;
      }
    }

    return *this;

  }   // End: Transpose()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real Matrix<M, N, Real>::Dot(Matrix<M, N, Real> const & a_mat) const
  {
	  Real result = static_cast<Real>(0.0);
	  for (size_t i = 0; i < M * N; i++)
	  {
		  result += (a_mat.m_V[i] * m_V[i]);
	  }
	  return result;
  }	// End: Dot()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Real Dot(Matrix<M, N, Real> const & a_mat0, Matrix<M, N, Real> const & a_mat1)
  {
	  return a_mat0.Dot(a_mat1);
  }	// End: Dot()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator+()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator+(Matrix<M, N, Real> const & a_other) const
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = m_V[i] + a_other.m_V[i];
    }

    return result;

  }   // End: Matrix::operator+()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator+=()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator+=(Matrix<M, N, Real> const & a_other)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] += a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix::operator+=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator-(Matrix<M, N, Real> const & other) const
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = m_V[i] - other.m_V[i];
    }

    return result;

  }   // End: Matrix::operator-()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-=()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator-=(Matrix<M, N, Real> const & a_other)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] -= a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix::operator-=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator-() const
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = -m_V[i];
    }

    return result;

  }    // End: Matrix::operator-()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  template<size_t _M>
  Matrix<M, _M, Real> Matrix<M, N, Real>::operator*(Matrix<N, _M, Real> const & a_other) const
  {
    Matrix<M, _M, Real> result;
    result.Zero();

    for (size_t m = 0; m < M; ++m)
    {
      for (size_t _m = 0; _m < _M; ++_m)
      {
        for (size_t n = 0; n < N; ++n)
        {
          result.m_V[m * _M + _m] += (m_V[m * N + n] * a_other.m_V[n * _M + _m]);
        }
      }
    }

    return result;

  }   // End: Matrix::operator*()

  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator*=(Matrix<M, N, Real> const & a_other)
  {
    static_assert(M == N, "Can only assign to self if a square matrix");

    Matrix<M, N, Real> result;
    result.Zero();

    for (size_t m = 0; m < M; ++m)
    {
      for (size_t n = 0; n < N; ++n)
      {
        for (size_t i = 0; i < N; i++)
        {
          result.m_V[m * N + n] += (m_V[m * N + i] * a_other.m_V[i * N + n]);
        }
      }
    }

    Set(result.m_V);

    return *this;

  }   // End: Matrix::operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::ElementwiseProduct()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::ElementwiseProduct(Matrix<M, N, Real> const & a_other) const
  {
    Matrix<M, N, Real> result;
    
    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = m_V[i] * a_other.m_V[i];
    }

    return result;

  }   // End: Matrix::ElementwiseProduct()


  //-------------------------------------------------------------------------------
  //	@	Matrix::ElementwiseProductSelf()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> & Matrix<M, N, Real>::ElementwiseProductSelf(Matrix<M, N, Real> const & a_other)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] *= a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix::ElementwiseProductSelf()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*=()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator*=(Real a_scalar)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] *= a_scalar;
    }

    return *this;

  }  // End: Matrix::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> operator*(Real a_scalar, Matrix<M, N, Real> const & matrix)
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = matrix.m_V[i] * a_scalar;
    }

    return result;

  }  // End: operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator*(Real a_scalar) const
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = m_V[i] * a_scalar;
    }

    return result;

  }  // End: Matrix::operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator/()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator/(Real a_scalar) const
  {
    Matrix<M, N, Real> result;

    for (size_t i = 0; i < M * N; i++)
    {
      result.m_V[i] = m_V[i] / a_scalar;
    }

    return result;

  }  // End: Matrix::operator/()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator/=()
  //-------------------------------------------------------------------------------
  template<size_t M, size_t N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator/=(Real a_scalar)
  {
    for (size_t i = 0; i < M * N; i++)
    {
      m_V[i] /= a_scalar;
    }

    return *this;

  }  // End: Matrix::operator/=()

}


#endif