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

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 2, Segment<Real, 2>, Segment<Real, 2>>
  {
  public:

    struct Result
    {
      QueryCode code; // Intersecting, NotIntersecting, Overlapping

      union
      {
        // code = Intersecting
        struct PointResult
        {
          Vector2<Real> point;
          Real u0;
          Real u1;
        } pointResult;

        // code = Overlapping
        struct SegmentResult
        {
          Segment2<Real> segment;
          Real u0_to_s0;
          Real u0_to_s1;
          Real u1_to_s0;
          Real u1_to_s1;
        } segmentResult;
      };

      Result() {}
      ~Result() {}

      Result &operator=(Result const &);
      Result(Result const &);
    };

    Result operator()(Segment<Real, 2> const &, Segment<Real, 2> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2SegmentSegment = Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using FI2SegmentSegment = Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP2SegmentSegment = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Segment2<Real>>;

  template<typename Real>
  using CP3SegmentSegment = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Segment3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result::Result
    (Result const &other)
  {
    code = other.code;
    if (other.code == Dg::QueryCode::Intersecting)
      pointResult = other.pointResult;
    else if (other.code == Dg::QueryCode::Overlapping)
      segmentResult = other.segmentResult;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result &
    Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result::operator=
    (Result const &other)
  {
    if (this != &other)
    {
      code = other.code;
      if (other.code == Dg::QueryCode::Intersecting)
        pointResult = other.pointResult;
      else if (other.code == Dg::QueryCode::Overlapping)
        segmentResult = other.segmentResult;
    }

    return *this;
  }

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::operator()
    (Segment2<Real> const &s0, Segment2<Real> const &s1)
  {
    Result result;
    result.code = QueryCode::NotIntersecting;

    // We do this check to account for floating point precision errors
    // when the segments are almost parallel
    Real distSq = Dg::MagSq(s0.GetCenter() - s1.GetCenter());
    Real lenSq = s0.Length() / Real(2) + s1.Length() / Real(2);
    lenSq *= lenSq;
    if (distSq > lenSq)
      return result;

    Vector2<Real> w = s0.GetP0() - s1.GetP0();

    Real denom = (s1.Vect().y() * s0.Vect().x()) - (s1.Vect().x() * s0.Vect().y());
    Real num_0 = (s1.Vect().x() * w.y()) - (s1.Vect().y() * w.x());
    Real num_1 = (s0.Vect().x() * w.y()) - (s0.Vect().y() * w.x());

    if (denom == Real(0) && (num_0 == Real(0) || num_1 == Real(0)))
    {
      Vector2<Real> w00 = s1.GetP0() - s0.GetP0();
      Vector2<Real> w01 = s1.GetP1() - s0.GetP0();

      Real len0Sq = MagSq(s0.Vect());
      Real u00 = Dot(w00, s0.Vect()) / len0Sq;
      Real u01 = Dot(w01, s0.Vect()) / len0Sq;

      if (!((u00 < Real(0) && u01 < Real(0)) || (u00 > Real(1) && u01 > Real(1))))
        result.code = QueryCode::Overlapping;
    }
    else if (denom != Real(0))
    {
      Real u0 = num_0 / denom;
      Real u1 = num_1 / denom;
      if (((u0 >= Real(0)) && (u0 <= Real(1)))
        && ((u1 >= Real(0)) && (u1 <= Real(1))))
        result.code = QueryCode::Intersecting;
    }
    return result;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Segment2<Real>>::operator()
    (Segment2<Real> const &s0, Segment2<Real> const &s1)
  {
    Result result;
    result.code = QueryCode::NotIntersecting;

    // Check if s1 is either completely behind or completely in front of s1.
    // We do this first to short circuit floating point precision errors
    // when the segments are almost parallel. These variables may be used later
    // if the the segments are found to be parallel.
    Vector2<Real> w00 = s1.GetP0() - s0.GetP0();
    Vector2<Real> w01 = s1.GetP1() - s0.GetP0();

    Real len0Sq = MagSq(s0.Vect());
    Real u00 = Dot(w00, s0.Vect()) / len0Sq;
    Real u01 = Dot(w01, s0.Vect()) / len0Sq;

    if ((u00 < Real(0) && u01 < Real(0)) || (u00 > Real(1) && u01 > Real(1)))
      return result;

    Vector2<Real> w = s0.GetP0() - s1.GetP0();

    Real denom = (s1.Vect().y() * s0.Vect().x()) - (s1.Vect().x() * s0.Vect().y());
    Real num_0 = (s1.Vect().x() * w.y()) - (s1.Vect().y() * w.x());
    Real num_1 = (s0.Vect().x() * w.y()) - (s0.Vect().y() * w.x());

    if (denom == Real(0) && (num_0 == Real(0) || num_1 == Real(0)))
    {
      if (u00 > u01)
      {
        Real temp = u00;
        u00 = u01;
        u01 = temp;
      }

      u00 = Clamp(u00, Real(0), Real(1));
      u01 = Clamp(u01, Real(0), Real(1));

      Segment2<Real> seg(s0.GetP0() + s0.Vect() * u00, s0.GetP0() + s0.Vect() * u01);

      Vector2<Real> w10 = seg.GetP0() - s1.GetP0();
      Vector2<Real> w11 = seg.GetP1() - s1.GetP0();

      Real len1Sq = MagSq(s1.Vect());
      Real u10 = Dot(w10, s1.Vect()) / len1Sq;
      Real u11 = Dot(w11, s1.Vect()) / len1Sq;

      u10 = Clamp(u10, Real(0), Real(1));
      u11 = Clamp(u11, Real(0), Real(1));

      result.code = QueryCode::Overlapping;
      result.segmentResult.segment = seg;
      result.segmentResult.u0_to_s0 = u00;
      result.segmentResult.u0_to_s1 = u01;
      result.segmentResult.u1_to_s0 = u10;
      result.segmentResult.u1_to_s1 = u11;
    }
    else if (denom != Real(0))
    {
      result.pointResult.u0 = num_0 / denom;
      result.pointResult.u1 = num_1 / denom;
      if (((result.pointResult.u0 >= Real(0)) && (result.pointResult.u0 <= Real(1)))
        && ((result.pointResult.u1 >= Real(0)) && (result.pointResult.u1 <= Real(1))))
      {
        result.code = QueryCode::Intersecting;
        result.pointResult.point = s0.GetP0() + result.pointResult.u0 * s0.Vect();
      }
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