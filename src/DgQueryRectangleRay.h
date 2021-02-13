//@group Math/Geometry/Query

#ifndef DGQUERYRECTANGLERAY_H
#define DGQUERYRECTANGLERAY_H

#include <limits>

#include "DgQuery.h"
#include "DgRectangle.h"
#include "DgRay.h"
#include "DgSegment.h"
#include "DgGeometryAncillary.h"
#include "DgQuerySegmentRay.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Ray3<Real>>
  {
  public:

    struct Result
    {
      Real          u;
      Real          rectangleParameter[2];
      Vector3<Real> point;
      QueryCode     code;
    };
    
    Result operator()(Rectangle3<Real> const &, Ray3<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using FI3RectangleRay = Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Ray3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Ray3<Real>>::Result
    Query<QueryType::FindIntersection, Real, 3, Rectangle3<Real>, Ray3<Real>>::operator()
    (Rectangle3<Real> const & a_rect, Ray3<Real> const & a_ray)
  {
    Result result;

    Vector3<Real> rectNorm = Cross(a_rect.GetAxis(0), a_rect.GetAxis(1));
    Real denom = Dot(rectNorm, a_ray.Direction());

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if ray is on the rectangle plane
      Vector3<Real> v = a_rect.GetCenter() - a_ray.Origin();
      if (Dg::IsZero(Dot(v, rectNorm)))
      {
        result.code = QueryCode::NotIntersecting;
        CP3SegmentRay<Real> query;

        //Find intersection with the smallest 'u' ray value
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

            CP3SegmentRay<Real>::Result lrResult = query(segment, a_ray);
            Vector3<Real> v = lrResult.cpr - lrResult.cps;
            if (v.IsZero() && lrResult.ur < result.u)
            {
              result.code = QueryCode::Intersecting;
              result.u = lrResult.ur;
              result.point = lrResult.cps;
              result.rectangleParameter[i1] = static_cast<Real>(i0) * a_rect.GetExtent(i1);
              Real val = lrResult.us * static_cast<Real>(2) - static_cast<Real>(1);
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
      // The ray and rectangle are not parallel, so the ray intersects
      // the plane of the rectangle.

      Vector3<Real> diff = a_ray.Origin() - a_rect.GetCenter();

      //If ray pointing toward rectangle
      if (Dot(diff, a_ray.Direction()) < static_cast<Real>(0))
      {
        Vector3<Real> basis[3];  // {D, U, V}
        basis[0] = a_ray.Direction();
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
          // Ray parameter for the point of intersection.
          Real DdD0 = Dot(a_ray.Direction(), a_rect.GetAxis(0));
          Real DdD1 = Dot(a_ray.Direction(), a_rect.GetAxis(1));
          Real DdDiff = Dot(a_ray.Direction(), diff);
          result.u = s0*DdD0 + s1*DdD1 - DdDiff;

          // Rectangle coordinates for the point of intersection.
          result.rectangleParameter[0] = s0;
          result.rectangleParameter[1] = s1;

          // The intersection point is inside or on the rectangle.
          result.point = a_ray.Origin() + result.u*a_ray.Direction();

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
  } //End: TIQuery::operator()
}

#endif