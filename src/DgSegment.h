//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGSEGMENT_GENERAL_H
#define DGSEGMENT_GENERAL_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Segment
  //!
  //! A Linesgment is a line that connects two points. It is represented by a Point4 
  //! and a Vector, the length of the vector being the distance between the points.
  //! This file also declares methods to test LineSegments against other 
  //! geometric entities.
  template<typename Real, int R>
  class Segment
  {
  public:
    //! Default constructor
    Segment();

    //! Construct from an origin and direction
    Segment(Vector<Real, R> const & p0, 
            Vector<Real, R> const & p1);
    ~Segment() {}

    //! Copy constructor
    Segment(Segment const &);

    //! Assignment
    Segment& operator=(Segment const &);

    //! Get the origin of the line
    Vector<Real, R> const & Origin() const { return m_origin; }

    //! Get the direction of the line
    Vector<Real, R> const & Direction() const { return m_direction; }

    //! Get endpoint 0
    Vector<Real, R> GetP0() const { return m_origin; }

    //! Get endpoint 1
    Vector<Real, R> GetP1() const { return m_origin + m_direction; }

    //! Get the center of the line segment
    Vector<Real, R> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

    //! Get the endpoints of the line segment
    void Get(Vector<Real, R>& a_p0, Vector<Real, R>& a_p1) const;

    //! Comparison
    bool operator== (Segment const &) const;

    //! Comparison
    bool operator!= (Segment const &) const;

    //! Set line segment from endpoints
    void Set(Vector<Real, R> const & p0, Vector<Real, R> const & p1);

    //! Get the length of the line segment
    Real Length() const;

    //! Get the squared length of the line segment
    Real LengthSquared() const;

  private:

    //Data members
    Vector<Real, R> m_origin;
    Vector<Real, R> m_direction;
  };

  template<typename Real>
  using Segment2 = Segment<Real, 2>;

  template<typename Real>
  using Segment3 = Segment<Real, 3>;


  //--------------------------------------------------------------------------------
  //	@	Segment::Segment()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Segment<Real, R>::Segment()
    : m_origin(Zeros<Real, R>())
    , m_direction(XAxis<Real, R>())
  {
  }	//End: Segment::Segment()


    //--------------------------------------------------------------------------------
    //  @ Segment::Segment()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Segment<Real, R>::Segment(Vector<Real, R> const & p0, Vector<Real, R> const & p1)
  {
    Set(p0, p1);
  }	//End: Segment::Segment()


    //--------------------------------------------------------------------------------
    //  @ Segment::Segment()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Segment<Real, R>::Segment(Segment<Real, R> const & a_other)
    : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }	//End: Segment::Segment()


    //--------------------------------------------------------------------------------
    //  @ Segment::operator=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Segment<Real, R> & Segment<Real, R>::operator=(Segment<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Segment::operator=()


    //--------------------------------------------------------------------------------
    //  @ Segment::Get()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Segment<Real, R>::Get(Vector<Real, R> & a_p0, Vector<Real, R> & a_p1) const
  {
    a_p0 = m_origin;
    a_p1 = m_origin + m_direction;
  }	//End: Segment::Get()


    //--------------------------------------------------------------------------------
    //  @ Segment::operator==()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Segment<Real, R>::operator==(Segment<Real, R> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Segment::operator==()


    //--------------------------------------------------------------------------------
    //  @ Segment::operator!=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Segment<Real, R>::operator!=(Segment<Real, R> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Segment::operator!=()


    //--------------------------------------------------------------------------------
    //  @ Segment::Set()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Segment<Real, R>::Set(Vector<Real, R> const & a_p0, Vector<Real, R> const & a_p1)
  {
    //Assign
    m_origin = a_p0;
    m_direction = a_p1 - a_p0;

  }	//End: Segment::Set()


    //--------------------------------------------------------------------------------
    //  @ Segment::Length()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Real Segment<Real, R>::Length() const
  {
    return Mag(m_direction);
  }	//End: Segment::Length()


    //--------------------------------------------------------------------------------
    //  @ Segment::LengthSquared()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Real Segment<Real, R>::LengthSquared() const
  {
    return MagSq(m_direction);

  }	//End: Segment::LengthSquared()
}
#endif