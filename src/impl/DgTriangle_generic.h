//! @file DgTriangle_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Triangle_generic

#ifndef DGTRIANGLE_GENERAL_H
#define DGTRIANGLE_GENERAL_H

#include "DgVector_generic.h"
#include "../dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Triangle_generic
    //!
    //! @author: Frank Hart
    //! @date 29/05/2016
    template<typename Real, int R>
    class Triangle_generic
    {
    public:
      //! Default constructor
      Triangle_generic();

      //! Construct from an origin and direction
      Triangle_generic(Vector_generic<Real, R> const & p0,
                       Vector_generic<Real, R> const & p1,
                       Vector_generic<Real, R> const & p2);
      ~Triangle_generic() {}

      //! Copy constructor
      Triangle_generic(Triangle_generic const &);

      //! Assignment
      Triangle_generic& operator=(Triangle_generic const &);

      //! Comparison
      bool operator== (Triangle_generic const &) const;

      //! Comparison
      bool operator!= (Triangle_generic const &) const;

      //! Set line segment from endpoints
      void Set(Vector_generic<Real, R> const & p0,
               Vector_generic<Real, R> const & p1,
               Vector_generic<Real, R> const & p2);

      Vector_generic<Real, R> const & P0() const { return m_points[0]; }
      Vector_generic<Real, R> const & P1() const { return m_points[1]; }
      Vector_generic<Real, R> const & P2() const { return m_points[2]; }

      Vector_generic<Real, R> Centroid() const;

    private:

      //Data members
      Vector_generic<Real, R> m_points[3];
    };


    //--------------------------------------------------------------------------------
    //	@	Triangle_generic::Triangle_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Triangle_generic<Real, R>::Triangle_generic()
      : m_point{ Vector_generic<Real, R>::Origin(),
      
                 Vector_generic<Real, R>(static_cast<Real>(1), 
                                         static_cast<Real>(0),
                                         static_cast<Real>(0), 
                                         static_cast<Real>(1)),
      
                 Vector_generic<Real, R>(static_cast<Real>(0), 
                                         static_cast<Real>(1),
                                         static_cast<Real>(0), 
                                         static_cast<Real>(1))}
    {
    }	//End: Triangle_generic::Triangle_generic()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::Triangle_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Triangle_generic<Real, R>::Triangle_generic(Vector_generic<Real, R> const & p0, 
                                                Vector_generic<Real, R> const & p1,
                                                Vector_generic<Real, R> const & p2)
      : m_points{ p0, p1, p2 }
    {
    }	//End: Triangle_generic::Triangle_generic()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::Triangle_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Triangle_generic<Real, R>::Triangle_generic(Triangle_generic<Real, R> const & a_other)
      : m_points{ a_other.m_points[0], a_other.m_points[1], a_other.m_points[2]}
    {
    }	//End: Triangle_generic::Triangle_generic()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Triangle_generic<Real, R> & Triangle_generic<Real, R>::operator=(Triangle_generic<Real, R> const & a_other)
    {
      m_points[0] = a_other.m_points[0];
      m_points[1] = a_other.m_points[1];
      m_points[2] = a_other.m_points[2];

      return *this;
    }	//End: Triangle_generic::operator=()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::operator==()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Triangle_generic<Real, R>::operator==(Triangle_generic<Real, R> const & a_other) const
    {
      bool result = true;
      for (int i = 0; i < 3; i++)
      {
        result &= (m_points[i] == a_other.m_points[i]);
      }
      return result;
    }	//End: Triangle_generic::operator==()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::operator!=()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Triangle_generic<Real, R>::operator!=(Triangle_generic<Real, R> const & a_other) const
    {
      bool result = false;
      for (int i = 0; i < 3; i++)
      {
        result |= (m_points[i] != a_other.m_points[i]);
      }
      return result;
    }	//End: Triangle_generic::operator!=()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::Set()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Triangle_generic<Real, R>::Set(Vector_generic<Real, R> const & a_p0, 
                                        Vector_generic<Real, R> const & a_p1,
                                        Vector_generic<Real, R> const & a_p2)
    {
      m_points[0] = a_p0;
      m_points[1] = a_p1;
      m_points[2] = a_p2;

    }	//End: Triangle_generic::Set()


    //--------------------------------------------------------------------------------
    //  @ Triangle_generic::Centroid()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Vector_generic<Real, R> Triangle_generic<Real, R>::Centroid() const
    {
      return (m_points[0] + m_points[1] + m_points[2]) / static_cast<Real>(3);

    }	//End: Triangle_generic::Set()
  }
}
#endif