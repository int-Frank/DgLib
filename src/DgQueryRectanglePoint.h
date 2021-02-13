//@group Math/Geometry/Query

#ifndef DGQUERYRECTANGLEPOINT_H
#define DGQUERYRECTANGLEPOINT_H

#include "DgQuery.h"
#include "DgRectangle.h"
#include "DgVector.h"

namespace Dg
{
  template <typename Real>
    class Query<QueryType::ClosestPoint, Real, 3, Rectangle3<Real>, Vector3<Real>>
  {
  public:

    struct Result
    {
      Real sqDistance;
      Real rectangleParameter[2];
      Vector3<Real> cp;
    };

    Result operator()(Rectangle3<Real> const &, Vector3<Real> const &);
  };
    
  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using CP3RectanglePoint = Query<QueryType::ClosestPoint, Real, 3, Rectangle3<Real>, Vector3<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::ClosestPoint, Real, 3, Rectangle3<Real>, Vector3<Real>>::Result
    Query<QueryType::ClosestPoint, Real, 3, Rectangle3<Real>, Vector3<Real>>::operator()
    (Rectangle3<Real> const & a_rect, Vector3<Real> const & a_point)
  {
    Result result;

    Vector3<Real> diff = a_rect.GetCenter() - a_point;
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

    result.rectangleParameter[0] = s0;
    result.rectangleParameter[1] = s1;
    result.cp = a_rect.GetCenter();
    for (int i = 0; i < 2; ++i)
      result.cp += result.rectangleParameter[i] * a_rect.GetAxis(i);

    return result;
  }
}

#endif