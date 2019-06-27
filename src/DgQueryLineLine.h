//! @file DgQueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINELINE_H
#define DGQUERYLINELINE_H

#include "DgCPQuery.h"
#include "..\query\DgQueryCommon.h"
#include "DgLine_generic.h"
#include "DgVector_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Line_generic<Real, R>,
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from line 0 origin to closest point to line 1
        Real u0;

        //! Distance from line 1 origin to closest point to line 0
        Real u1;

        //! Closest point on line 0 to line 1
        Vector_generic<Real, R> cp0;

        //! Closest point on line 1 to line 0
        Vector_generic<Real, R> cp1;

        //! Return codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Line_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Line_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Line_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Line_generic<Real, R> const & a_line0, Line_generic<Real, R> const & a_line1)
    {
      Result result;

      Vector_generic<Real, R> o0(a_line0.Origin());
      Vector_generic<Real, R> o1(a_line1.Origin());
      Vector_generic<Real, R> d0(a_line0.Direction());
      Vector_generic<Real, R> d1(a_line1.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(o0 - o1);
      Real a = d0.Dot(d1);
      Real b = d0.Dot(w0);
      Real c = d1.Dot(w0);
      Real d = static_cast<Real>(1.0) - a*a;

      if (Dg::IsZero(d))
      {
        result.u0 = static_cast<Real>(0.0);
        result.u1 = c;
        result.code = QueryCode::QC_Parallel;
      }
      else
      {
        result.u0 = ((a*c - b) / d);
        result.u1 = ((c - a*b) / d);
        result.code = QueryCode::QC_Success;
      }

      result.cp0 = o0 + result.u0 * d0;
      result.cp1 = o1 + result.u1 * d1;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif