//@group Math/Geometry/Query

#ifndef DGQUERYPOINTZALIGNEDCYLINDER_H
#define DGQUERYPOINTZALIGNEDCYLINDER_H

#include "DgQuery.h"
#include "DgSegment.h"
#include "DgZAlignedCylinder.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, ZAlignedCylinder<Real>>
  {
  public:

    struct Result
    {
      Vector3<Real> cp;
      QueryCode code;
    };

    Result operator()(Vector3<Real> const &, ZAlignedCylinder<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3PointZCylinder = Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, ZAlignedCylinder<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, ZAlignedCylinder<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Vector3<Real>, ZAlignedCylinder<Real>>::operator()
    (Vector3<Real> const & a_point, ZAlignedCylinder<Real> const & a_zCyl)
  {
    Result result;

    Vector3<Real> v = a_point - a_zCyl.GetBottomCenter();
    Real distSq = v[0] * v[0] + v[1] * v[1];
    Real rSq = a_zCyl.GetRadius() * a_zCyl.GetRadius();

    //Inside infinite cylinder
    if (distSq <= rSq)
    {
      result.cp[0] = a_point[0];
      result.cp[1] = a_point[1];

      //Outside finite cylinder
      if (v[2] > a_zCyl.GetHeight())
      {
        result.cp[2] = a_zCyl.GetHeight() + a_zCyl.GetBottomCenter()[2];
        result.code = QueryCode::NotIntersecting;
      }
      else if (v[2] < static_cast<Real>(0))
      {
        result.cp[2] = a_zCyl.GetBottomCenter()[2];
        result.code = QueryCode::NotIntersecting;
      }
      else
      {
        result.cp[2] = a_point[2];
        result.code = QueryCode::Intersecting;
      }
    }
    //Outside infinite cylinder
    else
    {
      result.code = QueryCode::NotIntersecting;
      Real ratio = a_zCyl.GetRadius() / sqrt(distSq);
      result.cp[0] = a_zCyl.GetBottomCenter()[0] + v[0] * ratio;
      result.cp[1] = a_zCyl.GetBottomCenter()[1] + v[1] * ratio;

      if (v[2] > a_zCyl.GetHeight())
        result.cp[2] = a_zCyl.GetHeight() + a_zCyl.GetBottomCenter()[2];
      else if (v[2] < static_cast<Real>(0))
        result.cp[2] = a_zCyl.GetBottomCenter()[2];
      else
        result.cp[2] = a_point[2];
    }

    return result;
  }
}

#endif