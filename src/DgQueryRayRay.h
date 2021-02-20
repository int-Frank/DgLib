//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYRAYRAY_H
#define DGQUERYRAYRAY_H

#include "DgQuery.h"
#include "DgRay.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Ray<Real, R>>
  {
  public:

    struct Result
    {
      Real u0; // Distance from ray 0 origin to closest point to ray 1
      Real u1; // Distance from ray 1 origin to closest point to ray 0

      Vector<Real, R> cp0; // Closest point on ray 0 to ray 1
      Vector<Real, R> cp1; // Closest point on ray 1 to ray 0

      QueryCode code; // Success, Overlapping
    };

    Result operator()(Ray<Real, R> const &, Ray<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP2RayRay = Query<QueryType::ClosestPoint, Real, 2, Ray2<Real>, Ray2<Real>>;

  template<typename Real>
  using CP3RayRay = Query<QueryType::ClosestPoint, Real, 3, Ray3<Real>, Ray3<Real>>;

  //--------------------------------------------------------------------------------
  //	Implementation
  //--------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Ray<Real, R>>::Result
    Query<QueryType::ClosestPoint, Real, R, Ray<Real, R>, Ray<Real, R>>::operator()
    (Ray<Real, R> const & a_ray0, Ray<Real, R> const & a_ray1)
  {
    Result result;
    result.code = QueryCode::Success;

    Vector<Real, R> o0(a_ray0.Origin());
    Vector<Real, R> o1(a_ray1.Origin());
    Vector<Real, R> d0(a_ray0.Direction());
    Vector<Real, R> d1(a_ray1.Direction());

    //compute intermediate parameters
    Vector<Real, R> w0(o0 - o1);
    Real a = Dot(d0, d1);
    Real b = Dot(d0, w0);
    Real c = Dot(d1, w0);
    Real d = static_cast<Real>(1) - a * a;

    //parameters to compute result.u0 and result.u1
    Real sn, sd, tn, td;

    //if denom is zero, try finding closest point on ray1 to origin of ray0
    if (Dg::IsZero(d))
    {
      //clamp result.u0 to 0
      sd = td = static_cast<Real>(1);
      sn = static_cast<Real>(0);
      tn = c;

      //Do the rays overlap?
      if (!(c < static_cast<Real>(0) &&
        b > static_cast<Real>(0)))
      {
        result.code = QueryCode::Overlapping;
      }
    }
    else
    {
      //clamp result.u0 within[0, +inf]
      sd = td = d;
      sn = a*c - b;
      tn = c - a*b;

      //clamp result.u0 to 0
      if (sn < static_cast<Real>(0))
      {
        sn = static_cast<Real>(0);
        tn = c;
        td = static_cast<Real>(1);
      }
    }

    //clamp result.u1 within [0, +inf]
    //clamp result.u1 to 0
    if (tn < static_cast<Real>(0))
    {
      result.u1 = static_cast<Real>(0);

      //clamp result.u0 to 0
      if (-b < static_cast<Real>(0))
      {
        result.u0 = static_cast<Real>(0);
      }
      else
      {
        result.u0 = -b;
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
  }
}

#endif