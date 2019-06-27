#ifndef DGR3QUERYRECTANGLESEGMENT_H
#define DGR3QUERYRECTANGLESEGMENT_H

#include <limits>

#include "impl/DgFIQuery.h"
#include "DgR3Rectangle.h"
#include "DgR3Segment.h"
#include "DgR3Segment.h"
#include "../query/DgQueryCommon.h"
#include "DgR3Vector_ancillary.h"
#include "DgR3QuerySegmentSegment.h"

namespace Dg
{
  namespace impl
  {
    template <typename Real>
    class FIQuery<Real, 3, 
                  R3::Rectangle<Real>,
                  R3::Segment<Real>>
    {
    public:

      struct Result
      {
        Real              u; //Distance from p0 along the segment direction to the point of intersection
        Real              rectangleParameter[2];
        R3::Vector<Real>  point;
        QueryCode         code;
      };

      //! Perform the query
      Result operator()(R3::Rectangle<Real> const &, R3::Segment<Real> const &);
    };

    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, 3, R3::Rectangle<Real>, R3::Segment<Real>>::Result
      FIQuery<Real, 3, R3::Rectangle<Real>, R3::Segment<Real>>::operator()
      (R3::Rectangle<Real> const & a_rect, R3::Segment<Real> const & a_seg)
    {
      Result result;

      R3::Vector<Real> rectNorm = a_rect.GetAxis(0).Cross(a_rect.GetAxis(1));
      Real denom = rectNorm.Dot(a_seg.Direction());

      //check if line is parallel to plane
      if (Dg::IsZero(denom))
      {
        //check if line is on the rectangle plane
        R3::Vector<Real> v = a_rect.GetCenter() - a_seg.Origin();
        if (Dg::IsZero(v.Dot(rectNorm)))
        {
          result.code = QC_NotIntersecting;
          R3::CPSegmentSegment<Real> query;

          //Find intersection with the smallest 'u' line value
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

              R3::CPSegmentSegment<Real>::Result ssResult = query(segment, a_seg);
              R3::Vector<Real> v = ssResult.cp0 - ssResult.cp1;
              if (v.IsZero() && ssResult.u1 < result.u)
              {
                result.code = QC_Intersecting;
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
          result.code = QC_NotIntersecting;
        }
      }
      else
      {
        // The line and rectangle are not parallel, so the line intersects
        // the plane of the rectangle.

        R3::Vector<Real> diff = a_seg.GetP0() - a_rect.GetCenter();
        R3::Vector<Real> diff2 = a_seg.GetP1() - a_rect.GetCenter();

        //Either end point in the segment need to lie on opposite sides of
        //the rectangle plane
        Real dDdir = diff.Dot(a_seg.Direction());
        Real d2Ddir = diff2.Dot(a_seg.Direction());

        if (dDdir * d2Ddir < static_cast<Real>(0))
        {
          R3::Vector<Real> basis[3];  // {D, U, V}
          basis[0] = a_seg.Direction();
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
            // Segment parameter for the point of intersection.
            Real DdD0 = basis[0].Dot(a_rect.GetAxis(0));
            Real DdD1 = basis[0].Dot(a_rect.GetAxis(1));
            Real DdDiff = basis[0].Dot(diff);
            result.u = s0*DdD0 + s1*DdD1 - DdDiff;

            // Rectangle coordinates for the point of intersection.
            result.rectangleParameter[0] = s0;
            result.rectangleParameter[1] = s1;

            // The intersection point is inside or on the rectangle.
            result.point = a_seg.Origin() + result.u*basis[0];

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
    using FIRectangleSegment = impl::FIQuery<Real, 3, Rectangle<Real>, Segment<Real>>;
  }
}

#endif