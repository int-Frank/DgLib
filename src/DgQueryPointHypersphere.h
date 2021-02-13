//@group Math/Geometry/Query

#ifndef DGQUERYPOINTHYPERSPHERE_H
#define DGQUERYPOINTHYPERSPHERE_H

#include "DgMath.h"
#include "DgQuery.h"
#include "DgHypersphere.h"
#include "DgVector.h"

namespace Dg
{
  //Assumes spheres are not intersecting
  template <typename Real, int R>
  class Query<QueryType::ClosestPointApproach, Real, R, Vector<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
      Real t;         // Time of contact
      QueryCode code; // Intersecting, NoIntersecting
    };

    Result operator()(Vector<Real, R> const & a_p, Vector<Real, R> const & a_vp,
                      Hypersphere<Real, R> const & a_s, Vector<Real, R> const & a_vs);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CPA2PointDisk = Query<QueryType::ClosestPointApproach, Real, 2, Vector2<Real>, Disk2<Real>>;

  template<typename Real>
  using CPA3PointSphere = Query<QueryType::ClosestPointApproach, Real, 3, Vector3<Real>, Sphere<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::ClosestPointApproach, Real, R, Vector<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::ClosestPointApproach, Real, R, Vector<Real, R>, Hypersphere<Real, R>>::operator()
    (Vector<Real, R> const & a_point, Vector<Real, R> const & a_vp,
     Hypersphere<Real, R> const & a_sphere, Vector<Real, R> const & a_vs)
  {
    Result result;
    Vector<Real, R> v_sp = a_vp - a_vs;
    
    //Find coefficient 'a' of the quadratic
    Real a = MagSq(v_sp);
    
    //If disk is stationary
    if (IsZero(a))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }
    
    //Find value 'b/2' of the quadratic
    Vector<Real, R> p_sp = a_point - a_sphere.Center();
    Real half_b = Dot(p_sp, v_sp);
    
    //Disk is moving away
    if (half_b >= Real(0))
    {
      result.code = QueryCode::NotIntersecting;
      return result;
    }
    
    //Find coefficient 'c' of the quadratic
    Real r_squared = a_sphere.Radius() * a_sphere.Radius();
    Real c = Dot(p_sp, p_sp) - r_squared;
    Real discriminite = half_b*half_b - a*c;
    
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