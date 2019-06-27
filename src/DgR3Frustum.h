//! @file DgR3Frustum.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Frustum

#ifndef DGR3FRUSTUM_H
#define DGR3FRUSTUM_H

#include "DgR3Vector.h"
#include "DgR3Plane.h"
#include "DgR3Sphere.h"
#include "DgR3VQS.h"
#include "dgmath.h"

//TODO Queries
namespace Dg
{
  namespace R3
  {
    //! Data used for intersection testing.
    template<typename Real>
    struct FrustumData
    {
      Plane<Real>           planes[6];    //[t, b, f, n, l, r]
      Vector<Real>          origin;
      Vector<Real>          basis[3];     //[f, l, u]
    };


    //! @ingroup DgMath_types
    //!
    //! @class Frustum
    //!
    //! @author: Frank Hart
    //! @date 20/06/2016
    template<typename Real>
    class Frustum
    {
    private:

    public:
      //! Default constructor.
      Frustum()
        : m_basis{ Vector<Real>::xAxis(), Vector<Real>::yAxis(), Vector<Real>::zAxis() }
        , m_origin(Vector<Real>::Origin())
        , m_near(static_cast<Real>(0.1))
        , m_far(static_cast<Real>(100.0))
        , m_nearHalfLengths{ static_cast<Real>(1.0), static_cast<Real>(1.0) }
      {}

      Frustum(Vector<Real> const & forward
        , Vector<Real> const & left
        , Vector<Real> const & up
        , Vector<Real> const & origin
        , Real nearL, Real farL, Real verL, Real horL)
        : m_basis{ forward, left, up }
        , m_nearHalfLengths{ verL, farL }
        , m_near(nearL)
        , m_far(farL)
      {}

      //! Construct Frustum from origin and radius
      ~Frustum() {}

      //! Copy constructor
      Frustum(Frustum const & a_other)
        : m_basis{ a_other.m_basis[0], a_other.m_basis[1], a_other.m_basis[2] }
        , m_origin(a_other.m_origin)
        , m_near(a_other.m_near)
        , m_far(a_other.m_far)
        , m_nearHalfLengths{ a_other.m_nearHalfLengths[0], a_other.m_nearHalfLengths[1] }
      {}

      //! Assignment
      Frustum& operator= (Frustum const & a_other)
      {
        m_basis[0] = a_other.m_basis[0];
        m_basis[1] = a_other.m_basis[1];
        m_basis[2] = a_other.m_basis[2];

        m_near = a_other.m_near;
        m_far = a_other.m_far;
        m_origin = a_other.m_origin;

        m_nearHalfLengths[0] = a_other.m_nearHalfLengths[0];
        m_nearHalfLengths[1] = a_other.m_nearHalfLengths[1];
      }

      Sphere<Real> GetSphere() const
      {
        Real mul = (m_far + m_near) / m_near;
        Real d = m_far - m_near;
        Real avn = m_nearHalfLengths[0];
        Real ahn = m_nearHalfLengths[1];
        Real avf = avn * mul;
        Real ahf = ahn * mul;
        Real b = (avf*avf + ahf*ahf + d*d - avn*avn - ahn*ahn) / (static_cast<Real>(2.0) * d);

        return Sphere<Real>(m_origin + (m_near + b) * m_basis[0], sqrt(b*b + avn*avn + ahn*ahn));
      }

      //! Get the Frustum center.
      void GetData(FrustumData<Real> & a_data) const
      {
        a_data.basis[0] = m_basis[0];
        a_data.basis[1] = m_basis[1];
        a_data.basis[2] = m_basis[2];

        a_data.origin = m_origin;

        //[f, n, t, b, l, r]
        Real eV = m_nearHalfLengths[0] / m_near;
        Real eH = m_nearHalfLengths[1] / m_near;
        a_data.planes[0].Set(-m_basis[0], m_origin + m_far * m_basis[0]);
        a_data.planes[1].Set(m_basis[0], m_origin + m_near * m_basis[0]);
        a_data.planes[2].Set(-m_basis[2] + (eV)* m_basis[0], m_origin);
        a_data.planes[3].Set(m_basis[2] + (eV)* m_basis[0], m_origin);
        a_data.planes[4].Set(-m_basis[1] + (eH)* m_basis[0], m_origin);
        a_data.planes[5].Set(m_basis[1] + (eH)* m_basis[0], m_origin);
      }

      //! Transform the Frustum
      Frustum GetTransformed(VQS<Real> const & a_vqs) const
      {
        return Frustum
        (
          a_vqs.Rotate(m_basis[0]),
          a_vqs.Rotate(m_basis[1]),
          a_vqs.Rotate(m_basis[2]),
          a_vqs.TransformPoint(m_origin),
          a_vqs.Scale(m_near),
          a_vqs.Scale(m_far),
          a_vqs.Scale(m_nearHalfLengths[0]),
          a_vqs.Scale(m_nearHalfLengths[1]),
          )
      }

      //! Transform the Frustum, assign to self
      Frustum & TransformSelf(VQS<Real> const & a_vqs)
      {
        a_vqs.RotateSelf(m_basis[0]);
        a_vqs.RotateSelf(m_basis[1]);
        a_vqs.RotateSelf(m_basis[2]);
        a_vqs.TransformPointSelf(m_origin);
        a_vqs.ScaleSelf(m_near);
        a_vqs.ScaleSelf(m_far);
        a_vqs.ScaleSelf(m_nearHalfLengths[0]);
        a_vqs.ScaleSelf(m_nearHalfLengths[1]);
      }

    private:
      Vector<Real>       m_basis[3];           //[f, l, u]
      Vector<Real>       m_origin;
      Real                m_near;
      Real                m_far;
      Real                m_nearHalfLengths[2]; //[vertical, horizontal]
    };
  }
}

#endif