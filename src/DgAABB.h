//@group Math/Geometry

#ifndef DGAABB_H
#define DGAABB_H

#include <stdint.h>

#include "DgError.h"
#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real, int R>
  class AABB
  {
  public:

    AABB();
    AABB(Vector<Real, R> const &a_ptMin, Vector<Real, R> const &a_ptMax);
    AABB(AABB const &);
    AABB &operator=(AABB const &);

    ErrorCode Set(Vector<Real, R> const &a_ptMin, Vector<Real, R> const &a_ptMax);
    
    Vector<Real, R> GetCenter() const;
    Vector<Real, R> Min() const { return m_ptMin; }
    Vector<Real, R> Max() const { return m_ptMax; }
    Vector<Real, R> Diagonal() const { return Vector<Real, R>(m_ptMax - m_ptMin); }

    AABB operator+(AABB const &a_other);
    AABB &operator+=(AABB const &a_other);
    AABB operator+(Vector<Real, R> const &a_point);
    AABB &operator+=(Vector<Real, R> const &a_point);

  private:
    Vector<Real, R> m_ptMin;
    Vector<Real, R> m_ptMax;
  };

  //--------------------------------------------------------------------------------
  //	Typedefs
  //--------------------------------------------------------------------------------

  template<typename Real>
  using AABB2 = AABB<Real, 2>;

  template<typename Real>
  using AABB3 = AABB<Real, 3>;

  //--------------------------------------------------------------------------------
  //	Definitions
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  AABB<Real, R>::AABB(AABB const &other)
    : m_ptMin(other.m_ptMin)
    , m_ptMax(other.m_ptMax)
  {
  }

  template<typename Real, int R>
  AABB<Real, R>::AABB(Vector<Real, R> const &a_ptMin, Vector<Real, R> const &a_ptMax)
    : m_ptMin(a_ptMin)
    , m_ptMax(a_ptMax)
  {
    Set(a_ptMin, a_ptMax);
  }

  template<typename Real, int R>
  AABB<Real, R> &AABB<Real, R>::operator=(AABB const &other)
  {
    m_ptMin = other.m_ptMin;
    m_ptMax = other.m_ptMax;

    return *this;
  }

  template<typename Real, int R>
  ErrorCode AABB<Real, R>::Set(Vector<Real, R> const &a_ptMin, Vector<Real, R> const &a_ptMax)
  {
    ErrorCode result;

    for (int i = 0; i < R; i++)
      DG_ERROR_IF(a_ptMin[i] > a_ptMax[i], ErrorCode::InvalidInput);

    m_ptMin = a_ptMin;
    m_ptMax = a_ptMax;

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<typename Real, int R>
  Vector<Real, R> AABB<Real, R>::GetCenter() const
  {
    return Vector<Real, R>(m_ptMin + static_cast<Real>(0.5) * (m_ptMax - m_ptMin));
  }

  template<typename Real, int R>
  AABB<Real, R> AABB<Real, R>::operator+(AABB const &a_other)
  {
    AABB result;
    for (int i = 0; i < R; i++)
    {
      result.m_ptMax[i] = (a_other.m_ptMax[i] > m_ptMax[i]) ? a_other.m_ptMax[i] : m_ptMax[i];
      result.m_ptMin[i] = (a_other.m_ptMin[i] < m_ptMin[i]) ? a_other.m_ptMin[i] : m_ptMin[i];
    }
    return result;
  }

  template<typename Real, int R>
  AABB<Real, R> & AABB<Real, R>::operator+=(AABB const &a_other)
  {
    for (int i = 0; i < R; i++)
    {
      if (a_other.m_ptMax[i] > m_ptMax[i]) m_ptMax[i] = a_other.m_ptMax[i];
      if (a_other.m_ptMin[i] < m_ptMin[i]) m_ptMin[i] = a_other.m_ptMin[i];
    }
    return *this;
  }

  template<typename Real, int R>
  AABB<Real, R> AABB<Real, R>::operator+(Vector<Real, R> const &a_point)
  {
    AABB result(*this);
    for (int i = 0; i < R; i++)
    {
      if (a_point[i] > m_ptMax[i]) result.m_ptMax[i] = a_point[i];
      else if (a_point[i] < m_ptMin[i]) result.m_ptMin[i] = a_point[i];
    }
    return result;
  }

  template<typename Real, int R>
  AABB<Real, R> & AABB<Real, R>::operator+=(Vector<Real, R> const &a_point)
  {
    for (int i = 0; i < R; i++)
    {
      if (a_point[i] > m_ptMax[i]) m_ptMax[i] = a_point[i];
      else if (a_point[i] < m_ptMin[i]) m_ptMin[i] = a_point[i];
    }
    return *this;
  }
}

#endif