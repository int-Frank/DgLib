//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGSEGMENT_GENERAL_H
#define DGSEGMENT_GENERAL_H

#include "DgError.h"
#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Segment
  //!
  //! A Linesgment is a line that connects two points. It is represented by a Point
  //! and a Vector, the length of the vector being the distance between the points.
  //! This file also declares methods to test LineSegments against other 
  //! geometric entities.
  template<typename Real, int R>
  class Segment
  {
  public:

    Segment();
    Segment(Vector<Real, R> const &p0, Vector<Real, R> const &p1);
    Segment(Segment const &);
    Segment& operator=(Segment const &);

    Vector<Real, R> const & Origin() const { return m_origin; }
    Vector<Real, R> const & Vect() const { return m_vector; }

    Vector<Real, R> GetP0() const { return m_origin; }
    Vector<Real, R> GetP1() const { return m_origin + m_vector; }
    void Get(Vector<Real, R> &p0, Vector<Real, R> &p1) const;

    Vector<Real, R> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_vector; }

    ErrorCode Set(Vector<Real, R> const & p0, Vector<Real, R> const & p1);

    Real Length() const;
    Real LengthSquared() const;

  private:

    Vector<Real, R> m_origin;
    Vector<Real, R> m_vector;
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using Segment2 = Segment<Real, 2>;

  template<typename Real>
  using Segment3 = Segment<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  Segment<Real, R>::Segment()
    : m_origin(Zeros<Real, R>())
    , m_vector(XAxis<Real, R>())
  {
  }

  template<typename Real, int R>
  Segment<Real, R>::Segment(Vector<Real, R> const &p0, Vector<Real, R> const &p1)
    : m_origin(Zeros<Real, R>())
    , m_vector(XAxis<Real, R>())
  {
    Set(p0, p1);
  }

  template<typename Real, int R>
  Segment<Real, R>::Segment(Segment<Real, R> const & a_other)
    : m_origin(a_other.m_origin)
    , m_vector(a_other.m_vector)
  {
  }

  template<typename Real, int R>
  Segment<Real, R> & Segment<Real, R>::operator=(Segment<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_vector = a_other.m_vector;

    return *this;
  }

  template<typename Real, int R>
  void Segment<Real, R>::Get(Vector<Real, R> &p0, Vector<Real, R> &p1) const
  {
    p0 = m_origin;
    p1 = m_origin + m_vector;
  }

  template<typename Real, int R>
  ErrorCode Segment<Real, R>::Set(Vector<Real, R> const & a_p0, Vector<Real, R> const & a_p1)
  {
    ErrorCode result;

    DG_ERROR_IF(a_p0 == a_p1, ErrorCode::InvalidInput);

    m_origin = a_p0;
    m_vector = a_p1 - a_p0;

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<typename Real, int R>
  Real Segment<Real, R>::Length() const
  {
    return Mag(m_vector);
  }

  template<typename Real, int R>
  Real Segment<Real, R>::LengthSquared() const
  {
    return MagSq(m_vector);

  }
}
#endif