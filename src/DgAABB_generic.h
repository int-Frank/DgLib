//! @file DgAABB_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: AABB_generic

#ifndef DGAABB_GENERIC_H
#define DGAABB_GENERIC_H

#include <stdint.h>

#include "DgVector_generic.h"
#include "dgmath.h"

//TODO Queries
namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class AABB_generic
    //!
    //! Axis Aligned Bounding Box.
    //!
    //! @author: Frank Hart
    //! @date 20/06/2016
    template<typename Real, int R>
    class AABB_generic
    {
    public:

    public:
      //! Default constructor.
      AABB_generic()
      {
        for (int i = 0; i < R; i++)
        {
          m_ptMin[i] = std::numeric_limits<Real>::infinity();
          m_ptMax[i] = -std::numeric_limits<Real>::infinity();
        }
        m_ptMax[R] = static_cast<Real>(1.0);
        m_ptMin[R] = static_cast<Real>(1.0);
      }

      //! Constructor.
      AABB_generic(Vector_generic<Real, R> const & a_ptMin,
                   Vector_generic<Real, R> const & a_ptMax)
        : m_ptMax(a_ptMax)
        , m_ptMin(a_ptMin)
      {
        m_ptMax[R] = static_cast<Real>(1.0);
        m_ptMin[R] = static_cast<Real>(1.0);
      }

      template<typename Iterator>
      AABB_generic(Iterator a_first, Iterator a_last)
      {
        for (int i = 0; i < R; i++)
        {
          m_ptMin[i] = std::numeric_limits<Real>::infinity();
          m_ptMax[i] = -std::numeric_limits<Real>::infinity();
        }
        m_ptMax[R] = static_cast<Real>(1.0);
        m_ptMin[R] = static_cast<Real>(1.0);

        for (; a_first != a_last; a_first++)
        {
          for (int i = 0; i < R; i++)
          {
            if      ((*a_first)[i] > m_ptMax[i]) m_ptMax[i] = (*a_first)[i];
            else if ((*a_first)[i] < m_ptMin[i]) m_ptMin[i] = (*a_first)[i];
          }
        }
      }

      //! Get the AABB_generic center.
      Vector_generic<Real, R> GetCenter() const 
      { 
        return Vector_generic<Real, R>(m_ptMin + static_cast<Real>(0.5) * (m_ptMax - m_ptMin));
      }

      Vector_generic<Real, R> Min() const
      {
        return m_ptMin;
      }

      Vector_generic<Real, R> Max() const
      {
        return m_ptMax;
      }

      Vector_generic<Real, R> Diagonal() const
      {
        return Vector_generic<Real, R>(m_ptMax - m_ptMin);
      }

      AABB_generic operator+(AABB_generic const & a_other)
      {
        AABB_generic result;
        for (int i = 0; i < R; i++)
        {
          result.m_ptMax[i] = (a_other.m_ptMax[i] > m_ptMax[i]) ? a_other.m_ptMax[i] : m_ptMax[i];
          result.m_ptMin[i] = (a_other.m_ptMin[i] < m_ptMin[i]) ? a_other.m_ptMin[i] : m_ptMin[i];
        }
        return result;
      }

      AABB_generic & operator+=(AABB_generic const & a_other)
      {
        for (int i = 0; i < R; i++)
        {
          if (a_other.m_ptMax[i] > m_ptMax[i]) m_ptMax[i] = a_other.m_ptMax[i];
          if (a_other.m_ptMin[i] < m_ptMin[i]) m_ptMin[i] = a_other.m_ptMin[i];
        }
        return *this;
      }

      AABB_generic operator+(Vector_generic<Real, R> const & a_point)
      {
        AABB_generic result(*this);
        for (int i = 0; i < R; i++)
        {
          if      (a_point[i] > m_ptMax[i]) result.m_ptMax[i] = a_point[i];
          else if (a_point[i] < m_ptMin[i]) result.m_ptMin[i] = a_point[i];
        }
        return result;
      }

      AABB_generic & operator+=(Vector_generic<Real, R> const & a_point)
      {
        for (int i = 0; i < R; i++)
        {
          if      (a_point[i] > m_ptMax[i]) m_ptMax[i] = a_point[i];
          else if (a_point[i] < m_ptMin[i]) m_ptMin[i] = a_point[i];
        }
        return *this;
      }

    private:
      Vector_generic<Real, R> m_ptMin;
      Vector_generic<Real, R> m_ptMax;
    };
  }
}

#endif