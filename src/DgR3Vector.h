//! @file DgR3Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGR3VECTOR_H
#define DGR3VECTOR_H

#include "impl/DgR3Vector_generic.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Vector = Dg::impl::Vector_generic<Real, 3>;


    //! Creates an orthogonal basis from two input vectors. 
    //!   - The vector 'A_in' is considered the principle axis.
    //!   - If A_in is a zero vector, A_out will be set to Vector<Real>::xAxis()
    //!   - If A_in and B_in are parallel, or B_in is a Zero vector, B_out will be chosen such that it is orthogonal to A_in
    //!   - Finally, C_out is created from A_out X B_out.
    template<typename Real>
    void GetBasis(Vector<Real> const & a_A_in,
                  Vector<Real> const & a_B_in,
                  Vector<Real> & a_A_out,
                  Vector<Real> & a_B_out,
                  Vector<Real> & a_C_out)
    {
      if (a_A_in.IsZero())
      {
        a_A_out = Vector<Real>::xAxis();
      }
      else
      {
        a_A_out = a_A_in;
        a_A_out.Normalize();
      }

      a_C_out = a_A_out.Cross(a_B_in);

      //Test to see if A_in and B_in are parallel
      if (IsZero(a_C_out.LengthSquared()))
      {
        a_B_out = a_A_out.Perpendicular();
        a_C_out = a_A_out.Cross(a_B_out);
      }
      else
      {
        a_C_out.Normalize();
        a_B_out = a_C_out.Cross(a_A_out);
      }
    } //End: GetBasis()


    //! Squared distance between two points.
    template<typename Real>
    Real SquaredDistance(Vector<Real> const & a_p0, Vector<Real> const & a_p1)
    {
      return (a_p0 - a_p1).LengthSquared();
    } // End: SquaredDistance()


    //! Distance between two points.
    template<typename Real>
    Real Distance(Vector<Real> const & a_p0, Vector<Real> const & a_p1)
    {
      return (a_p0 - a_p1).Length();
    } // End: SquaredDistance()
  }
}


#endif