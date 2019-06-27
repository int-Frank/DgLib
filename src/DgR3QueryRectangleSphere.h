#ifndef DGR3QUERYRECTANGLESPHERE_H
#define DGR3QUERYRECTANGLESPHERE_H

#include "impl/DgTIQuery.h"
#include "DgR3Rectangle.h"
#include "DgR3Sphere.h"
#include "../query/DgQueryCommon.h"

namespace Dg
{
  namespace impl
  {
    template <typename Real>
    class TIQuery<Real, 3, 
                  R3::Rectangle<Real>,
                  R3::Sphere<Real>>
    {
    public:

      struct Result
      {
        Real sqDistance;
        QueryCode code;
      };

      //! Perform the query
      Result operator()(R3::Rectangle<Real> const &, R3::Sphere<Real> const &);
    };

    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Rectangle<Real>, R3::Sphere<Real>>::Result
      TIQuery<Real, 3, R3::Rectangle<Real>, R3::Sphere<Real>>::operator()
      (R3::Rectangle<Real> const & a_rect, R3::Sphere<Real> const & a_sphere)
    {
      Result result;

      R3::Vector<Real> diff = a_rect.GetCenter() - a_sphere.Center();
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

      if (result.sqDistance <= a_sphere.Radius() * a_sphere.Radius())
      {
        result.code = QC_Intersecting;
      }
      else
      {
        result.code = QC_NotIntersecting;
      }

      return result;
    } //End: TIQuery::operator()
  }

  namespace R3
  {
    template<typename Real>
    using TIRectangleSphere = impl::TIQuery<Real, 3, Rectangle<Real>, Sphere<Real>>;
  }
}

#endif