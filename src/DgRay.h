//@group Math/Geometry

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

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