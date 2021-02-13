//@group Math/Geometry/Query

#ifndef DGQUERYSEGMENTHYPERSPHERE_H
#define DGQUERYSEGMENTHYPERSPHERE_H

#include "DgQuery.h"
#include "DgSegment.h"
#include "DgHypersphere.h"

namespace Dg
{
  template<typename Real, int R>
  class Query<QueryType::TestForIntersection, Real, R, Segment<Real, R>, Hypersphere<Real, R>>
  {
  public:

    struct Result
    {
      bool isIntersecting;
    };

    Result operator()(Segment<Real, R> const &, Hypersphere<Real, R> const &);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real> using TI2SegmentDisk   = Query<QueryType::TestForIntersection, Real, 3, Segment2<Real>, Disk2<Real>>;
  template<typename Real> using TI3SegmentSphere = Query<QueryType::TestForIntersection, Real, 3, Segment3<Real>, Sphere<Real>>;

  //---------------------------------------------------------------------------------------
  // Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real, int R>
  typename Query<QueryType::TestForIntersection, Real, R, Segment<Real, R>, Hypersphere<Real, R>>::Result
    Query<QueryType::TestForIntersection, Real, R, Segment<Real, R>, Hypersphere<Real, R>>::operator()
    (Segment<Real, R> const & a_seg, Hypersphere<Real, R> const & a_sphere)
  {
    Result result;
    Real a = MagSq(a_seg.Direction());

    //Zero-length segment case.
    if (IsZero(a))
    {
      result.isIntersecting = !a_sphere.IsOutside(a_seg.Origin());
      return result;
    }

    Vector<Real, R> w0(a_seg.Origin() - a_sphere.Center());
    Real inv_a = static_cast<Real>(1) / a;
    Real b = inv_a * Dot(w0, a_seg.Direction());
    Real c = inv_a * (MagSq(w0) - a_sphere.Radius() *a_sphere.Radius());
    Real discriminant = b * b + c;

    if (discriminant < static_cast<Real>(0))
    {
      result.isIntersecting = false;
      return result;
    }

    //TODO Not complete
    Real irr = sqrt(b * b - static_cast<Real>(4) * a * c);
    Real u0 = (-b - irr) / (static_cast<Real>(2) * a);
    Real u1 = (-b + irr) / (static_cast<Real>(2) * a);
    result.isIntersecting = (u0 > static_cast<Real>(0) && u0 < static_cast<Real>(1))
                         || (u1 > static_cast<Real>(0) && u1 < static_cast<Real>(1))
                         || (u0 * u1 < static_cast<Real>(0));
    return result;
  }
}

#endif