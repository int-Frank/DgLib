//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYSEGMENTSEGMENT_H
#define DGQUERYSEGMENTSEGMENT_H

#include "DgQuery.h"
#include "DgQueryPointSegment.h"
#include "DgVector.h"
#include "DgSegment.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>
  {
  public:

    struct Result
    {
      Real u0; // Fraction along segment 0 to closest point to the segment 1
      Real u1; // Fraction along segment 1 to closest point to the segment 0

      Vector<Real, R> cp0; // Closest point on segment 0 to segment 1
      Vector<Real, R> cp1; // Closest point on segment 1 to segment 0

      QueryCode code; // Success, Overlapping
    };

    Result operator()(Segment<Real, R> const &, Segment<Real, R> const &);
  };

  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Segment<Real, 2>, Segment<Real, 2>>
  {
  public:

    struct Result
    {
      QueryCode code; // Intersecting, NotIntersecting, Overlapping
    };

    Result operator()(Segment<Real, 2> const &, Segment<Real, 2> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2SegmentSegment = Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP2SegmentSegment = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP3SegmentSegment = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Segment3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  // Helper function for segments in 2D, derived from http:// paulbourke.net/geometry/pointlineplane/
  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::operator()
    (Segment2<Real> const &s0, Segment2<Real> const &s1)
  {
    Result result;
    result.code = QueryCode::NotIntersecting;
    Vector2<Real> w = s0.GetP0() - s1.GetP0();

    Real denom = (s1.Vect().y() * s0.Vect().x()) - (s1.Vect().x() * s0.Vect().y());
    Real num_a = (s1.Vect().x() * w.y()) - (s1.Vect().y() * w.x());
    Real num_b = (s0.Vect().x() * w.y()) - (s0.Vect().y() * w.x());

    if (IsZero(denom) && (IsZero(num_a) || IsZero(num_b)))
    {
      Vector2<Real> w0 = s1.GetP0() - s0.GetP0();
      Vector2<Real> w1 = s1.GetP1() - s0.GetP0();

      Real len = s0.Length();
      Real u0 = Dot(w0, s0.Vect()) / len;
      Real u1 = Dot(w1, s0.Vect()) / len;

      if (!(((u0 < Real(0)) && (u1 < Real(0))) || ((u0 > Real(1)) && (u1 > Real(1)))))
        result.code = QueryCode::Overlapping;
    }
    else if (!IsZero(denom))
    {
      Real ua = num_a / denom;
      Real ub = num_b / denom;
      if (((ua >= Real(0)) && (ua <= Real(1))) && ((ub >= Real(0)) && (ub <= Real(1))))
        result.code = QueryCode::Intersecting;
    }
    return result;
  }

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Segment<Real, R>>::operator()
    (Segment<Real, R> const & a_seg0, Segment<Real, R> const & a_seg1)
  {
    Result result;
    result.code = QueryCode::Success;

    Vector<Real, R> o0(a_seg0.Origin());
    Vector<Real, R> o1(a_seg1.Origin());
    Vector<Real, R> d0(a_seg0.Vect());
    Vector<Real, R> d1(a_seg1.Vect());

    //compute intermediate parameters
    Vector<Real, R> w0(o0 - o1);
    Real a = Dot(d0, d0);
    Real b = Dot(d0, d1);
    Real c = Dot(d1, d1);
    Real d = Dot(d0, w0);
    Real e = Dot(d1, w0);
    Real denom = a*c - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      // clamp result.u0 to 0
      sd = td = c;
      sn = static_cast<Real>(0);
      tn = e;

      //Do the line segments overlap?
      Vector<Real, R> w1((o0 + d0) - o1);
      Vector<Real, R> w2(o0 - (o1 + d1));
      Vector<Real, R> w3((o0 + d0) - (o1 + d1));
      bool bse = (e < static_cast<Real>(0));
      if (!(
        bse == (Dot(w1, d1) < static_cast<Real>(0)) &&
        bse == (Dot(w2, d1) < static_cast<Real>(0)) &&
        bse == (Dot(w3, d1) < static_cast<Real>(0))
        ))
      {
        result.code = QueryCode::Overlapping;
      }
    }
    else
    {
      // clamp result.u0 within [0,1]
      sd = td = denom;
      sn = b*e - c*d;
      tn = a*e - b*d;

      // clamp result.u0 to 0
      if (sn < static_cast<Real>(0))
      {
        sn = static_cast<Real>(0);
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
    if (tn < static_cast<Real>(0))
    {
      result.u1 = static_cast<Real>(0);
     
      if (-d < static_cast<Real>(0))    // clamp result.u0 to 0
        result.u0 = static_cast<Real>(0);
      else if (-d > a)                  // clamp result.u0 to 1
        result.u0 = static_cast<Real>(1);
      else
        result.u0 = -d / a;
    }
    // clamp result.u1 to 1
    else if (tn > td)
    {
      result.u1 = static_cast<Real>(1.0);
     
      if ((-d + b) < static_cast<Real>(0))  // clamp result.u0 to 0
        result.u0 = static_cast<Real>(0);
      else if ((-d + b) > a)                // clamp result.u0 to 1
        result.u0 = static_cast<Real>(1);
      else
        result.u0 = (-d + b) / a;
    }
    else
    {
      result.u1 = tn / td;
      result.u0 = sn / sd;
    }

    result.cp0 = o0 + result.u0*d0;
    result.cp1 = o1 + result.u1*d1;
    return result;
  }
}

#endif