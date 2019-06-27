//! @file DgSegment_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment_generic

#ifndef DGSEGMENT_GENERAL_H
#define DGSEGMENT_GENERAL_H

#include "DgVector_generic.h"
#include "DgMatrix_generic.h"
#include "../DgR3VQS.h"
#include "../dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Segment_generic
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
    class Segment_generic
    {
    public:
      //! Default constructor
      Segment_generic();

      //! Construct from an origin and direction
      Segment_generic(Vector_generic<Real, R> const & p0, 
                      Vector_generic<Real, R> const & p1);
      ~Segment_generic() {}

      //! Copy constructor
      Segment_generic(Segment_generic const &);

      //! Assignment
      Segment_generic& operator=(Segment_generic const &);

      //! Get the origin of the line
      Vector_generic<Real, R> const & Origin() const { return m_origin; }

      //! Get the direction of the line
      Vector_generic<Real, R> const & Direction() const { return m_direction; }

      //! Get endpoint 0
      Vector_generic<Real, R> GetP0() const { return m_origin; }

      //! Get endpoint 1
      Vector_generic<Real, R> GetP1() const { return m_origin + m_direction; }

      //! Get the center of the line segment
      Vector_generic<Real, R> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

      //! Get the endpoints of the line segment
      void Get(Vector_generic<Real, R>& a_p0, Vector_generic<Real, R>& a_p1) const;

      //! Comparison
      bool operator== (Segment_generic const &) const;

      //! Comparison
      bool operator!= (Segment_generic const &) const;

      //! Set line segment from endpoints
      void Set(Vector_generic<Real, R> const & p0, Vector_generic<Real, R> const & p1);

      //! Get the length of the line segment
      Real Length() const;

      //! Get the squared length of the line segment
      Real LengthSquared() const;

      //! Transform the line segment
      Segment_generic GetTransformed(Matrix_generic<Real, R> const &) const;

      //! Transform the line segment, assign to self
      Segment_generic & TransformSelf(Matrix_generic<Real, R> const &);

      //! Transform the line segment
      Segment_generic GetTransformed(R3::VQS<Real> const &) const;

      //! Transform the line segment, assign to self
      Segment_generic & TransformSelf(R3::VQS<Real> const &);
    private:

      //Data members
      Vector_generic<Real, R> m_origin;
      Vector_generic<Real, R> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Segment_generic::Segment_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R>::Segment_generic()
      : m_origin(Vector_generic<Real, R>::Origin())
      , m_direction(Vector_generic<Real, R>::xAxis())
    {
    }	//End: Segment_generic::Segment_generic()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::Segment_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R>::Segment_generic(Vector_generic<Real, R> const & p0, Vector_generic<Real, R> const & p1)
    {
      Set(p0, p1);
    }	//End: Segment_generic::Segment_generic()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::Segment_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R>::Segment_generic(Segment_generic<Real, R> const & a_other)
      : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Segment_generic::Segment_generic()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R> & Segment_generic<Real, R>::operator=(Segment_generic<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Segment_generic::operator=()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Segment_generic<Real, R>::Get(Vector_generic<Real, R> & a_p0, Vector_generic<Real, R> & a_p1) const
    {
      a_p0 = m_origin;
      a_p1 = m_origin + m_direction;
    }	//End: Segment_generic::Get()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Segment_generic<Real, R>::operator==(Segment_generic<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Segment_generic::operator==()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Segment_generic<Real, R>::operator!=(Segment_generic<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Segment_generic::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Segment_generic<Real, R>::Set(Vector_generic<Real, R> const & a_p0, Vector_generic<Real, R> const & a_p1)
    {
      //Assign
      m_origin = a_p0;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_p1 - a_p0;
      m_direction.w() = static_cast<Real>(0.0);

    }	//End: Segment_generic::Set()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::Length()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Real Segment_generic<Real, R>::Length() const
    {
      return m_direction.Length();

    }	//End: Segment_generic::Length()


      //--------------------------------------------------------------------------------
      //  @ Segment_generic::LengthSquared()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Real Segment_generic<Real, R>::LengthSquared() const
    {
      return m_direction.LengthSquared();

    }	//End: Segment_generic::LengthSquared()


      //--------------------------------------------------------------------------------
      //	@	Segment_generic::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R> Segment_generic<Real, R>::GetTransformed(Matrix_generic<Real, R> const & a_mat) const
    {
      Vector_generic<Real, R> p0(m_origin * a_mat);
      Vector_generic<Real, R> p1(p0 + m_direction * a_mat);
      return Segment_generic<Real, R>(p0, p1);
    }	//End: Segment_generic::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment_generic::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R>& Segment_generic<Real, R>::TransformSelf(Matrix_generic<Real, R> const & a_mat)
    {
      Vector_generic<Real, R> p0(m_origin * a_mat);
      Vector_generic<Real, R> p1(p0 + m_direction * a_mat);
      Set(p0, p1);
      return *this;
    }	//End: Segment_generic::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Segment::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R> Segment_generic<Real, R>::GetTransformed(R3::VQS<Real> const & a_vqs) const
    {
      Vector_generic<Real, R> p0(a_vqs.TransformPoint(m_origin));
      Vector_generic<Real, R> p1(p0 + a_vqs.TransformVector(m_direction));
      return Segment_generic<Real, R>(p0, p1);
    }	//End: Segment::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_generic<Real, R>& Segment_generic<Real, R>::TransformSelf(R3::VQS<Real> const & a_vqs)
    {
      Vector_generic<Real, R> p0(a_vqs.TransformPoint(m_origin));
      Vector_generic<Real, R> p1(p0 + a_vqs.TransformVector(m_direction));
      Set(p0, p1);
      return *this;
    }	//End: Segment::TransformSelf()
  }
}
#endif