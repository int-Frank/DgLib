//! @file DgQueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTZCYLINDER_H
#define DGQUERYPOINTZCYLINDER_H

#include "../impl/DgCPQuery.h"
#include "DgQueryCommon.h"
#include "impl/DgSegment_generic.h"
#include "../DgR3ZCylinder.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, zCylinder.
    template <typename Real>
    class CPQuery<Real, 3,
      R3::Vector<Real>,
      R3::ZCylinder<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Closest point on the line segment to the point.
        R3::Vector<Real> cp;

        //! QC_Intersecting, QC_NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(R3::Vector<Real> const &, R3::ZCylinder<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, 3, R3::Vector<Real>, R3::ZCylinder<Real>>::Result
      CPQuery<Real, 3, R3::Vector<Real>, R3::ZCylinder<Real>>::operator()
      (R3::Vector<Real> const & a_point, R3::ZCylinder<Real> const & a_zCyl)
    {
      Result result;

      R3::Vector<Real> v = a_point - a_zCyl.GetBottomCenter();
      Real distSq = v[0] * v[0] + v[1] * v[1];
      Real rSq = a_zCyl.GetRadius() * a_zCyl.GetRadius();

      result.cp[3] = static_cast<Real>(1);

      //Inside infinite cylinder
      if (distSq <= rSq)
      {
        result.cp[0] = a_point[0];
        result.cp[1] = a_point[1];

        //Outside finite cylinder
        if (v.z() > a_zCyl.GetHeight())
        {
          result.cp[2] = a_zCyl.GetHeight() + a_zCyl.GetBottomCenter()[2];
          result.code = QC_NotIntersecting;
        }
        else if (v.z() < static_cast<Real>(0))
        {
          result.cp[2] = a_zCyl.GetBottomCenter()[2];
          result.code = QC_NotIntersecting;
        }
        else
        {
          result.cp[2] = a_point[2];
          result.code = QC_Intersecting;
        }
      }
      //Outside infinite cylinder
      else
      {
        result.code = QC_NotIntersecting;
        Real ratio = a_zCyl.GetRadius() / sqrt(distSq);
        result.cp[0] = a_zCyl.GetBottomCenter()[0] + v[0] * ratio;
        result.cp[1] = a_zCyl.GetBottomCenter()[1] + v[1] * ratio;

        if (v.z() > a_zCyl.GetHeight())
        {
          result.cp[2] = a_zCyl.GetHeight() + a_zCyl.GetBottomCenter()[2];
        }
        else if (v.z() < static_cast<Real>(0))
        {
          result.cp[2] = a_zCyl.GetBottomCenter()[2];
        }
        else
        {
          result.cp[2] = a_point[2];
        }
      }

      return result;
    } //End: CPQuery::operator()
  }

  namespace R3
  {
    template<typename Real>
    using CPPointZCylinder = impl::CPQuery<Real, 3, Vector<Real>, ZCylinder<Real>>;
  }
}

#endif