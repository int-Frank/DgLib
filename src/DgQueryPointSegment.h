//! @file DgQueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTSEGMENT_H
#define DGQUERYPOINTSEGMENT_H

#include "DgCPQuery.h"
#include "DgTIQuery.h"
#include "..\query\DgQueryCommon.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Segment.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Segment_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line segment origin to closest point to point
        Real u;

        //! Closest point on the line segment to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &, Segment_generic<Real, R> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Intersection test: Segment, Segment
    template <typename Real>
    class TIQuery<Real, 2,
                  Vector_generic<Real, 2>,
                  Segment_generic<Real, 2>>
    {
    public:

      //! Query return data
      struct Result
      {
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Vector_generic<Real, 2> const &,
                        Segment_generic<Real, 2> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::Result
      TIQuery<Real, 2, Vector_generic<Real, 2>, Segment_generic<Real, 2>>::operator()
      (Vector_generic<Real, 2> const & a_point, Segment_generic<Real, 2> const & a_seg)
    {
      Result result;
      result.isIntersecting = false;

      Vector_generic<Real, 2> w = a_point - a_seg.GetP0();

      Real proj = w.Dot(a_seg.Direction());

      if (proj > static_cast<Real>(0) && proj < static_cast<Real>(1))
      {
        Real perpDot = w.PerpDot(a_seg.Direction());
        if (IsZero(w.PerpDot(a_seg.Direction())))
        {
          result.isIntersecting = true;
        }
      }
      return result;
    }

    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Segment_generic<Real, R> const & a_seg)
    {
      Result result;
      Vector_generic<Real, R> w = a_point - a_seg.Origin();

      Real proj = w.Dot(a_seg.Direction());

      if (proj < Dg::Constants<Real>::EPSILON)
      {
        result.u = static_cast<Real>(0.0);
      }
      else
      {
        Real vsq = a_seg.LengthSquared();

        if (proj >= vsq)
        {
          result.u = static_cast<Real>(1.0);
        }
        else
        {
          result.u = (proj / vsq);
        }
      }

      result.cp = a_seg.Origin() + result.u * a_seg.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif