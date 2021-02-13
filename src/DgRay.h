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

#ifndef DGRAY_H
#define DGRAY_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Ray
  //!
  //! A Ray is defined by a point of origin and direction extending infinitely in one
  //! direction. It is represented by a Point4 and a normalized Vector. This file
  //! also declares methods to test Rays against other geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 1/06/2016
  template<typename Real, int R>
  class Ray
  {
  public:
    //! Default constructor
    Ray();

    //! Construct from an origin and direction
    Ray(Vector<Real, R> const & origin, 
        Vector<Real, R> const & direction);
    ~Ray() {}

    //! Copy constructor
    Ray(Ray const &);

    //! Assignment
    Ray& operator=(Ray const &);

    //! Get the origin of the ray
    Vector<Real, R> const & Origin() const { return m_origin; }

    //! Get the direction of the ray
    Vector<Real, R> const & Direction() const { return m_direction; }

    //! Get the origin and direction of the ray
    void Get(Vector<Real, R>& origin, Vector<Real, R>& direction) const;

    //! Comparison
    bool operator== (Ray const &) const;

    //! Comparison
    bool operator!= (Ray const &) const;

    //! Set ray from an origin and direction
    void Set(Vector<Real, R> const & origin, 
              Vector<Real, R> const & direction);
  private:

    //Data members
    Vector<Real, R> m_origin;
    Vector<Real, R> m_direction;
  };

  template<typename Real>
  using Ray2 = Ray<Real, 2>;

  template<typename Real>
  using Ray3 = Ray<Real, 3>;


  //--------------------------------------------------------------------------------
  //	@ Ray::Ray()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Ray<Real, R>::Ray() 
    : m_origin(Zeros<Real, R>())
    , m_direction(XAxis<Real, R>())
  {
  }	//End: Ray::Ray()


    //--------------------------------------------------------------------------------
    //  @ Ray::Ray()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Ray<Real, R>::Ray(Vector<Real, R> const & a_origin,
                    Vector<Real, R> const & a_direction)
  {
    Set(a_origin, a_direction);
  }	//End: Ray::Ray()


    //--------------------------------------------------------------------------------
    //  @ Ray::Ray()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Ray<Real, R>::Ray(Ray<Real, R> const & a_other) 
    : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }	//End: Ray::Ray()


    //--------------------------------------------------------------------------------
    //  @ Ray::operator=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Ray<Real, R> & Ray<Real, R>::operator=(Ray<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Ray::operator=()


    //--------------------------------------------------------------------------------
    //  @ Ray::Get()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Ray<Real, R>::Get(Vector<Real, R> & a_origin, 
                         Vector<Real, R> & a_direction) const
  {
    a_origin = m_origin;
    a_direction = m_direction;
  }	//End: Ray::Get()


    //--------------------------------------------------------------------------------
    //  @ Ray::operator==()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Ray<Real, R>::operator==(Ray<Real, R> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Ray::operator==()


    //--------------------------------------------------------------------------------
    //  @ Ray::operator!=()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Ray<Real, R>::operator!=(Ray<Real, R> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Ray::operator!=()


    //--------------------------------------------------------------------------------
    //  @ Ray::Set()
    //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Ray<Real, R>::Set(Vector<Real, R> const & a_origin, 
                         Vector<Real, R> const & a_direction)
  {
    //Assign
    m_origin = a_origin;
    m_direction = a_direction;
    m_direction = Normalize(a_direction);

  }	//End: Ray::Set()

}

#endif