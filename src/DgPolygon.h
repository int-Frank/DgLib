//@group Math/Geometry

//! @file DgR2Polygon.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Polygon

#ifndef DGR2POLYGON_H
#define DGR2POLYGON_H

#include "DgCircularDoublyLinkedList.h"
#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Polygon
  //!
  //!
  //! @author: Frank Hart
  //! @date 20/06/2016

  //TODO maybe instead of deriving from CircularDoublyLinkedList, have the container a template parameter.
  template<typename Real>
  class Polygon : public CircularDoublyLinkedList<Vector2<Real>>
  {
  public:

    Real Area() const
    {
      return abs(SignedArea());
    }

    Dg::Orientation Orientation() const
    {
      if (size() < 3)
        return Dg::Orientation::Colinear;

      Real signedArea = SignedArea();
      if (IsZero(signedArea))
        return Dg::Orientation::Colinear;
      if (signedArea > static_cast<Real>(0))
        return Dg::Orientation::CW;
      return Dg::Orientation::CCW;
    }

  private:

    Real SignedArea() const
    {
      Real area = static_cast<Real>(0);
      const_iterator it = chead();
      for (size_t i = 0; i < size(); i++, it++)
      {
        auto next(it);
        next++;

        area += (next->x() - it->x()) * (next->y() + it->y());
      }

      return area / static_cast<Real>(2);
    }
  };
}

#endif