//@group Math/Geometry

#ifndef DGHYPERSPHERE_H
#define DGHYPERSPHERE_H

#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real, int R>
  class Hypersphere
  {
  public:
    //! Default constructor.
    Hypersphere() 
      : m_origin(Zeros<Real, R>())
      , m_radius(static_cast<Real>(1)) 
    {}

    //! Construct sphere from origin and radius
    Hypersphere(Vector<Real, R> const &, Real);
    ~Hypersphere() {}

    //! Copy constructor
    Hypersphere(Hypersphere const & a_other)
      : m_origin(a_other.m_origin)
      , m_radius(a_other.m_radius) 
    {}

    //! Assignment
    Hypersphere& operator= (Hypersphere const &);

    //! Get the sphere center.
    Vector<Real, R> const & Center() const { return m_origin; }

    //! Get the sphere radius.
    Real Radius() const { return m_radius; }

    //! Get the plane center and .
    void Get(Vector<Real, R>&, Real&) const;

    //! Quick point test
    bool IsOutside(Vector<Real, R> const &) const;

    //! Are two planes equal?
    bool operator== (Hypersphere const &) const;

    //! Are two planes not equal?
    bool operator!= (Hypersphere const &) const;

    //! Set the sphere center
    void SetCenter(Vector<Real, R> const &);

    //! Set the sphere radius
    void SetRadius(Real);

    //! Set the center and radius.
    void Set(Vector<Real, R> const & n, Real);

  private:
    Vector<Real, R>   m_origin;
    Real              m_radius;
  };

  template<typename Real>
  using Disk2 = Hypersphere<Real, 2>;

  template<typename Real>
  using Sphere = Hypersphere<Real, 3>;

  //--------------------------------------------------------------------------------
  //	@	Hypersphere::Hypersphere()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Hypersphere<Real, R>::Hypersphere(Vector<Real, R> const & a_center, Real a_radius)
  {
    Set(a_center, a_radius);
  }	//End: Hypersphere::Hypersphere()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  Hypersphere<Real, R> & Hypersphere<Real, R>::operator=(Hypersphere<Real, R> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_radius = a_other.m_radius;
  }	//End: Hypersphere::operator=()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::Set()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Hypersphere<Real, R>::Set(Vector<Real, R> const & a_center, Real a_radius)
  {
    m_origin = a_center;
    m_radius = a_radius;
  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Hypersphere<Real, R>::operator==(Hypersphere<Real, R> const & a_other) const
  {
    return (m_origin == a_other.m_origin && Dg::AreEqual(m_radius, a_other.m_radius));
  }	//End: Hypersphere::operator==()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  bool Hypersphere<Real, R>::operator!=(Hypersphere<Real, R> const & a_other) const
  {
    return (m_origin != a_other.m_origin || !Dg::AreEqual(m_radius, a_other.m_radius));
  }	//End: Hypersphere::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::SetCenter()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Hypersphere<Real, R>::SetCenter(Vector<Real, R> const & a_center)
  {
    m_origin = a_center;
  }	//End: Hypersphere::SetCenter()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::SetRadius()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Hypersphere<Real, R>::SetRadius(Real a_radius)
  {
    m_radius = (a_radius < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_radius;
  }	//End: Hypersphere::SetRadius()


  //--------------------------------------------------------------------------------
  //	@	Hypersphere::Get()
  //--------------------------------------------------------------------------------
  template<typename Real, int R>
  void Hypersphere<Real, R>::Get(Vector<Real, R> & a_center, Real & a_radius) const
  {
    a_center = m_origin;
    a_radius = m_radius;
  }	//End: Hypersphere::Get()


  template<typename Real, int R>
  bool Hypersphere<Real, R>::IsOutside(Vector<Real, R> const & a_pt) const
  {
    return MagSq(m_origin - a_pt) > (m_radius * m_radius);
  }
}

#endif