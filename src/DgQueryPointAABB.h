//@group Math/impl

#ifndef DGQUERYPOINTAABB_H
#define DGQUERYPOINTAABB_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgAABB.h"

namespace Dg
{
  template <typename Real, int R>
  class Query<QueryType::TestForIntersection, Real, R, Vector<Real, R>, AABB<Real, R>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Vector<Real, R> const &, AABB<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2PointAABB = Query<QueryType::TestForIntersection, Real, 2, Vector2<Real>, AABB2<Real>>;

  template<typename Real>
  using TI3PointAABB = Query<QueryType::TestForIntersection, Real, 3, Vector3<Real>, AABB3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::TestForIntersection, Real, R, Vector<Real, R>, AABB<Real, R>>::Result
    Query<QueryType::TestForIntersection, Real, R, Vector<Real, R>, AABB<Real, R>>::operator()
    (Vector<Real, R> const & a_point, AABB<Real, R> const & a_aabb)
  {
    Result result;
    result.isIntersecting = true;

    for (int i = 0; i < R; i++)
    {
      if ((a_point[i] < a_aabb.Min()[i]) || (a_point[i] > a_aabb.Max()[i]))
      {
        result.isIntersecting = false;
        break;
      }
    }

    return result;
  }
}

#endif