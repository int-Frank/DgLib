//@group Math/Geometry/Query

#ifndef DGQUERYRECTANGLESPHERE_H
#define DGQUERYRECTANGLESPHERE_H

#include "DgQuery.h"
#include "DgRectangle.h"
#include "DgHyperSphere.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 3, Rectangle3<Real>, Sphere<Real>>
  {
  public:

    struct Result
    {
      Real sqDistance;
      QueryCode code;
    };

    Result operator()(Rectangle3<Real> const &, Sphere<Real> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI3RectangleSphere = Query<QueryType::TestForIntersection, Real, 3, Rectangle3<Real>, Sphere<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 3, Rectangle3<Real>, Sphere<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 3, Rectangle3<Real>, Sphere<Real>>::operator()
    (Rectangle3<Real> const & a_rect, Sphere<Real> const & a_sphere)
  {
    Result result;

    Vector3<Real> diff = a_rect.GetCenter() - a_sphere.Center();
    Real b0 = Dot(diff, a_rect.GetAxis(0));
    Real b1 = Dot(diff, a_rect.GetAxis(1));
    Real s0 = -b0;
    Real s1 = -b1;
    result.sqDistance = Dot(diff, diff);

    if (s0 < -a_rect.GetExtent(0))
      s0 = -a_rect.GetExtent(0);
    else if (s0 > a_rect.GetExtent(0))
      s0 = a_rect.GetExtent(0);

    result.sqDistance += s0*(s0 + (static_cast<Real>(2))*b0);

    if (s1 < -a_rect.GetExtent(1))
      s1 = -a_rect.GetExtent(1);
    else if (s1 > a_rect.GetExtent(1))
      s1 = a_rect.GetExtent(1);

    result.sqDistance += s1*(s1 + (static_cast<Real>(2))*b1);

    // Account for numerical round-off error.
    if (result.sqDistance < static_cast<Real>(0))
      result.sqDistance = static_cast<Real>(0);

    if (result.sqDistance <= a_sphere.Radius() * a_sphere.Radius())
      result.code = QueryCode::Intersecting;
    else
      result.code = QueryCode::NotIntersecting;

    return result;
  }
}

#endif