//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGRAY_H
#define DGRAY_H

#include "DgError.h"
#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Ray
  //!
  //! A Ray is defined by a point of origin and direction extending infinitely in one
  //! direction. It is represented by a Point and a normalized Vector. This file
  //! also declares methods to test Rays against other geometric entities.
  //!
  template<typename Real, int R>
  class Ray
  {
  public:

    Ray();
    Ray(Vector<Real, R> const &origin, Vector<Real, R> const &direction);
    Ray(Ray const &);
    Ray& operator=(Ray const &);

    Vector<Real, R> const & Origin() const { return m_origin; }
    Vector<Real, R> const & Direction() const { return m_direction; }

    //! Set ray from an origin and direction
    ErrorCode Set(Vector<Real, R> const & origin, Vector<Real, R> const & direction);

  private:

    //Data members
    Vector<Real, R> m_origin;
    Vector<Real, R> m_direction;
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using Ray2 = Ray<Real, 2>;

  template<typename Real>
  using Ray3 = Ray<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  Ray<Real, R>::Ray() 
    : m_origin(Zeros<Real, R>())
    , m_direction(XAxis<Real, R>())
  {
  }

  template<typename Real, int R>
  Ray<Real, R>::Ray(Vector<Real, R> const &origin, Vector<Real, R> const &direction)
    : m_origin(origin)
    , m_direction(direction)
  {
    Set(origin, direction);
  }

  template<typename Real, int R>
  Ray<Real, R>::Ray(Ray<Real, R> const & a_other) 
    : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }

  template<typename Real, int R>
  Ray<Real, R> & Ray<Real, R>::operator=(Ray<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }

  template<typename Real, int R>
  ErrorCode Ray<Real, R>::Set(Vector<Real, R> const & a_origin, Vector<Real, R> const & a_direction)
  {
    m_origin = a_origin;
    m_direction = Normalize(a_direction);
    return ErrorCode::None;
  }
}

#endif