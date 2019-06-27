#ifndef DGFPCQUERYHYPERSPHEREHYPERSPHERE_H
#define DGFPCQUERYHYPERSPHEREHYPERSPHERE_H

#include <cmath>
#include "DgMath.h"
#include "DgFPCQuery.h"
#include "DgHypersphere_generic.h"
#include "DgVector_generic.h"
#include "../query/DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    //Assumes spheres are not already intersecting
    template <typename Real, int R>
    class FPCQuery<Real, R,
                   Hypersphere_generic<Real, R>,
                   Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Time until intersection
        Real t;

        //! Return code. Codes include:
        //! Intersecting, NoIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(Hypersphere_generic<Real, R> const & a_sphere_0, 
                        Vector_generic<Real, R> const & a_traj_0,
                        Hypersphere_generic<Real, R> const & a_sphere_1,
                        Vector_generic<Real, R> const & a_traj_1)
      {
        Result result;

        Vector_generic<Real, R> v_01 = a_traj_1 - a_traj_0;

        //Find coefficient 'a' of the quadratic
        Real a = v_01.LengthSquared();

        //If disk is stationary
        if (IsZero(a))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Find value 'b/2' of the quadratic
        Vector_generic<Real, R> p_01 = a_sphere_1.Center() - a_sphere_0.Center();
        Real half_b = p_01.Dot(v_01);

        //TODO Maybe we want to know negative values of t.
        //     Add this as a template option
        //Disk is moving away
        if (half_b >= Real(0))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Find coefficient 'c' of the quadratic
        Real r_squared = a_sphere_0.Radius() + a_sphere_1.Radius();
        r_squared *= r_squared;

        Real c = p_01.Dot(p_01) - r_squared;
        Real discriminite = half_b*half_b - a*c;

        //Disk will come closer, but not collide
        if (discriminite < Real(0))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Find the first collision time
        result.t = (-half_b - sqrt(discriminite)) / a;
        result.code = QC_Intersecting;
        return result;
      }
    };
  }
}
#endif