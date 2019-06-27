#ifndef DGR2REGRESSION_H
#define DGR2REGRESSION_H

#include "DgR2Vector.h"
#include "DgR2Line.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    Line<Real> LineOfBestFit(Vector<Real> const * a_points,
                             size_t a_nPoints)
    {
      Real sumX(static_cast<Real>(0));
      Real sumY(static_cast<Real>(0));
      Real sumXY(static_cast<Real>(0));
      Real sumXX(static_cast<Real>(0));

      for (size_t i = 0; i < a_nPoints; i++)
      {
        sumX += a_points[i].x();
        sumY += a_points[i].y();
        sumXY += (a_points[i].x() * a_points[i].y());
        sumXX += (a_points[i].x() * a_points[i].x());
      }

      Real x = static_cast<Real>(a_nPoints) * sumXX - sumX * sumX;
      Real y = static_cast<Real>(a_nPoints) * sumXY - sumX * sumY;
      Vector<Real> direction(x, y, static_cast<Real>(0));
      direction.Normalize();
      Vector<Real> centroid(sumX / static_cast<Real>(a_nPoints),
                            sumY / static_cast<Real>(a_nPoints), 
                            static_cast<Real>(1) );
      return Line<Real>(centroid, direction);
    }
  }
}

#endif