#ifndef DGFPCQUERYPOINTHYPERSPHERE_H
#define DGFPCQUERYPOINTHYPERSPHERE_H

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
    //Assumes point lies outside of the sphere
    template <typename Real, int R>
    class FPCQuery<Real, R,
                   Hypersphere_generic<Real, R>,
                   Vector_generic<Real, R>>
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
      Result operator()(Hypersphere_generic<Real, R> const & a_sphere, 
                        Vector_generic<Real, R> const & a_trajSphere,
                        Vector_generic<Real, R> const & a_point,
                        Vector_generic<Real, R> const & a_trajPoint)
      {
        Result result;

        Vector_generic<Real, R> v_sp = a_trajPoint - a_trajSphere;

        /*
          Because our equation will always be in the form

              at^2 + bt + c = 0

            where a = v_sp.v_sp
                  b = 2(v_sp . p_sp)
                  c = p_sp.p_sp - r^2

          we see (v_sp . p_sp) == b/2

          our solution becomes:

           -2(b/2) +- sqrt((2 * b/2)^2 - 4ac)
           ----------------------------------
                         2a

        which reduces to

           -(b/2) +- sqrt((b/2)^2 - ac)
           ----------------------------
                       a
        */

        //Find coefficient 'a' of the quadratic
        Real a = v_sp.LengthSquared();

        //If disk is stationary
        if (IsZero(a))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Find value 'b/2' of the quadratic
        Vector_generic<Real, R> p_sp = a_point - a_sphere.Center();
        Real half_b = p_sp.Dot(v_sp);

        //Disk is moving away
        if (half_b >= Real(0))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Find coefficient 'c' of the quadratic
        Real r_squared = a_sphere.Radius() * a_sphere.Radius();
        Real c = p_sp.Dot(p_sp) - r_squared;
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