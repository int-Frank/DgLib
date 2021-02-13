//@group Math/Geometry

/*
  Derived from https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
*/

#ifndef DGR2CONVEXHULL_H
#define DGR2CONVEXHULL_H

#include <algorithm>

#include "DgMath.h"
#include "DgVector.h"
#include "DgError.h"

namespace Dg
{
  namespace impl_ConvexHull
  {
    template<typename Real>
    Orientation GetOrientation(Vector2<Real> const& a_p0, Vector2<Real> const& a_p1, Vector2<Real> const& a_p2)
    {
      Vector2<Real> v_01 = a_p1 - a_p0;
      Vector2<Real> v_12 = a_p2 - a_p1;
      Real perpDot = PerpDot(v_01, v_12);
      if (perpDot > Dg::Constants<Real>::EPSILON)
        return Orientation::CCW;
      else if (perpDot < -Dg::Constants<Real>::EPSILON)
        return Orientation::CW;
      return Orientation::Colinear;
    }
  }

  //Container must have the methods:
  // - push_back()
  // - pop_back()
  // - end()
  template <typename Real, typename Container>
  ErrorCode ConvexHull2(Vector2<Real> const * a_points, size_t a_pointCount, Container & a_out)
  {
    ErrorCode result;
    Vector2<Real> const ** ppTempPoints = nullptr;

    if (a_points == nullptr)
      DG_ERROR_SET(ErrorCode::BadInput);

    if (a_pointCount < 3)
      DG_ERROR_SET(ErrorCode::None);

    // Find the bottom most point 
    Real ymin = a_points[0][1];
    size_t minIndex = 0;
    for (size_t i = 1; i < a_pointCount; i++)
    {
      Real y = a_points[i][1];

      // Pick the bottom-most or chose the left 
      // most point in case of tie 
      if ((y < ymin) || (ymin == y &&
        a_points[i][0] < a_points[minIndex][0]))
      {
        ymin = a_points[i][1];
        minIndex = i;
      }
    }

    ppTempPoints = new Vector2<Real> const *[a_pointCount];
    DG_ERROR_NULL(ppTempPoints, ErrorCode::FailedToAllocMem);

    for (size_t i = 0; i < a_pointCount; i++)
      ppTempPoints[i] = &a_points[i];

    // Place the bottom-most point at first position 
    {
      Vector2<Real> const * pTemp = ppTempPoints[0];
      ppTempPoints[0] = ppTempPoints[minIndex];
      ppTempPoints[minIndex] = pTemp;
    }

    // Sort n-1 points with respect to the first point. 
    // A point p1 comes before p2 in sorted output if p2 
    // has larger polar angle (in counterclockwise 
    // direction) than p1 
    std::sort(&ppTempPoints[1], &ppTempPoints[1] + a_pointCount - 1, [p0 = ppTempPoints[0]](Vector2<Real> const * a_p1, Vector2<Real> const * a_p2)
    {
      Orientation o = impl_ConvexHull::GetOrientation(*p0, *a_p1, *a_p2);
      if (o == Orientation::Colinear)
        return MagSq(vec2(*p0 - *a_p1)) <= MagSq(*p0 - *a_p2);

      return o == Orientation::CCW;
    });

    // If two or more points make same angle with p0, 
    // Remove all but the one that is farthest from p0 
    // Remember that, in above sorting, our criteria was 
    // to keep the farthest point at the end when more than 
    // one points have same angle. 
    size_t m = 1; // Initialize size of modified array 
    for (size_t i = 1; i < a_pointCount; i++)
    {
      // Keep removing i while angle of i and i+1 is same 
      // with respect to p0 
      while ((i < a_pointCount - 1) && (impl_ConvexHull::GetOrientation(*ppTempPoints[0], *ppTempPoints[i], *ppTempPoints[i+1]) == Dg::Orientation::Colinear))
        i++;

      ppTempPoints[m] = ppTempPoints[i];
      m++;  // Update size of modified array 
    }

    // If modified array of points has less than 3 points, 
    // convex hull is not possible 
    DG_ERROR_IF(m < 3, ErrorCode::Failure);

    a_out.push_back(*ppTempPoints[0]);
    a_out.push_back(*ppTempPoints[1]);
    a_out.push_back(*ppTempPoints[2]);

    // Process remaining n-3 points 
    for (size_t i = 3; i < m; i++)
    {
      // Keep removing top while the angle formed by 
      // points next-to-top, top, and points[i] makes 
      // a non-left turn 
      while (impl_ConvexHull::GetOrientation(*(--(--a_out.end())), *(--a_out.end()), *ppTempPoints[i]) != Orientation::CCW)
        a_out.pop_back();
      a_out.push_back(*ppTempPoints[i]);
    }

    result = ErrorCode::None;
  epilogue:
    delete[] ppTempPoints;
    return result;
  }

