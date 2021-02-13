//@group Math/Geometry

//Copyright (c) 2015, James M. Van Verth and Lars M. Bishop.
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//* Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//* Neither the name of Essential Mathematics for Games and Interactive
//Applications nor the names of its contributors may be used to endorse
//or promote products derived from this software without specific prior
//written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL JAMES M. VAN VERTH OR LARS M. BISHOP BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 29/05/2016
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