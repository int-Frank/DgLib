//@group Math/R3/Queries

#ifndef DGR3QUERYRECTANGLESEGMENT_H
#define DGR3QUERYRECTANGLESEGMENT_H

#include <limits>

#include "DgQuery.h"
#include "DgRectangle.h"
#include "DgSegment.h"
#include "DgGeometryAncillary.h"
#include "DgQuerySegmentSegment.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Segment3<Real>>
  {
  public:

    struct Result
    {
      Real          u; //Distance from p0 along the segment direction to the point of intersection
      Real          rectangleParameter[2];
      Vector3<Real> point;
      QueryCode     code;
    };

    Result operator()(Rectangle3<Real> const &, Segment3<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using FI3RectangleSegment = Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Segment3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Segment3<Real>>::Result
    Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Segment3<Real>>::operator()
    (Rectangle3<Real> const & a_rect, Segment3<Real> const & a_seg)
  {
    Result result;

    Vector3<Real> rectNorm = Cross(a_rect.GetAxis(0), a_rect.GetAxis(1));
    Real denom = Dot(rectNorm, a_seg.Direction());

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if line is on the rectangle plane
      Vector3<Real> v = a_rect.GetCenter() - a_seg.Origin();
      if (Dg::IsZero(Dot(v, rectNorm)))
      {
        result.code = QueryCode::NotIntersecting;
        CP3SegmentSegment<Real> query;

        //Find intersection with the smallest 'u' line value
        result.u = std::numeric_limits<Real>::max();

        Vector3<Real> scaledDir[2] =
        {
          a_rect.GetExtent(0) * a_rect.GetAxis(0),
          a_rect.GetExtent(1) * a_rect.GetAxis(1)
        };
        for (int i1 = 0, omi1 = 1; i1 <= 1; ++i1, --omi1)
        {
          for (int i0 = -1; i0 <= 1; i0 += 2)
          {
            Vector3<Real> segCenter = a_rect.GetCenter() + scaledDir[i1] * static_cast<Real>(i0);

            Vector3<Real> p0(segCenter - scaledDir[omi1]);
            Vector3<Real> p1(segCenter + scaledDir[omi1]);
            Segment3<Real> segment(p0, p1);

            CP3SegmentSegment<Real>::Result ssResult = query(segment, a_seg);
            Vector3<Real> v = ssResult.cp0 - ssResult.cp1;
            if (v.IsZero() && ssResult.u1 < result.u)
            {
              result.code = QueryCode::Intersecting;
              result.u = ssResult.u1;
              result.point = ssResult.cp1;
              result.rectangleParameter[i1] = static_cast<Real>(i0) * a_rect.GetExtent(i1);
              Real val = ssResult.u1 * static_cast<Real>(2) - static_cast<Real>(1);
              result.rectangleParameter[omi1] = val * a_rect.GetExtent(omi1);
            }
          }
        }
      }
      else
      {
        result.code = QueryCode::NotIntersecting;
      }
    }
    else
    {
      // The line and rectangle are not parallel, so the line intersects
      // the plane of the rectangle.

      Vector3<Real> diff = a_seg.GetP0() - a_rect.GetCenter();
      Vector3<Real> diff2 = a_seg.GetP1() - a_rect.GetCenter();

      //Either end point in the segment need to lie on opposite sides of
      //the rectangle plane
      Real dDdir = Dot(diff, a_seg.Direction());
      Real d2Ddir = Dot(diff2, a_seg.Direction());

      if (dDdir * d2Ddir < static_cast<Real>(0))
      {
        Vector3<Real> basis[3];  // {D, U, V}
        basis[0] = a_seg.Direction();
        ComputeOrthogonalComplement<Real>(1, basis);
        Real UdD0 = Dot(basis[1], a_rect.GetAxis(0));
        Real UdD1 = Dot(basis[1], a_rect.GetAxis(1));
        Real UdPmC = Dot(basis[1], diff);
        Real VdD0 = Dot(basis[2], a_rect.GetAxis(0));
        Real VdD1 = Dot(basis[2], a_rect.GetAxis(1));
        Real VdPmC = Dot(basis[2], diff);
        Real invDet = (static_cast<Real>(1)) / (UdD0*VdD1 - UdD1*VdD0);

        // Rectangle coordinates for the point of intersection.
        Real s0 = (VdD1*UdPmC - UdD1*VdPmC)*invDet;
        Real s1 = (UdD0*VdPmC - VdD0*UdPmC)*invDet;

        if (std::abs(s0) <= a_rect.GetExtent(0)
          && std::abs(s1) <= a_rect.GetExtent(1))
        {
          // Segment parameter for the point of intersection.
          Real DdD0 = Dot(basis[0], a_rect.GetAxis(0));
          Real DdD1 = Dot(basis[0], a_rect.GetAxis(1));
          Real DdDiff = Dot(basis[0], diff);
          result.u = s0*DdD0 + s1*DdD1 - DdDiff;

          // Rectangle coordinates for the point of intersection.
          result.rectangleParameter[0] = s0;
          result.rectangleParameter[1] = s1;

          // The intersection point is inside or on the rectangle.
          result.point = a_seg.Origin() + result.u*basis[0];

          result.code = QueryCode::Intersecting;
        }
        else
        {
          result.code = QueryCode::NotIntersecting;
        }
      }
      else
      {
        result.code = QueryCode::NotIntersecting;
      }
    }
    return result;
  }
}

#endif