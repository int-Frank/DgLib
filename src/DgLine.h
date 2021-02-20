//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGLINE_H
#define DGLINE_H

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
    //! Default constructor
    Line();

    //! Construct from an origin and direction
    Line(Vector<Real, R> const & origin,
         Vector<Real, R> const & direction);

    ~Line()
    {
    }

    //! Copy constructor
    Line(Line const &);

    //! Assignment
    Line & operator=(Line const &);

    //! Get the origin of the line
    Vector<Real, R> const & Origin() const
    {
      return m_origin;
    }

    //! Get the direction of the line
    Vector<Real, R> const & Direction() const
    {
      return m_direction;
    }

    //! Get the origin and direction of the line
    void Get(Vector<Real, R> & origin,
             Vector<Real, R> & direction) const;

    //! Comparison
    bool operator== (Line const &) const;

    //! Comparison
    bool operator!= (Line const &) const;

    //! Set line from an origin and direction
    void Set(Vector<Real, R> const & origin,
             Vector<Real, R> const & direction);

  private:

    //Data members
    Vector<Real, R> m_origin;
    Vector<Real, R> m_direction;
  };

  template<typename Real>
  using Line2 = Line<Real, 2>;

  template<typename Real>
  using Line3 = Line<Real, 3>;

  //--------------------------------------------------------------------------------
  //	@	Line::Line()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Line<Real, R>::Line()
    : m_origin(Zeros<Real, R>())
    , m_direction(XAxis<Real, R>())
  {
  }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::Line()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Line<Real, R>::Line(Vector<Real, R> const & a_origin, Vector<Real, R> const & a_direction)
  {
    Set(a_origin, a_direction);
  }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::Line()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Line<Real, R>::Line(Line<Real, R> const & a_other)
    : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::operator=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Line<Real, R> & Line<Real, R>::operator=(Line<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Line::operator=()


    //--------------------------------------------------------------------------------
    //	@	Line::Get()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Line<Real, R>::Get(Vector<Real, R> & a_origin, Vector<Real, R> & a_direction) const
  {
    a_origin = m_origin;
    a_direction = m_direction;
  }	//End: Line::Get()


    //--------------------------------------------------------------------------------
    //	@	Line::operator==()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Line<Real, R>::operator==(Line<Real, R> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Line::operator==()


    //--------------------------------------------------------------------------------
    //	@	Line::operator!=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Line<Real, R>::operator!=(Line<Real, R> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Line::operator!=()


    //--------------------------------------------------------------------------------
    //	@	Line::Set()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Line<Real, R>::Set(Vector<Real, R> const & a_origin, Vector<Real, R> const & a_direction)
  {
    m_origin = a_origin;
    m_direction = Normalize(a_direction);
  }	//End: Line::Set()
}

#endif