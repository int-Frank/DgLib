//! @file DgQuerySegmentSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYSEGMENTSEGMENT_H
#define DGQUERYSEGMENTSEGMENT_H

#include "DgCPQuery.h"
#include "DgTIQuery.h"
#include "DgQueryPointSegment.h"
#include "..\query\DgQueryCommon.h"
#include "DgVector_generic.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line Segment, Line Segment
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Segment_generic<Real, R>,
                  Segment_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Fraction along segment 0 to closest point to the segment 1
        Real u0;

        //! Fraction along segment 1 to closest point to the segment 0
        Real u1;

        //! Closest point on segment 0 to segment 1
        Vector_generic<Real, R> cp0;

        //! Closest point on segment 1 to segment 0
        Vector_generic<Real, R> cp1;

        //! Return code. Codes include:
        //! Success, Overlapping
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, R> const &, Segment_generic<Real, R> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Intersection test: Segment, Segment
    template <typename Real>
    class TIQuery<Real, 2, 
                  Segment_generic<Real, 2>, 
                  Segment_generic<Real, 2>>
    {
    public:

      //! Query return data
      struct Result
      {
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, 2> const &, 
                        Segment_generic<Real, 2> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 2, Segment_generic<Real, 2>, Segment_generic<Real, 2>>::Result
      TIQuery<Real, 2, Segment_generic<Real, 2>, Segment_generic<Real, 2>>::operator()
      (Segment_generic<Real, 2> const & a_seg0, Segment_generic<Real, 2> const & a_seg1)
    {
      Result result;

      Vector_generic<Real, 2> const & dir0 = a_seg0.Direction();
      Vector_generic<Real, 2> const & dir1 = a_seg1.Direction();

      Vector_generic<Real, 2> w = a_seg0.GetP0() - a_seg1.GetP0();
      Real denom = a_seg0.Direction().PerpDot(a_seg1.Direction());
      Real u0_numerator = a_seg1.Direction().PerpDot(w);
      Real u1_numerator = a_seg0.Direction().PerpDot(w);

      if (Dg::IsZero(denom))
      {
        //Either or both segment lengths are zero
        if (dir0.IsZero())
        {
          if (dir1.IsZero())
          {
            result.isIntersecting = (a_seg0.GetP0() == a_seg1.GetP0());
          }
          else
          {
            TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>> query;
            TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::Result qRes = query(a_seg0.GetP0(), a_seg1);
            result.isIntersecting = qRes.isIntersecting;
          }
        }
        else if (dir1.IsZero()) 
        {
          TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>> query;
          TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::Result qRes = query(a_seg1.GetP0(), a_seg0);
          result.isIntersecting = qRes.isIntersecting;
        }

        //Parallel
        else if (!Dg::IsZero(u0_numerator))
        {
          result.isIntersecting = false;
        }

        //Segments lie on the same line
        else
        {
          Vector_generic<Real, 2>  v0 = a_seg0.Direction() / a_seg0.Direction().LengthSquared();
          Vector_generic<Real, 2>  v1 = a_seg1.Direction() / a_seg1.Direction().LengthSquared();
          Vector_generic<Real, 2> q = a_seg1.GetP0() - a_seg1.GetP0();

          result.isIntersecting = (IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(w, v1))
            || IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(q, v1))
            || IsInRange(static_cast<Real>(0), static_cast<Real>(1), Dot(-w, v0)));
        }
      }
      else
      {
        Real u0 = u0_numerator / denom;
        Real u1 = u1_numerator / denom;
        result.isIntersecting = (IsInRange(static_cast<Real>(0), static_cast<Real>(1), u0)
          && IsInRange(static_cast<Real>(0), static_cast<Real>(1), u1));
      }
      return result;
    }

    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Segment_generic<Real, R>, Segment_generic<Real, R>>::Result
      CPQuery<Real, R, Segment_generic<Real, R>, Segment_generic<Real, R>>::operator()
      (Segment_generic<Real, R> const & a_seg0, Segment_generic<Real, R> const & a_seg1)
    {
      Result result;
      result.code = QueryCode::QC_Success;

      Vector_generic<Real, R> o0(a_seg0.Origin());
      Vector_generic<Real, R> o1(a_seg1.Origin());
      Vector_generic<Real, R> d0(a_seg0.Direction());
      Vector_generic<Real, R> d1(a_seg1.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(o0 - o1);
      Real a = d0.Dot(d0);
      Real b = d0.Dot(d1);
      Real c = d1.Dot(d1);
      Real d = d0.Dot(w0);
      Real e = d1.Dot(w0);
      Real denom = a*c - b*b;

      Real sn, sd, tn, td;

      // if denom is zero, try finding closest point on segment1 to origin0
      if (Dg::IsZero(denom))
      {
        //Either or both segment lengths are zero
        if (IsZero(a))
        {
          if (IsZero(c))
          {
            result.u0 = static_cast<Real>(0);
            result.u1 = static_cast<Real>(0);
            result.cp0 = o0;
            result.cp1 = o1;
          }
          else
          {
            CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>> query;
            CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::Result qRes = query(o0, a_seg1);
            result.u0 = static_cast<Real>(0);
            result.u1 = qRes.u;
            result.cp0 = o0;
            result.cp1 = qRes.cp;
          }
          return result;
        }
        else if (IsZero(c))
        {
          CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>> query;
          CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::Result qRes = query(o1, a_seg0);
          result.u0 = qRes.u;
          result.u1 = static_cast<Real>(0);
          result.cp0 = qRes.cp;
          result.cp1 = o0;
          return result;
        }

        // clamp result.u0 to 0
        sd = td = c;
        sn = static_cast<Real>(0.0);
        tn = e;

        //Do the line segments overlap?
        Vector_generic<Real, R> w1((o0 + d0) - o1);
        Vector_generic<Real, R> w2(o0 - (o1 + d1));
        Vector_generic<Real, R> w3((o0 + d0) - (o1 + d1));
        bool bse = (e < static_cast<Real>(0.0));
        if (!(
          bse == (w1.Dot(d1) < static_cast<Real>(0.0)) &&
          bse == (w2.Dot(d1) < static_cast<Real>(0.0)) &&
          bse == (w3.Dot(d1) < static_cast<Real>(0.0))
          ))
        {
          result.code = QueryCode::QC_Overlapping;
        }
      }
      else
      {
        // clamp result.u0 within [0,1]
        sd = td = denom;
        sn = b*e - c*d;
        tn = a*e - b*d;

        // clamp result.u0 to 0
        if (sn < static_cast<Real>(0.0))
        {
          sn = static_cast<Real>(0.0);
          tn = e;
          td = c;
        }
        // clamp result.u0 to 1
        else if (sn > sd)
        {
          sn = sd;
          tn = e + b;
          td = c;
        }
      }

      // clamp result.u1 within [0,1]
      // clamp result.u1 to 0
      if (tn < static_cast<Real>(0.0))
      {
        result.u1 = static_cast<Real>(0.0);
        // clamp result.u0 to 0
        if (-d < static_cast<Real>(0.0))
        {
          result.u0 = static_cast<Real>(0.0);
        }
        // clamp result.u0 to 1
        else if (-d > a)
        {
          result.u0 = static_cast<Real>(1.0);
        }
        else
        {
          result.u0 = -d / a;
        }
      }
      // clamp result.u1 to 1
      else if (tn > td)
      {
        result.u1 = static_cast<Real>(1.0);
        // clamp result.u0 to 0
        if ((-d + b) < static_cast<Real>(0.0))
        {
          result.u0 = static_cast<Real>(0.0);
        }
        // clamp result.u0 to 1
        else if ((-d + b) > a)
        {
          result.u0 = static_cast<Real>(1.0);
        }
        else
        {
          result.u0 = (-d + b) / a;
        }
      }
      else
      {
        result.u1 = tn / td;
        result.u0 = sn / sd;
      }

      result.cp0 = o0 + result.u0*d0;
      result.cp1 = o1 + result.u1*d1;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif