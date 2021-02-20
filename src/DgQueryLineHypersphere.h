//@group Math/Geometry/Query

// This code is derived from:
//    'Mathematics for Games and Interactive Applications'
//     Authors: James M. Van Verth and Lars M. Bishop.

#ifndef DGQUERYLINEHYPERSPHERE_H
#define DGQUERYLINEHYPERSPHERE_H

#include "DgQuery.h"
#include "DgLine.h"
#include "DgHypersphere.h"
#include "DgMath.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::TestForIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Line<Real, R> const &, Hypersphere<Real, R> const &);
  };

  template<typename Real, int R>
  class Query<QueryType::FindIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
       
      Vector<Real, R> p0; // Point of intersection from u0. Not set if line does not intersect sphere.
      Vector<Real, R> p1; // Point of intersection from u1. Not set if line does not intersect sphere.

      Real u0; // Distance from the line origin to the first point of intersection. Not set if line does not intersect sphere.
      Real u1; // Distance from the line origin to the second point of intersection. Not set if line does not intersect sphere.

      QueryCode code; // Intersecting, NotIntersecting
    };

    Result operator()(Line<Real, R> const &, Hypersphere<Real, R> const &);
  };

  //Assumes spheres is not already intersecting with the line
  template <typename Real, int R>
  class Query<QueryType::ClosestPointApproach, Real, R, Line<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
      Real t; // Time until intersection
      QueryCode code; // Intersecting, NoIntersecting
    };

    Result operator()(Line<Real, R> const & a_line, Hypersphere<Real, R> const & a_sphere, Vector<Real, R> const & a_vs);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real> using TI2LineDisk     = Query<QueryType::TestForIntersection, Real, 2, Line2<Real>, Disk2<Real>>;
  template<typename Real> using FI2LineDisk     = Query<QueryType::FindIntersection, Real, 2, Line2<Real>, Disk2<Real>>;
  template<typename Real> using CPA2LineDisk    = Query<QueryType::ClosestPointApproach, Real, 2, Line2<Real>, Disk2<Real>>;
  
  template<typename Real> using TI3LineSphere   = Query<QueryType::TestForIntersection, Real, 3, Line3<Real>, Sphere<Real>>;
  template<typename Real> using FI3LineSphere   = Query<QueryType::FindIntersection, Real, 3, Line3<Real>, Sphere<Real>>;
  template<typename Real> using CPA3LineSphere  = Query<QueryType::ClosestPointApproach, Real, 3, Line3<Real>, Sphere<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::TestForIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::TestForIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>::operator()
    (Line<Real, R> const & a_line, Hypersphere<Real, R> const & a_sphere)
  {
    Result result;
    Vector<Real, R> w0(a_line.Origin() - a_sphere.Center());
    Real a = MagSq(a_line.Direction());
    Real b = Dot(w0, a_line.Direction());
    Real c = MagSq(w0) - a_sphere.Radius() * a_sphere.Radius();
    result.isIntersecting = (b*b >= a*c);
    return result;
  }

  template<typename Real, int R>
  typename Query<QueryType::FindIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::FindIntersection, Real, R, Line<Real, R>, Hypersphere<Real, R>>::operator()
    (Line<Real, R> const & a_line, Hypersphere<Real, R> const & a_sphere)
  {
    Result result;
    Vector<Real, R> w0(a_line.Origin() - a_sphere.Center());
    Real a = MagSq(a_line.Direction());
    Real b = static_cast<Real>(2) * Dot(w0, a_line.Direction());
    Real c = MagSq(w0) - a_sphere.Radius() * a_sphere.Radius();

    Real discr = b*b - static_cast<Real>(4)*a*c;
    if (discr < static_cast<Real>(0))
    {
      result.code = QueryCode::NotIntersecting;
    }
    else
    {
      Real d = sqrt(discr);
      Real e = static_cast<Real>(1) / (static_cast<Real>(2) * a);
      result.u0 = (-b - d) * e;
      result.u1 = (-b + d) * e;
      result.p0 = a_line.Origin() + result.u0 * a_line.Direction();
      result.p1 = a_line.Origin() + result.u1 * a_line.Direction();
      result.code = QueryCode::Intersecting;
    }

    return result;
  }

  template<typename Real, int R>
  typename Query<QueryType::ClosestPointApproach, Real, R, Line<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::ClosestPointApproach, Real, R, Line<Real, R>, Hypersphere<Real, R>>::operator()
    (Line<Real, R> const & a_line, Hypersphere<Real, R> const & a_sphere, Vector<Real, R> const & a_vs)
  {
    /*
    t = -a.b +- sqrt(r^2*b.b)
        ---------------------
                b.b

    where
          a = u_ls - (u_ls.u_l)*u_l
          b = a_vs - (a_vs.u_l)*u_l
    and
          u_ls = sphere center - line origin
          a_vs = sphere velocity - line velocity
    */

    Result result;

    Vector<Real, R> u_ls = a_sphere.Center() - a_line.Origin();

    Vector<Real, R> a = u_ls - Dot(u_ls, a_line.Direction()) * a_line.Direction();
    Vector<Real, R> b = a_vs - Dot(a_vs, a_line.Direction()) * a_line.Direction();

    //No relative velocity between sphere and line
    Real dot_b_b = Dot(b, b);
    if (IsZero(dot_b_b))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }

    //Sphere moving away from line
    Real dot_a_b = Dot(a, b);
    if (dot_a_b >= Real(0))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }

    Real r_squared = a_sphere.Radius() * a_sphere.Radius();
    result.t = (-dot_a_b - sqrt(r_squared * dot_b_b))/dot_b_b;
    result.code = QueryCode::Intersecting;
    return result;
  }
}

#endif