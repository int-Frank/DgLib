//! @file DgR3QueryRayLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYRAYLINE_H
#define DGQUERYRAYLINE_H

#include "../query/DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgLine_generic.h"
#include "DgRay_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Ray, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Ray_generic<Real, R>, 
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the line origin to closest point to the ray
        Real ul;

        //! Distance from the ray origin to closest point to the line
        Real ur;

        //! Closest point on line to the ray
        Vector_generic<Real, R> cpl;

        //! Closest point on ray to the line
        Vector_generic<Real, R> cpr;

        //! Return code. Codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Ray_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Ray_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Ray_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Ray_generic<Real, R> const & a_ray, Line_generic<Real, R> const & a_line)
    {
      Vector_generic<Real, R> or (a_ray.Origin());
      Vector_generic<Real, R> ol(a_line.Origin());
      Vector_generic<Real, R> dr(a_ray.Direction());
      Vector_generic<Real, R> dl(a_line.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> diff(or -ol);
      Real a = dr.Dot(dl);
      Real b = dl.Dot(diff);
      Real c = static_cast<Real>(1.0) - a * a;

      Result result;

      //if c is zero, try finding closest point on the ray to the line origin
      if (Dg::IsZero(c))
      {
        //compute closest points
        result.ur = static_cast<Real>(0.0);
        result.ul = b;
        result.code = QueryCode::QC_Parallel;
      }
      else
      {
        // clamp ur within [0,+inf]
        Real d = dr.Dot(diff);
        Real sn = a * b - d;

        // clamp ur to 0
        if (sn < static_cast<Real>(0.0))
        {
          result.ur = static_cast<Real>(0.0);
          result.ul = b;
        }
        else
        {
          Real denom = static_cast<Real>(1.0) / c;
          result.ur = sn * denom;
          result.ul = (b - a*d) * denom;
        }

        result.code = QC_Success;
      }

      result.cpr = or +result.ur*dr;
      result.cpl = ol + result.ul*dl;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif