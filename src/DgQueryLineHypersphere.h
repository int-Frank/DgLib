//! @file DgR3QueryLineBall.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINEHYPERSPHERE_H
#define DGQUERYLINEHYPERSPHERE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "../query/DgQueryCommon.h"
#include "DgLine_generic.h"
#include "DgHypersphere_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a sphere.
    template <typename Real, int R>
    class TIQuery<Real, R,
                  Line_generic<Real, R>, 
                  Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects sphere.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Line_generic<Real, R> const &, Hypersphere_generic<Real, R> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line and a sphere.
    template<typename Real, int R>
    class FIQuery<Real, R,
                  Line_generic<Real, R>, 
                  Hypersphere_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersection from u0. Not set if line does not intersect sphere.
        Vector_generic<Real, R> p0;

        //! Point of intersection from u1. Not set if line does not intersect sphere.
        Vector_generic<Real, R> p1;

        //! Distance from the line origin to the first point of intersection. Not set if line does not intersect sphere.
        Real u0;

        //! Distance from the line origin to the second point of intersection. Not set if line does not intersect sphere.
        Real u1;

        //! Return code. Codes include:
        //! Intersecting, NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(Line_generic<Real, R> const &, Hypersphere_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename TIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::Result
      TIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line, Hypersphere_generic<Real, R> const & a_sphere)
    {
      Result result;
      Vector_generic<Real, R> w0(a_line.Origin() - a_sphere.Center());
      Real a = a_line.Direction().LengthSquared();
      Real b = w0.Dot(a_line.Direction());
      Real c = w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius();
      result.isIntersecting = (b*b >= a*c);
      return result;
    } //End: CPQuery::operator()


      //--------------------------------------------------------------------------------
      //	@	CPQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename FIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::Result
      FIQuery<Real, R, Line_generic<Real, R>, Hypersphere_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line, Hypersphere_generic<Real, R> const & a_sphere)
    {
      Result result;
      Vector_generic<Real, R> w0(a_line.Origin() - a_sphere.Center());
      Real a = a_line.Direction().LengthSquared();
      Real b = static_cast<Real>(2.0) * w0.Dot(a_line.Direction());
      Real c = w0.LengthSquared() - a_sphere.Radius() * a_sphere.Radius();

      Real discr = b*b - static_cast<Real>(4.0)*a*c;
      if (discr < static_cast<Real>(0.0))
      {
        result.code = QueryCode::NotIntersecting;
      }
      else
      {
        Real d = sqrt(discr);
        Real e = static_cast<Real>(1.0) / (static_cast<Real>(2.0) * a);
        result.u0 = (-b - d) * e;
        result.u1 = (-b + d) * e;
        result.p0 = a_line.Origin() + result.u0 * a_line.Direction();
        result.p1 = a_line.Origin() + result.u1 * a_line.Direction();
        result.code = QueryCode::Intersecting;
      }

      return result;
    } //End: CPQuery::operator()
  }
}

#endif