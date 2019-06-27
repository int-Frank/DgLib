#ifndef DGR3QUERYRECTANGLEPOINT_H
#define DGR3QUERYRECTANGLEPOINT_H

#include "impl/DgCPQuery.h"
#include "DgR3Rectangle.h"
#include "DgR3Vector.h"
#include "../query/DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    template <typename Real>
      class CPQuery<Real, 3, 
                    R3::Rectangle<Real>,
                    R3::Vector<Real>>
    {
    public:

      struct Result
      {
        Real sqDistance;
        Real rectangleParameter[2];
        R3::Vector<Real> cp;
      };

      //! Perform the query
      Result operator()(R3::Rectangle<Real> const &, R3::Vector<Real> const &);
    };

    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, 3, R3::Rectangle<Real>, R3::Vector<Real>>::Result
      CPQuery<Real, 3, R3::Rectangle<Real>, R3::Vector<Real>>::operator()
      (R3::Rectangle<Real> const & a_rect, R3::Vector<Real> const & a_point)
    {
      Result result;

      R3::Vector<Real> diff = a_rect.GetCenter() - a_point;
      Real b0 = diff.Dot(a_rect.GetAxis(0));
      Real b1 = diff.Dot(a_rect.GetAxis(1));
      Real s0 = -b0;
      Real s1 = -b1;
      result.sqDistance = Dot(diff, diff);

      if (s0 < -a_rect.GetExtent(0))
      {
        s0 = -a_rect.GetExtent(0);
      }
      else if (s0 > a_rect.GetExtent(0))
      {
        s0 = a_rect.GetExtent(0);
      }
      result.sqDistance += s0*(s0 + (static_cast<Real>(2))*b0);

      if (s1 < -a_rect.GetExtent(1))
      {
        s1 = -a_rect.GetExtent(1);
      }
      else if (s1 > a_rect.GetExtent(1))
      {
        s1 = a_rect.GetExtent(1);
      }
      result.sqDistance += s1*(s1 + (static_cast<Real>(2))*b1);

      // Account for numerical round-off error.
      if (result.sqDistance < static_cast<Real>(0))
      {
        result.sqDistance = static_cast<Real>(0);
      }

      result.rectangleParameter[0] = s0;
      result.rectangleParameter[1] = s1;
      result.cp = a_rect.GetCenter();
      for (int i = 0; i < 2; ++i)
      {
        result.cp += result.rectangleParameter[i] * a_rect.GetAxis(i);
      }

      return result;
    } //End: TIQuery::operator()
  }

  namespace R3
  {
    template<typename Real>
    using CPRectangleCylinder = impl::CPQuery<Real, 3, Rectangle<Real>, Vector<Real>>;
  }
}

#endif