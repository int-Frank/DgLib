//! @file DgRay_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray_generic

#ifndef DGRAY_GENERIC_H
#define DGRAY_GENERIC_H

#include "DgVector_generic.h"
#include "DgMatrix_generic.h"
#include "../DgR3VQS.h"
#include "..\dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Ray_generic
    //!
    //! A Ray_generic is defined by a point of origin and direction extending infinitely in one
    //! direction. It is represented by a Point4 and a normalized Vector. This file
    //! also declares methods to test Rays against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 1/06/2016
    template<typename Real, int R>
    class Ray_generic
    {
    public:
      //! Default constructor
      Ray_generic();

      //! Construct from an origin and direction
      Ray_generic(Vector_generic<Real, R> const & origin, 
                  Vector_generic<Real, R> const & direction);
      ~Ray_generic() {}

      //! Copy constructor
      Ray_generic(Ray_generic const &);

      //! Assignment
      Ray_generic& operator=(Ray_generic const &);

      //! Get the origin of the ray
      Vector_generic<Real, R> const & Origin() const { return m_origin; }

      //! Get the direction of the ray
      Vector_generic<Real, R> const & Direction() const { return m_direction; }

      //! Get the origin and direction of the ray
      void Get(Vector_generic<Real, R>& origin, Vector_generic<Real, R>& direction) const;

      //! Comparison
      bool operator== (Ray_generic const &) const;

      //! Comparison
      bool operator!= (Ray_generic const &) const;

      //! Set ray from an origin and direction
      void Set(Vector_generic<Real, R> const & origin, 
               Vector_generic<Real, R> const & direction);

      //! Transform the ray
      Ray_generic GetTransformed(Matrix_generic<Real, R> const &) const;

      //! Transform the ray, assign to self
      Ray_generic & TransformSelf(Matrix_generic<Real, R> const &);

      //! Transform the ray
      Ray_generic GetTransformed(R3::VQS<Real> const &) const;

      //! Transform the ray, assign to self
      Ray_generic & TransformSelf(R3::VQS<Real> const &);
    private:

      //Data members
      Vector_generic<Real, R> m_origin;
      Vector_generic<Real, R> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@ Ray_generic::Ray_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R>::Ray_generic() 
      : m_origin(Vector_generic<Real, R>::Origin())
      , m_direction(Vector_generic<Real, R>::xAxis())
    {
    }	//End: Ray_generic::Ray_generic()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::Ray_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R>::Ray_generic(Vector_generic<Real, R> const & a_origin,
                                      Vector_generic<Real, R> const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Ray_generic::Ray_generic()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::Ray_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R>::Ray_generic(Ray_generic<Real, R> const & a_other) 
      : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Ray_generic::Ray_generic()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R> & Ray_generic<Real, R>::operator=(Ray_generic<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Ray_generic::operator=()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Ray_generic<Real, R>::Get(Vector_generic<Real, R> & a_origin, 
                                   Vector_generic<Real, R> & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Ray_generic::Get()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Ray_generic<Real, R>::operator==(Ray_generic<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Ray_generic::operator==()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Ray_generic<Real, R>::operator!=(Ray_generic<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Ray_generic::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Ray_generic::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Ray_generic<Real, R>::Set(Vector_generic<Real, R> const & a_origin, 
                                   Vector_generic<Real, R> const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Ray_generic::Set()


      //--------------------------------------------------------------------------------
      //	@	Ray_generic::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R> Ray_generic<Real, R>::GetTransformed(Matrix_generic<Real, R> const & a_mat) const
    {
      return Ray_generic<Real, R>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Ray_generic::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray_generic::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R>& Ray_generic<Real, R>::TransformSelf(Matrix_generic<Real, R> const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Ray_generic::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Ray_generic::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R> Ray_generic<Real, R>::GetTransformed(R3::VQS<Real> const & a_vqs) const
    {
      return Ray_generic<Real, R>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Ray_generic::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray_generic::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_generic<Real, R>& Ray_generic<Real, R>::TransformSelf(R3::VQS<Real> const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Ray_generic::TransformSelf()
  }
}

#endif