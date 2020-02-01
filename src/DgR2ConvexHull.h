//@group Math/R2

#ifndef DGR2CONVEXHULL_H
#define DGR2CONVEXHULL_H

#include <algorithm>

#include "DgMath.h"
#include "DgR2Vector.h"

namespace Dg
{
  namespace R2
  {
    namespace impl_ConvexHull
    {
      template<typename Real>
      Orientation GetOrientation(R2::Vector<Real> const& a_p0, R2::Vector<Real> const& a_p1, R2::Vector<Real> const& a_p2)
      {
        R2::Vector<Real> v_01 = a_p1 - a_p0;
        R2::Vector<Real> v_12 = a_p2 - a_p1;
        Real perpDot = v_01.PerpDot(v_12);
        if (perpDot > static_cast<Real>(0))
          return Orientation::CCW;
        else if (perpDot < static_cast<Real>(0))
          return Orientation::CW;
        return Orientation::Colinear;
      }
    }

    //Container must have the methods:
    // - push_back()
    // - pop_back()
    // - end()
    template <typename Real, typename Container>
    void ConvexHull(R2::Vector<Real> const * a_points, size_t a_pointCount, Container & a_out)
    {
      if (a_points == nullptr)
        return;

      if (a_pointCount < 3)
        return;

      // Find the bottom most point 
      Real ymin = a_points[0].y();
      size_t minIndex = 0;
      for (size_t i = 1; i < a_pointCount; i++)
      {
        Real y = a_points[i].y();

        // Pick the bottom-most or chose the left 
        // most point in case of tie 
        if ((y < ymin) || (ymin == y &&
          a_points[i].x() < a_points[minIndex].x()))
        {
          ymin = a_points[i].y();
          minIndex = i;
        }
      }

      R2::Vector<Real> const ** ptrs = new R2::Vector<Real> const *[a_pointCount];
      for (size_t i = 0; i < a_pointCount; i++)
        ptrs[i] = &a_points[i];

      // Place the bottom-most point at first position 
      {
        R2::Vector<Real> const * temp = ptrs[0];
        ptrs[0] = ptrs[minIndex];
        ptrs[minIndex] = temp;
      }

      // Sort n-1 points with respect to the first point. 
      // A point p1 comes before p2 in sorted output if p2 
      // has larger polar angle (in counterclockwise 
      // direction) than p1 
      //p0 = points[0];
      std::sort(&ptrs[1], &ptrs[1] + a_pointCount - 1, [p0 = ptrs[0]](R2::Vector<Real> const * a_p1, R2::Vector<Real> const * a_p2)
      {
        // Find orientation 
        Orientation o = impl_ConvexHull::GetOrientation(*p0, *a_p1, *a_p2);
        if (o == Orientation::Colinear)
          return p0->SquaredDistance(*a_p1) <= p0->SquaredDistance(*a_p2);

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
        while (i < a_pointCount - 1 
          && impl_ConvexHull::GetOrientation(*ptrs[0], *ptrs[i], *ptrs[i+1]) == Dg::Orientation::Colinear)
          i++;

        ptrs[m] = ptrs[i];
        m++;  // Update size of modified array 
      }

      // If modified array of points has less than 3 points, 
      // convex hull is not possible 
      if (m < 3) goto epilogue;

      // Create an empty stack and push first three points 
      // to it. 
      a_out.push_back(*ptrs[0]);
      a_out.push_back(*ptrs[1]);
      a_out.push_back(*ptrs[2]);

      // Process remaining n-3 points 
      for (size_t i = 3; i < m; i++)
      {
        // Keep removing top while the angle formed by 
        // points next-to-top, top, and points[i] makes 
        // a non-left turn 
        while (impl_ConvexHull::GetOrientation(*(--(--a_out.end())), *(--a_out.end()), *ptrs[i]) != Orientation::CCW)
          a_out.pop_back();
        a_out.push_back(*ptrs[i]);
      }

      epilogue:

      delete[] ptrs;
    }
  }
}

#endif