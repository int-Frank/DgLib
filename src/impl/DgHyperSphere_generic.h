//! @file DgBall_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Hypersphere_generic

#ifndef DGHYPERSPHERE_GENERIC_H
#define DGHYPERSPHERE_GENERIC_H

#include "DgVector_generic.h"
#include "dgmath.h"
#include "DgRNG.h"

//TODO Queries
namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Hypersphere_generic
    //!
    //! A sphere can be defined by the set of all points p at distance <= r away from 
    //! a central point c. It is implemented simply by defining a point and radius
    //!
    //! @author: Frank Hart
    //! @date 20/06/2016
    template<typename Real, int R>
    class Hypersphere_generic
    {
    public:
      //! Default constructor.
      Hypersphere_generic() 
        : m_origin(Vector_generic<Real, R>::Origin())
        , m_radius(static_cast<Real>(1.0)) 
      {}

      //! Construct sphere from origin and radius
      Hypersphere_generic(Vector_generic<Real, R> const &, Real);
      ~Hypersphere_generic() {}

      //! Copy constructor
      Hypersphere_generic(Hypersphere_generic const & a_other)
        : m_origin(a_other.m_origin)
        , m_radius(a_other.m_radius) 
      {}

      //! Assignment
      Hypersphere_generic& operator= (Hypersphere_generic const &);

      //! Get the sphere center.
      Vector_generic<Real, R> const & Center() const { return m_origin; }

      //! Get the sphere radius.
      Real Radius() const { return m_radius; }

      //! Get the plane center and .
      void Get(Vector_generic<Real, R>&, Real&) const;

      //! Quick point test
      bool IsOutside(Vector_generic<Real, R> const &) const;

      //! Are two planes equal?
      bool operator== (Hypersphere_generic const &) const;

      //! Are two planes not equal?
      bool operator!= (Hypersphere_generic const &) const;

      //! Set the sphere center
      void SetCenter(Vector_generic<Real, R> const &);

      //! Set the sphere radius
      void SetRadius(Real);

      //! Set the center and radius.
      void Set(Vector_generic<Real, R> const & n, Real);

      //! Get a random point inside the sphere
      Vector_generic<Real, R> GetRandomPointInside() const;

    private:
      Vector_generic<Real, R>   m_origin;
      Real                      m_radius;
    };


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::Hypersphere_generic()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Hypersphere_generic<Real, R>::Hypersphere_generic(Vector_generic<Real, R> const & a_center, Real a_radius)
    {
      Set(a_center, a_radius);
    }	//End: Hypersphere_generic::Hypersphere_generic()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Hypersphere_generic<Real, R> & Hypersphere_generic<Real, R>::operator=(Hypersphere_generic<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_radius = a_other.m_radius;
    }	//End: Hypersphere_generic::operator=()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::Set()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Hypersphere_generic<Real, R>::Set(Vector_generic<Real, R> const & a_center, Real a_radius)
    {
      m_origin = a_center;
      m_origin.w() = static_cast<Real>(1.0);

      if (a_radius < static_cast<Real>(0.0))
      {
        m_radius = static_cast<Real>(0.0);
      }
      else
      {
        m_radius = a_radius;
      }
    }	//End: Plane4::Set()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::operator==()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Hypersphere_generic<Real, R>::operator==(Hypersphere_generic<Real, R> const & a_other) const
    {
      return (m_origin == a_other.m_origin && Dg::AreEqual(m_radius, a_other.m_radius));
    }	//End: Hypersphere_generic::operator==()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::operator!=()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Hypersphere_generic<Real, R>::operator!=(Hypersphere_generic<Real, R> const & a_other) const
    {
      return (m_origin != a_other.m_origin || !Dg::AreEqual(m_radius, a_other.m_radius));
    }	//End: Hypersphere_generic::operator!=()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::SetCenter()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Hypersphere_generic<Real, R>::SetCenter(Vector_generic<Real, R> const & a_center)
    {
      m_origin = a_center;
      m_origin.w() = static_cast<Real>(1.0);
    }	//End: Hypersphere_generic::SetCenter()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::SetRadius()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Hypersphere_generic<Real, R>::SetRadius(Real a_radius)
    {
      m_radius = (a_radius < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_radius;
    }	//End: Hypersphere_generic::SetRadius()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::Get()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Hypersphere_generic<Real, R>::Get(Vector_generic<Real, R> & a_center, Real & a_radius) const
    {
      a_center = m_origin;
      a_radius = m_radius;
    }	//End: Hypersphere_generic::Get()


    template<typename Real, int R>
    bool Hypersphere_generic<Real, R>::IsOutside(Vector_generic<Real, R> const & a_pt) const
    {
      return m_origin.SquaredDistance(a_pt) > (m_radius * m_radius);
    }	//End: Hypersphere_generic::Get()


    //--------------------------------------------------------------------------------
    //	@	Hypersphere_generic::GetRandomPointInside()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Vector_generic<Real, R> Hypersphere_generic<Real, R>::GetRandomPointInside() const
    {
      Vector_generic<Real, R> result(m_origin);
      if (!Dg::IsZero(m_radius))
      {
        do
        {
          for (int i = 0; i < R; i++)
          {
            result[i] = RNG::GetUniform(m_origin[i] - m_radius, m_origin[i] + m_radius);
          }
        } while (Vector_generic<Real, R>(result - m_origin).LengthSquared() > m_radius * m_radius);
      }
      return result;
    }	//End: Hypersphere_generic::GetRandomPointInside()
  }
}

#endif