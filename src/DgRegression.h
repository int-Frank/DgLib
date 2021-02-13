//@group Math/R2

#ifndef DGR2REGRESSION_H
#define DGR2REGRESSION_H

#include "DgVector.h"
#include "DgLine.h"

namespace Dg
{
  template<typename Real>
  Line2<Real> LineOfBestFit2(Vector2<Real> const * a_points,
                            size_t a_nPoints)
  {
    Real sumX(static_cast<Real>(0));
    Real sumY(static_cast<Real>(0));
    Real sumXY(static_cast<Real>(0));
    Real sumXX(static_cast<Real>(0));

    for (size_t i = 0; i < a_nPoints; i++)
    {
      sumX += a_points[i][0];
      sumY += a_points[i][1];
      sumXY += (a_points[i][0] * a_points[i][1]);
      sumXX += (a_points[i][0] * a_points[i][0]);
    }

    Real x = static_cast<Real>(a_nPoints) * sumXX - sumX * sumX;
    Real y = static_cast<Real>(a_nPoints) * sumXY - sumX * sumY;
    Vector2<Real> direction(x, y);
    direction = Normalize(direction);
    Vector2<Real> centroid(sumX / static_cast<Real>(a_nPoints),
                           sumY / static_cast<Real>(a_nPoints));
    return Line2<Real>(centroid, direction);
  }
}

#endif