//! @file DgQueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTLINE_H
#define DGQUERYPOINTLINE_H

#include "DgCPQuery.h"
#include "..\query\DgQueryCommon.h"
#include "DgLine_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Line.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Line_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line origin to closest point to point
        Real u;

        //! Closest point on the line to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &,
                        Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Line_generic<Real, R> const & a_line)
    {
      Result result;

      Vector_generic<Real, R> w = a_point - a_line.Origin();
      result.u = w.Dot(a_line.Direction());
      result.cp = a_line.Origin() + result.u * a_line.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif