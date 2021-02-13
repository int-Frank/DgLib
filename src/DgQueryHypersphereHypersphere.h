//@group Math/Geometry/Query

#ifndef DGQUERYHYPERSPHEREHYPERSPHERE_H
#define DGQUERYHYPERSPHEREHYPERSPHERE_H

#include "DgMath.h"
#include "DgQuery.h"
#include "DgHypersphere.h"
#include "DgVector.h"

namespace Dg
{
  //Assumes spheres are not intersecting
  template <typename Real, int R>
  class Query<QueryType::ClosestPointApproach, Real, R, Hypersphere<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
      Real t;         // Time of contact
      QueryCode code; // Intersecting, NoIntersecting
    };

    Result operator()(Hypersphere<Real, R> const & a_s0, Vector<Real, R> const & a_v0,
                      Hypersphere<Real, R> const & a_s1, Vector<Real, R> const & a_v1);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CPA2DiskDisk = Query<QueryType::ClosestPointApproach, Real, 2, Disk2<Real>, Disk2<Real>>;

  template<typename Real>
  using CPA3SphereSphere = Query<QueryType::ClosestPointApproach, Real, 3, Sphere<Real>, Sphere<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPointApproach, Real, R, Hypersphere<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::ClosestPointApproach, Real, R, Hypersphere<Real, R>, Hypersphere<Real, R>>::operator()
    (Hypersphere<Real, R> const & a_s0, Vector<Real, R> const & a_v0,
     Hypersphere<Real, R> const & a_s1, Vector<Real, R> const & a_v1)
  {
    /*
      Because our equation will be in the form

          at^2 + bt + c = 0

        where a = v_ss.v_ss
              b = 2(v_ss . p_ss)
              c = p_ss.p_ss - r^2

      we see that (v_ss . p_ss) == b/2

      our solution becomes:

        -2(b/2) +- sqrt((2 * b/2)^2 - 4ac)
        ----------------------------------
                      2a

    which reduces to

        -(b/2) +- sqrt((b/2)^2 - ac)
        ----------------------------
                    a
    */

    Result result;
    Vector<Real, R> v_01 = a_v1 - a_v0;

    //Find coefficient 'a' of the quadratic
    Real a = MagSq(v_01);

    //If disk is stationary
    if (IsZero(a))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }

    //Find value 'b/2' of the quadratic
    Vector<Real, R> p_01 = a_s1.Center() - a_s0.Center();
    Real half_b = Dot(p_01, v_01);

    //TODO Maybe we want to know negative values of t.
    //     Add this as a template option
    //Disk is moving away
    if (half_b >= Real(0))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }

    //Find coefficient 'c' of the quadratic
    Real r_squared = a_s0.Radius() + a_s1.Radius();
    r_squared *= r_squared;

    Real c = Dot(p_01, p_01) - r_squared;
    Real discriminite = half_b * half_b - a*c;

    //Disk will come closer, but not collide
    if (discriminite < Real(0))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }

    //Find the first collision time
    result.t = (-half_b - sqrt(discriminite)) / a;
    result.code = QueryCode::Intersecting;
    return result;
  }
}
#endif