  //A faster version of ConvexHull, but will modify the input point array.
  template <typename Real, typename Container>
  ErrorCode ConvexHull2_Mutable(Vector2<Real>* a_points, size_t a_pointCount, Container& a_out)
  {
    ErrorCode result;
    Vector2<Real> const ** ppTempPoints = nullptr;

    if (a_points == nullptr)
      DG_ERROR_SET(ErrorCode::BadInput);

    if (a_pointCount < 3)
      DG_ERROR_SET(ErrorCode::None);

    // Find the bottom most point 
    Real ymin = a_points[0][1];
    size_t minIndex = 0;
    for (size_t i = 1; i < a_pointCount; i++)
    {
      Real y = a_points[i][1];

      // Pick the bottom-most or chose the left 
      // most point in case of tie 
      if ((y < ymin) || (ymin == y &&
        a_points[i][0] < a_points[minIndex][0]))
      {
        ymin = a_points[i][1];
        minIndex = i;
      }
    }

    // Place the bottom-most point at first position 
    {
      Vector2<Real> temp = a_points[0];
      a_points[0] = a_points[minIndex];
      a_points[minIndex] = temp;
    }

    // Sort n-1 points with respect to the first point. 
    // A point p1 comes before p2 in sorted output if p2 
    // has larger polar angle (in counterclockwise 
    // direction) than p1 
    std::sort(&a_points[1], &a_points[1] + a_pointCount - 1, [p0 = a_points[0]](Vector2<Real> const& a_p1, Vector2<Real> const& a_p2)
    {
      Orientation o = impl_ConvexHull::GetOrientation(p0, a_p1, a_p2);
      if (o == Orientation::Colinear)
        return MagSq(p0 - a_p1) <= MagSq(p0 - a_p2);

      return o == Orientation::CCW;
    });

    // If two or more points make same angle with p0, 
    // Remove all but the one that is farthest from p0 
    // Remember that, in above sorting, our criteria was 
    // to keep the farthest point at the end when more than 
    // one points have same angle. 
    size_t m = 1; // Initialize size of modified array 
    for (size_t i = 1; i < a_pointCount; i++)
    {
      // Keep removing i while angle of i and i+1 is same 
      // with respect to p0 
      while ((i < a_pointCount - 1) && (impl_ConvexHull::GetOrientation(a_points[0], a_points[i], a_points[i+1]) == Dg::Orientation::Colinear))
        i++;

      a_points[m] = a_points[i];
      m++;  // Update size of modified array 
    }

    // If modified array of points has less than 3 points, 
    // convex hull is not possible 
    DG_ERROR_IF(m < 3, ErrorCode::Failure);

    a_out.push_back(a_points[0]);
    a_out.push_back(a_points[1]);
    a_out.push_back(a_points[2]);

    // Process remaining n-3 points 
    for (size_t i = 3; i < m; i++)
    {
      // Keep removing top while the angle formed by 
      // points next-to-top, top, and points[i] makes 
      // a non-left turn 
      while (impl_ConvexHull::GetOrientation(*(--(--a_out.end())), *(--a_out.end()), a_points[i]) != Orientation::CCW)
        a_out.pop_back();
      a_out.push_back(a_points[i]);
    }

    result = ErrorCode::None;
  epilogue:
    return result;
  }
}

#endif