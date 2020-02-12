//@group Math/impl

#ifndef DGQUERYPOINTAABB_H
#define DGQUERYPOINTAABB_H

#include "DgTIQuery.h"
#include "DgVector_generic.h"
#include "DgAABB_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Plane.
    template <typename Real, int R>
    class TIQuery<Real, R,
                  Vector_generic<Real, R>,
                  AABB_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        bool isIntersecting;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const&, AABB_generic<Real, R> const&);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename TIQuery<Real, R, Vector_generic<Real, R>, AABB_generic<Real, R>>::Result
      TIQuery<Real, R, Vector_generic<Real, R>, AABB_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const& a_point, AABB_generic<Real, R> const& a_aabb)
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
    } //End: CPQuery::operator()
  }
}

#endif