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

#ifndef DGLINE_GENERIC_H
#define DGLINE_GENERIC_H

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
    //! @class Line_generic
    //!
    //! A Line_generic is defined by a point and direction extending infinitely in both
    //! directions. It is represented by a point and a normalized vector. This file
    //! also declares methods to test Lines against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real, int R>
    class Line_generic
    {
    public:
      //! Default constructor
      Line_generic();

      //! Construct from an origin and direction
      Line_generic(Vector_generic<Real, R> const & origin, 
                   Vector_generic<Real, R> const & direction);

      ~Line_generic() {}

      //! Copy constructor
      Line_generic(Line_generic const &);

      //! Assignment
      Line_generic& operator=(Line_generic const &);

      //! Get the origin of the line
      Vector_generic<Real, R> const & Origin() const { return m_origin; }

      //! Get the direction of the line
      Vector_generic<Real, R> const & Direction() const { return m_direction; }

      //! Get the origin and direction of the line
      void Get(Vector_generic<Real, R> & origin, 
               Vector_generic<Real, R> & direction) const;

      //! Comparison
      bool operator== (Line_generic const &) const;

      //! Comparison
      bool operator!= (Line_generic const &) const;

      //! Set line from an origin and direction
      void Set(Vector_generic<Real, R> const & origin, 
               Vector_generic<Real, R> const & direction);

      //! Transform the line
      Line_generic GetTransformed(Matrix_generic<Real, R> const &) const;

      //! Transform the line, assign to self
      Line_generic & TransformSelf(Matrix_generic<Real, R> const &);

      //! Transform the line
      Line_generic GetTransformed(R3::VQS<Real> const &) const;

      //! Transform the line, assign to self
      Line_generic & TransformSelf(R3::VQS<Real> const &);

    private:

      //Data members
      Vector_generic<Real, R> m_origin;
      Vector_generic<Real, R> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Line_generic::Line_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R>::Line_generic() 
      : m_origin(Vector_generic<Real, R>::Origin())
      , m_direction(Vector_generic<Real, R>::xAxis())
    {
    }	//End: Line_generic::Line_generic()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::Line_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R>::Line_generic(Vector_generic<Real, R> const & a_origin, Vector_generic<Real, R> const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Line_generic::Line_generic()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::Line_generic()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R>::Line_generic(Line_generic<Real, R> const & a_other) 
      : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Line_generic::Line_generic()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R> & Line_generic<Real, R>::operator=(Line_generic<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Line_generic::operator=()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Line_generic<Real, R>::Get(Vector_generic<Real, R> & a_origin, Vector_generic<Real, R> & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Line_generic::Get()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Line_generic<Real, R>::operator==(Line_generic<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Line_generic::operator==()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Line_generic<Real, R>::operator!=(Line_generic<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Line_generic::operator!=()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Line_generic<Real, R>::Set(Vector_generic<Real, R> const & a_origin, Vector_generic<Real, R> const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Line_generic::Set()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R> Line_generic<Real, R>::GetTransformed(Matrix_generic<Real, R> const & a_mat) const
    {
      return Line_generic<Real, R>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Line_generic::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R>& Line_generic<Real, R>::TransformSelf(Matrix_generic<Real, R> const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Line_generic::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R> Line_generic<Real, R>::GetTransformed(R3::VQS<Real> const & a_vqs) const
    {
      return Line_generic<Real, R>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Line_generic::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Line_generic::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_generic<Real, R>& Line_generic<Real, R>::TransformSelf(R3::VQS<Real> const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Line_generic::TransformSelf()
  }
}

#endif