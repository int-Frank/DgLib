#ifndef DGR3QUERYRECTANGLERAY_H
#define DGR3QUERYRECTANGLERAY_H

#include <limits>

#include "impl/DgFIQuery.h"
#include "DgR3Rectangle.h"
#include "DgR3Ray.h"
#include "DgR3Segment.h"
#include "../query/DgQueryCommon.h"
#include "DgR3Vector_ancillary.h"
#include "DgR3QuerySegmentRay.h"

namespace Dg
{
  namespace impl
  {
    template <typename Real>
    class FIQuery<Real, 3, 
          R3::Rectangle<Real>,
          R3::Ray<Real>>
    {
    public:

      struct Result
      {
        Real              u;
        Real              rectangleParameter[2];
        R3::Vector<Real>  point;
        QueryCode         code;
      };

      //! Perform the query
      Result operator()(R3::Rectangle<Real> const &, R3::Ray<Real> const &);
    };

    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, 3, R3::Rectangle<Real>, R3::Ray<Real>>::Result
      FIQuery<Real, 3, R3::Rectangle<Real>, R3::Ray<Real>>::operator()
      (R3::Rectangle<Real> const & a_rect, R3::Ray<Real> const & a_ray)
    {
      Result result;

      R3::Vector<Real> rectNorm = a_rect.GetAxis(0).Cross(a_rect.GetAxis(1));
      Real denom = rectNorm.Dot(a_ray.Direction());

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        //check if ray is on the rectangle plane
        R3::Vector<Real> v = a_rect.GetCenter() - a_ray.Origin();
        if (Dg::IsZero(v.Dot(rectNorm)))
        {
          result.code = QC_NotIntersecting;
          R3::CPSegmentRay<Real> query;

          //Find intersection with the smallest 'u' ray value
          result.u = std::numeric_limits<Real>::max();

          R3::Vector<Real> scaledDir[2] =
          {
            a_rect.GetExtent(0) * a_rect.GetAxis(0),
            a_rect.GetExtent(1) * a_rect.GetAxis(1)
          };
          for (int i1 = 0, omi1 = 1; i1 <= 1; ++i1, --omi1)
          {
            for (int i0 = -1; i0 <= 1; i0 += 2)
            {
              R3::Vector<Real> segCenter = a_rect.GetCenter() + scaledDir[i1] * static_cast<Real>(i0);
              
              R3::Vector<Real> p0(segCenter - scaledDir[omi1]);
              R3::Vector<Real> p1(segCenter + scaledDir[omi1]);
              R3::Segment<Real> segment(p0, p1);

              R3::CPSegmentRay<Real>::Result lrResult = query(segment, a_ray);
              R3::Vector<Real> v = lrResult.cpr - lrResult.cps;
              if (v.IsZero() && lrResult.ur < result.u)
              {
                result.code = QC_Intersecting;
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
          result.code = QC_NotIntersecting;
        }
      }
      else
      {
        // The ray and rectangle are not parallel, so the ray intersects
        // the plane of the rectangle.

        R3::Vector<Real> diff = a_ray.Origin() - a_rect.GetCenter();

        //If ray pointing toward rectangle
        if (diff.Dot(a_ray.Direction()) < static_cast<Real>(0))
        {
          R3::Vector<Real> basis[3];  // {D, U, V}
          basis[0] = a_ray.Direction();
          R3::ComputeOrthogonalComplement<Real>(1, basis);
          Real UdD0 = basis[1].Dot(a_rect.GetAxis(0));
          Real UdD1 = basis[1].Dot(a_rect.GetAxis(1));
          Real UdPmC = basis[1].Dot(diff);
          Real VdD0 = basis[2].Dot(a_rect.GetAxis(0));
          Real VdD1 = basis[2].Dot(a_rect.GetAxis(1));
          Real VdPmC = basis[2].Dot(diff);
          Real invDet = (static_cast<Real>(1)) / (UdD0*VdD1 - UdD1*VdD0);

          // Rectangle coordinates for the point of intersection.
          Real s0 = (VdD1*UdPmC - UdD1*VdPmC)*invDet;
          Real s1 = (UdD0*VdPmC - VdD0*UdPmC)*invDet;

          if (std::abs(s0) <= a_rect.GetExtent(0)
            && std::abs(s1) <= a_rect.GetExtent(1))
          {
            // Ray parameter for the point of intersection.
            Real DdD0 = a_ray.Direction().Dot(a_rect.GetAxis(0));
            Real DdD1 = a_ray.Direction().Dot(a_rect.GetAxis(1));
            Real DdDiff = a_ray.Direction().Dot(diff);
            result.u = s0*DdD0 + s1*DdD1 - DdDiff;

            // Rectangle coordinates for the point of intersection.
            result.rectangleParameter[0] = s0;
            result.rectangleParameter[1] = s1;

            // The intersection point is inside or on the rectangle.
            result.point = a_ray.Origin() + result.u*a_ray.Direction();

            result.code = QC_Intersecting;
          }
          else
          {
            result.code = QC_NotIntersecting;
          }
        }
        else
        {
          result.code = QC_NotIntersecting;
        }
      }
      return result;
    } //End: TIQuery::operator()
  }

  namespace R3
  {
    template<typename Real>
    using FIRectangleRay = impl::FIQuery<Real, 3, Rectangle<Real>, Ray<Real>>;
  }
}

#endif