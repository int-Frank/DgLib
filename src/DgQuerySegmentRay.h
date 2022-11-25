//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYSEGMENTRAY_H
#define DGQUERYSEGMENTRAY_H

#include "DgQuery.h"
#include "DgSegment.h"
#include "DgRay.h"
#include "DgQueryPointRay.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>
  {
  public:

    struct Result
    {
      Real ur; // Distance from the ray origin to closest point to the line segment
      Real us; // Distance from the line segment origin to closest point to the ray

      Vector<Real, R> cpr; // Closest point on ray to the line segment
      Vector<Real, R> cps; // Closest point on line segment to the ray

      QueryCode code; // Success, Parallel
    };

    Result operator()(Segment<Real, R> const &, Ray<Real, R> const &);
  };

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 2, Segment<Real, 2>, Ray<Real, 2>>
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
          Real us;
          Real ur;
        } pointResult;

        // code = Overlapping
        struct SegmentResult
        {
          Segment2<Real> segment;
          Real us_to_s0;
          Real us_to_s1;
          Real ur_to_s0;
          Real ur_to_s1;
        } segmentResult;
      };

      Result() {}
      ~Result() {}

      Result &operator=(Result const &);
      Result(Result const &);
    };

    Result operator()(Segment<Real, 2> const &, Ray<Real, 2> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using FI2SegmentRay = Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>;

  template<typename Real>
  using CP2SegmentRay = Query<QueryType::ClosestPoint, Real, 2, Segment2<Real>, Ray2<Real>>;

  template<typename Real>
  using CP3SegmentRay = Query<QueryType::ClosestPoint, Real, 3, Segment3<Real>, Ray3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>::Result::Result
  (Result const &other)
  {
    code = other.code;
    if (other.code == Dg::QueryCode::Intersecting)
      pointResult = other.pointResult;
    else if (other.code == Dg::QueryCode::Overlapping)
      segmentResult = other.segmentResult;
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>::Result &
    Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>::Result::operator=
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
  typename Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Segment2<Real>, Ray2<Real>>::operator()
    (Segment2<Real> const &s, Ray2<Real> const &r)
  {
    Result result;
    result.code = QueryCode::NotIntersecting;

    Vector2<Real> w = s.GetP0() - r.Origin();

    Real denom = (r.Direction().y() * s.Vect().x()) - (r.Direction().x() * s.Vect().y());
    Real num_s = (r.Direction().x() * w.y()) - (r.Direction().y() * w.x());
    Real num_r = (s.Vect().x() * w.y()) - (s.Vect().y() * w.x());

    if (IsZero(denom) && (IsZero(num_s) || IsZero(num_r)))
    {
      Vector2<Real> wr0 = s.GetP0() - r.Origin();
      Vector2<Real> wr1 = s.GetP1() - r.Origin();

      result.segmentResult.ur_to_s0 = Dot(wr0, r.Direction());
      result.segmentResult.ur_to_s1 = Dot(wr1, r.Direction());

      if (result.segmentResult.ur_to_s0 > Real(0) || result.segmentResult.ur_to_s1 > Real(0))
      {
        result.code = QueryCode::Overlapping;

        Vector2<Real> p0, p1;

        if (result.segmentResult.ur_to_s0 > Real(0))
        {
          p0 = s.GetP0();
          result.segmentResult.us_to_s0 = Real(0);
        }
        else
        {
          p0 = r.Origin();
          result.segmentResult.us_to_s0 = Dot(-wr0, s.Vect()) / MagSq(s.Vect());
          result.segmentResult.ur_to_s0 = Real(0);
        }

        if (result.segmentResult.ur_to_s1 > Real(0))
        {
          p1 = s.GetP1();
          result.segmentResult.us_to_s1 = Real(1);
        }
        else
        {
          p1 = r.Origin();
          result.segmentResult.us_to_s1 = Dot(-wr0, s.Vect()) / MagSq(s.Vect());
          result.segmentResult.ur_to_s1 = Real(0);
        }

        result.segmentResult.segment.Set(p0, p1);
      }
    }
    else if (!IsZero(denom))
    {
      result.pointResult.us = num_s / denom;
      result.pointResult.ur = num_r / denom;
      if (((result.pointResult.us >= Real(0)) && (result.pointResult.us <= Real(1)))
        && ((result.pointResult.ur >= Real(0))))
      {
        result.code = QueryCode::Intersecting;
        result.pointResult.point = s.GetP0() + result.pointResult.us * s.Vect();
      }
    }
    return result;
  }

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Segment<Real, R>, Ray<Real, R>>::operator()
    (Segment<Real, R> const & a_seg, Ray<Real, R> const & a_ray)
  {
    Result result;
    result.code = QueryCode::Success;

    Vector<Real, R> ds(a_seg.Vect());
    Vector<Real, R> os(a_seg.Origin());
    Vector<Real, R> or (a_ray.Origin());
    Vector<Real, R> dr(a_ray.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(os - or);
    Real a = Dot(ds, ds);
    Real b = Dot(ds, dr);
    Real c = Dot(ds, w0);
    Real d = Dot(dr, w0);
    Real denom = a - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      if (IsZero(a))
      {
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>> query;
        Query<QueryType::ClosestPoint, Real, R, Vector<Real, R>, Ray<Real, R>>::Result res = query(os, a_ray);
        result.ur = res.u;
        result.us = static_cast<Real>(0);
        result.cpr = res.cp;
        result.cps = os;
        return result;
      }

      // clamp result.uls to 0
      sd = td = static_cast<Real>(1);
      sn = static_cast<Real>(0);
      tn = d;

      //Does the live segment overlap the ray?
      Vector<Real, R> w1((os + ds) - or);
      if (!(d < static_cast<Real>(0) && Dot(w1, dr) < static_cast<Real>(0)))
        result.code = QueryCode::Overlapping;
    }
    else
    {
      // clamp result.uls within [0,1]
      sd = td = denom;
      sn = b*d - c;
      tn = a*d - b*c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0))
      {
        sn = static_cast<Real>(0);
        tn = d;
        td = static_cast<Real>(1);
      }
      // clamp result.uls to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = d + b;
        td = static_cast<Real>(1);
      }
    }

    // clamp result.ur within [0,+inf]
    // clamp result.ur to 0
    if (tn < static_cast<Real>(0))
    {
      result.ur = static_cast<Real>(0);
     
      if (-c < static_cast<Real>(0))  // clamp result.uls to 0
        result.us = static_cast<Real>(0);
      else if (-c > a)                // clamp result.uls to 1
        result.us = static_cast<Real>(1);
      else
        result.us = -c / a;
    }
    else
    {
      result.ur = tn / td;
      result.us = sn / sd;
    }

    result.cps = os + result.us*ds;
    result.cpr = or +result.ur*dr;
    return result;
  }
}

#endif