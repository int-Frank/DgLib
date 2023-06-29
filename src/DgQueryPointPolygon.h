//@group Math/Geometry/Query

#ifndef DGQUERYPOINTPOLYGON_H
#define DGQUERYPOINTPOLYGON_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPolygon.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Vector<Real, 2>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;
    };

    Result operator()(Vector<Real, 2> const &, Polygon2<Real> const &);
  };

  template <typename Real>
  class Query<QueryType::ClosestPoint, Real, 2, Vector<Real, 2>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;
      Vector<Real, 2> cp;
    };

    Result operator()(Vector<Real, 2> const &, Polygon2<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2PointPolygon = Query<QueryType::TestForIntersection, Real, 2, Vector2<Real>, Polygon2<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  // Adapted from "Optimal Reliable Point-in-Polygon Test and Differential Coding Boolean Operations on Polygons"
  // Authors: Jianqiang Hao, Jianzhi Sun, Yi Chen, Qiang Cai and Li Tan
  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Vector<Real, 2>, Polygon2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Vector<Real, 2>, Polygon2<Real>>::operator()
    (Vector<Real, 2> const &a_point, Polygon2<Real> const &a_polygon)
  {
    Result result;
    result.code = QueryCode::Success;

    size_t k = 0;
    Real f = Real(0);
    Real u1 = Real(0);
    Real v1 = Real(0);
    Real u2 = Real(0);
    Real v2 = Real(0);

    for (auto edge_it = a_polygon.cEdgesBegin(); edge_it != a_polygon.cEdgesEnd(); edge_it++)
    {
      Segment2<Real> edge = *edge_it;
      Real xi = edge.GetP0().x();
      Real yi = edge.GetP0().y();
      Real xj = edge.GetP1().x();
      Real yj = edge.GetP1().y();

      v1 = yi - a_point.y();
      v2 = yj - a_point.y();

      if (((v1 < Real(0)) && (v2 < Real(0))) || ((v1 > Real(0)) && (v2 > Real(0))))
        continue;

      u1 = xi - a_point.x();
      u2 = xj - a_point.x();

      if ((v2 > Real(0)) && (v1 <= Real(0)))
      {
        f = u1 * v2 - u2 * v1;
        if (f > 0)
        {
          k++;
        }
        else if (f == 0)
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
      }
      else if ((v1 > Real(0)) && (v2 <= Real(0)))
      {
        f = u1 * v2 - u2 * v1;
        if (f < 0)
        {
          k++;
        }
        else if (f == 0)
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
      }
      else if ((v2 == Real(0)) && (v1 < Real(0)))
      {
        f = u1 * v2 - u2 * v1;
        if (f == 0)
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
      }
      else if ((v1 == Real(0)) && (v2 < Real(0)))
      {
        f = u1 * v2 - u2 * v1;
        if (f == 0)
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
      }
      else if ((v1 == Real(0)) && (v2 == Real(0)))
      {
        if ((u2 <= Real(0)) && (u1 >= Real(0)))
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
        else if ((u1 <= Real(0)) && (u2 >= Real(0)))
        {
          result.code = QueryCode::OnBoundary;
          break;
        }
      }
    }

    if (result.code != QueryCode::OnBoundary)
      result.code = (k % 2 == 0) ? QueryCode::CompletelyOutside : QueryCode::CompletelyInside;

    return result;
  }
}

#endif