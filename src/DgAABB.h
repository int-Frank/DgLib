//@group Math/Geometry

#ifndef DGAABB_H
#define DGAABB_H

#include <stdint.h>

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real, int R>
  class AABB
  {
  public:

  public:
    //! Default constructor.
    AABB()
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
    AABB(Vector<Real, R> const & a_ptMin,
                  Vector<Real, R> const & a_ptMax)
      : m_ptMax(a_ptMax)
      , m_ptMin(a_ptMin)
    {
      m_ptMax[R] = static_cast<Real>(1.0);
      m_ptMin[R] = static_cast<Real>(1.0);
    }

    template<typename Iterator>
    AABB(Iterator a_first, Iterator a_last)
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

    //! Get the AABB center.
    Vector<Real, R> GetCenter() const 
    { 
      return Vector<Real, R>(m_ptMin + static_cast<Real>(0.5) * (m_ptMax - m_ptMin));
    }

    Vector<Real, R> Min() const
    {
      return m_ptMin;
    }

    Vector<Real, R> Max() const
    {
      return m_ptMax;
    }

    Vector<Real, R> Diagonal() const
    {
      return Vector<Real, R>(m_ptMax - m_ptMin);
    }

    AABB operator+(AABB const & a_other)
    {
      AABB result;
      for (int i = 0; i < R; i++)
      {
        result.m_ptMax[i] = (a_other.m_ptMax[i] > m_ptMax[i]) ? a_other.m_ptMax[i] : m_ptMax[i];
        result.m_ptMin[i] = (a_other.m_ptMin[i] < m_ptMin[i]) ? a_other.m_ptMin[i] : m_ptMin[i];
      }
      return result;
    }

    AABB & operator+=(AABB const & a_other)
    {
      for (int i = 0; i < R; i++)
      {
        if (a_other.m_ptMax[i] > m_ptMax[i]) m_ptMax[i] = a_other.m_ptMax[i];
        if (a_other.m_ptMin[i] < m_ptMin[i]) m_ptMin[i] = a_other.m_ptMin[i];
      }
      return *this;
    }

    AABB operator+(Vector<Real, R> const & a_point)
    {
      AABB result(*this);
      for (int i = 0; i < R; i++)
      {
        if      (a_point[i] > m_ptMax[i]) result.m_ptMax[i] = a_point[i];
        else if (a_point[i] < m_ptMin[i]) result.m_ptMin[i] = a_point[i];
      }
      return result;
    }

    AABB & operator+=(Vector<Real, R> const & a_point)
    {
      for (int i = 0; i < R; i++)
      {
        if      (a_point[i] > m_ptMax[i]) m_ptMax[i] = a_point[i];
        else if (a_point[i] < m_ptMin[i]) m_ptMin[i] = a_point[i];
      }
      return *this;
    }

  private:
    Vector<Real, R> m_ptMin;
    Vector<Real, R> m_ptMax;
  };


  template<typename Real>
  using AABB2 = AABB<Real, 2>;

  template<typename Real>
  using AABB3 = AABB<Real, 3>;

}

#endif