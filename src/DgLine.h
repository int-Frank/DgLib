//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGLINE_H
#define DGLINE_H

#include "DgError.h"
#include "DgVector.h"
#include "DgMath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Line
  //!
  //! A Line is defined by a point and direction extending infinitely in both
  //! directions. It is represented by a point and a normalized vector. This file
  //! also declares methods to test Lines against other geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 29/05/2016
  template<typename Real, int R>
  class Line
  {
  public:

    Line();
    Line(Vector<Real, R> const &origin, Vector<Real, R> const &direction);
    Line(Line const &);
    Line & operator=(Line const &);

    Vector<Real, R> const & Origin() const { return m_origin; }
    Vector<Real, R> const & Direction() const { return m_direction; }
    
    ErrorCode Set(Vector<Real, R> const & origin, Vector<Real, R> const & direction);

  private:

    Vector<Real, R> m_origin;
    Vector<Real, R> m_direction;
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using Line2 = Line<Real, 2>;

  template<typename Real>
  using Line3 = Line<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  Line<Real, R>::Line()
    : m_origin(Zeros<Real, R>())
    , m_direction(XAxis<Real, R>())
  {
  }

  template<typename Real, int R>
  Line<Real, R>::Line(Vector<Real, R> const &origin, Vector<Real, R> const &direction)
    : m_origin(origin)
    , m_direction(direction)
  {
    Set(origin, direction);
  }

  template<typename Real, int R>
  Line<Real, R>::Line(Line<Real, R> const &a_other)
    : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }

  template<typename Real, int R>
  Line<Real, R> & Line<Real, R>::operator=(Line<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }

  template<typename Real, int R>
  ErrorCode Line<Real, R>::Set(Vector<Real, R> const & a_origin, Vector<Real, R> const & a_direction)
  {
    m_origin = a_origin;
    m_direction = Normalize(a_direction);
    return ErrorCode::None;
  }
}

#endif