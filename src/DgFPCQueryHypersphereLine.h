#ifndef DGFPCQUERYHYPERSPHERELINE_H
#define DGFPCQUERYHYPERSPHERELINE_H

#include <cmath>
#include "DgFPCQuery.h"
#include "DgMath.h"
#include "DgHypersphere_generic.h"
#include "DgLine_generic.h"
#include "DgVector_generic.h"
#include "../query/DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    //Assumes spheres is not already intersecting with the line
    template <typename Real, int R>
    class FPCQuery<Real, R,
                   Hypersphere_generic<Real, R>,
                   Line_generic<Real, R>>
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
      // In  this query, a_traj_l represents the velocity of the line tangental
      // to the line direction
      // TODO the velocity of the line seems redundant. When will this ever happen?
      Result operator()(Hypersphere_generic<Real, R> const & a_sphere, 
                        Vector_generic<Real, R> const & a_traj_s,
                        Line_generic<Real, R> const & a_line,
                        Vector_generic<Real, R> const & a_traj_l)
      {
        Result result;

        /*
        t = -a.b +- sqrt(r^2*b.b)
            ---------------------
                    b.b

        where 
              a = u_ls - (u_ls.u_l)*u_l
              b = v_ls - (v_ls.u_l)*u_l
        and
              u_ls = sphere center - line origin
              v_ls = sphere velocity - line velocity

        */

        Vector_generic<Real, R> u_ls = a_sphere.Center() - a_line.Origin();
        Vector_generic<Real, R> v_ls = a_traj_s - a_traj_l;

        Vector_generic<Real, R> a = u_ls - u_ls.Dot(a_line.Direction()) * a_line.Direction();
        Vector_generic<Real, R> b = v_ls - v_ls.Dot(a_line.Direction()) * a_line.Direction();

        //No relative velocity between sphere and line
        Real dot_b_b = b.Dot(b);
        //TODO This should be IsZero(dot_b_b)
        if (dot_b_b <= Real(0))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        //Sphere moving away from line
        Real dot_a_b = a.Dot(b);
        if (dot_a_b >= Real(0))
        {
          result.code = QC_NotIntersecting;
          return result;
        }

        Real r_squared = a_sphere.Radius() * a_sphere.Radius();
        result.t = (-dot_a_b - sqrt(r_squared * dot_b_b))/dot_b_b;
        result.code = QC_Intersecting;
        return result;
      }
    };
  }
}
#endif