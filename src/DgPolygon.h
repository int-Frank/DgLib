//@group Math/Geometry

//! @file DgR2Polygon.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Polygon

#ifndef DGR2POLYGON_H
#define DGR2POLYGON_H

#include "DgDoublyLinkedList.h"
#include "DgVector.h"
#include "DgSegment.h"
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

  // Polygon, no holes
  template<typename Real>
  class Polygon2
  {
  public:

    using ContainerType = DoublyLinkedList<Vector2<Real>>;
    typedef typename ContainerType::iterator PointIterator;
    typedef typename ContainerType::const_iterator ConstPointIterator;

    class ConstEdgeIterator
    {
      friend class Polygon2;
      friend class EdgeIterator;
    private:

      ConstEdgeIterator(Polygon2::ConstPointIterator const &p0,
                        Polygon2::ConstPointIterator const &p1,
                        Polygon2::ConstPointIterator const &begin,
                        Polygon2::ConstPointIterator const &end);

    public:

      ConstEdgeIterator();
      ~ConstEdgeIterator();

      ConstEdgeIterator(ConstEdgeIterator const &a_it);
      ConstEdgeIterator &operator= (ConstEdgeIterator const &);

      bool operator==(ConstEdgeIterator const &a_it) const;
      bool operator!=(ConstEdgeIterator const &a_it) const;

      ConstEdgeIterator operator+(size_t) const;
      ConstEdgeIterator operator-(size_t) const;

      ConstEdgeIterator &operator+=(size_t);
      ConstEdgeIterator &operator-=(size_t);

      ConstEdgeIterator &operator++();
      ConstEdgeIterator operator++(int);
      ConstEdgeIterator &operator--();
      ConstEdgeIterator operator--(int);

      Segment2<Real> ToSegment() const;
      Segment2<Real> operator*() const;

    private:
      Polygon2::ConstPointIterator m_p0;
      Polygon2::ConstPointIterator m_p1;
      Polygon2::ConstPointIterator const m_begin;
      Polygon2::ConstPointIterator const m_end;
    };

    class EdgeIterator
    {
      friend class Polygon2;
    private:

      EdgeIterator(Polygon2::PointIterator const &p0,
                   Polygon2::PointIterator const &p1, 
                   Polygon2::PointIterator const &begin, 
                   Polygon2::PointIterator const &end);

    public:

      EdgeIterator();
      ~EdgeIterator();

      EdgeIterator(EdgeIterator const &a_it);
      EdgeIterator &operator= (EdgeIterator const &);

      bool operator==(EdgeIterator const &a_it) const;
      bool operator!=(EdgeIterator const &a_it) const;

      EdgeIterator operator+(size_t) const;
      EdgeIterator operator-(size_t) const;

      EdgeIterator &operator+=(size_t);
      EdgeIterator &operator-=(size_t);

      EdgeIterator &operator++();
      EdgeIterator operator++(int);
      EdgeIterator &operator--();
      EdgeIterator operator--(int);

      Segment2<Real> ToSegment() const;
      Segment2<Real> operator*() const;

      operator ConstEdgeIterator() const;

    private:
      Polygon2::PointIterator m_p0;
      Polygon2::PointIterator m_p1;
      Polygon2::PointIterator const m_begin;
      Polygon2::PointIterator const m_end;
    };

  public:

    Polygon2();
    ~Polygon2();

    Polygon2(Polygon2 const &);
    Polygon2 &operator=(Polygon2 const &);

    Polygon2(Polygon2 &&)  noexcept;
    Polygon2 &operator=(Polygon2 &&)  noexcept;

    PointIterator PointsBegin();
    PointIterator PointsEnd();
    ConstPointIterator cPointsBegin() const;
    ConstPointIterator cPointsEnd() const;

    EdgeIterator EdgesBegin();
    EdgeIterator EdgesEnd();
    ConstEdgeIterator cEdgesBegin() const;
    ConstEdgeIterator cEdgesEnd() const;

    size_t Size() const;

    void PushBack(Vector2<Real> const &);
    void PushFront(Vector2<Real> const &);

    PointIterator Insert(PointIterator const &position, Vector2<Real> const &point);

    void PopBack();
    void PopFront();

    PointIterator Erase(PointIterator const &position);

    void Clear();

    Polygon2 ReverseWinding() const;
    void ReverseThisWinding();
    void SetWinding(Dg::Orientation);

    Real Area() const;
    Dg::Orientation Winding() const;
    Real SignedArea() const;

  private:

    DoublyLinkedList<Vector2<Real>> m_points;
  };

  //--------------------------------------------------------------------------------
  //		ConstEdgeIterator
  //--------------------------------------------------------------------------------

  template<typename Real>
  Polygon2<Real>::ConstEdgeIterator::ConstEdgeIterator(Polygon2::ConstPointIterator const &p0,
                                                       Polygon2::ConstPointIterator const &p1,
                                                       Polygon2::ConstPointIterator const &begin,
                                                       Polygon2::ConstPointIterator const &end)
    : m_p0(p0)
    , m_p1(p1)
    , m_begin(begin)
    , m_end(end)
  {

  }

  template<typename Real>
  Polygon2<Real>::ConstEdgeIterator::ConstEdgeIterator()
  {

  }

  template<typename Real>
  Polygon2<Real>::ConstEdgeIterator::~ConstEdgeIterator()
  {

  }

  template<typename Real>
  Polygon2<Real>::ConstEdgeIterator::ConstEdgeIterator(ConstEdgeIterator const &it)
    : m_p0(it.m_p0)
    , m_p1(it.m_p1)
    , m_begin(it.m_begin)
    , m_end(it.m_end)
  {

  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator &
    Polygon2<Real>::ConstEdgeIterator::operator=(ConstEdgeIterator const &other)
  {
    if (this != &other)
    {
      m_p0 = other.m_p0;
      m_p1 = other.m_p1;
      m_begin = other.m_begin;
      m_end = other.m_end;
    }
  }

  template<typename Real>
  bool Polygon2<Real>::ConstEdgeIterator::operator==(ConstEdgeIterator const &it) const
  {
    return m_p0 == it.m_p0 && m_p1 == it.m_p1;
  }

  template<typename Real>
  bool Polygon2<Real>::ConstEdgeIterator::operator!=(ConstEdgeIterator const &it) const
  {
    return !(m_p0 == it.m_p0 && m_p1 == it.m_p1);
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator
    Polygon2<Real>::ConstEdgeIterator::operator+(size_t a_val) const
  {
    auto p0 = m_p0;
    auto p1 = m_p1;
    for (size_t i = 0; i < a_val; i++)
    {
      p0++;
      p1++;

      if (p1 == m_end)
        p1 = m_begin;
    }

    return ConstEdgeIterator(p0, p1, m_begin, m_end);
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator
    Polygon2<Real>::ConstEdgeIterator::operator-(size_t a_val) const
  {
    auto p0 = m_p0;
    auto p1 = m_p1;
    for (size_t i = 0; i < a_val; i++)
    {
      if (p1 == m_begin)
        p1 = end;

      p0--;
      p1--;
    }

    return ConstEdgeIterator(p0, p1, m_begin, m_end);
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator &
    Polygon2<Real>::ConstEdgeIterator::operator+=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
    {
      m_p0++;
      m_p1++;

      if (m_p1 == m_end)
        m_p1 = m_begin;
    }

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator &
    Polygon2<Real>::ConstEdgeIterator::operator-=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
    {
      if (m_p1 == m_begin)
        m_p1 = end;

      m_p0--;
      m_p1--;
    }

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator &
    Polygon2<Real>::ConstEdgeIterator::operator++()
  {
    m_p0++;
    m_p1++;

    if (m_p1 == m_end)
      m_p1 = m_begin;

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator
    Polygon2<Real>::ConstEdgeIterator::operator++(int)
  {
    ConstEdgeIterator result(*this);
    ++(*this);
    return result;
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator &
    Polygon2<Real>::ConstEdgeIterator::operator--()
  {
    if (m_p1 == m_begin)
      m_p1 = m_end;

    m_p0--;
    m_p1--;

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator
    Polygon2<Real>::ConstEdgeIterator::operator--(int)
  {
    ConstEdgeIterator result(*this);
    --(*this);
    return result;
  }

  template<typename Real>
  Segment2<Real> Polygon2<Real>::ConstEdgeIterator::ToSegment() const
  {
    Segment2<Real> seg;
    seg.Set(*m_p0, *m_p1); // TODO handle error
    return seg;
  }

  template<typename Real>
  Segment2<Real> Polygon2<Real>::ConstEdgeIterator::operator*() const
  {
    Segment2<Real> seg;
    seg.Set(*m_p0, *m_p1); // TODO handle error
    return seg;
  }

  //--------------------------------------------------------------------------------
  //		EdgeIterator
  //--------------------------------------------------------------------------------

  template<typename Real>
  Polygon2<Real>::EdgeIterator::EdgeIterator(Polygon2::PointIterator const &p0,
                                             Polygon2::PointIterator const &p1,
                                             Polygon2::PointIterator const &begin,
                                             Polygon2::PointIterator const &end)
    : m_p0(p0)
    , m_p1(p1)
    , m_begin(begin)
    , m_end(end)
  {

  }

  template<typename Real>
  Polygon2<Real>::EdgeIterator::EdgeIterator()
  {

  }

  template<typename Real>
  Polygon2<Real>::EdgeIterator::~EdgeIterator()
  {

  }

  template<typename Real>
  Polygon2<Real>::EdgeIterator::EdgeIterator(EdgeIterator const &it)
    : m_p0(it.m_p0)
    , m_p1(it.m_p1)
    , m_begin(it.m_begin)
    , m_end(it.m_end)
  {

  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator &
    Polygon2<Real>::EdgeIterator::operator=(EdgeIterator const &other)
  {
    if (this != &other)
    {
      m_p0 = other.m_p0;
      m_p1 = other.m_p1;
      m_begin = other.m_begin;
      m_end = other.m_end;
    }
  }

  template<typename Real>
  bool Polygon2<Real>::EdgeIterator::operator==(EdgeIterator const &it) const
  {
    return m_p0 == it.m_p0 && m_p1 == it.m_p1;
  }

  template<typename Real>
  bool Polygon2<Real>::EdgeIterator::operator!=(EdgeIterator const &it) const
  {
    return !(m_p0 == it.m_p0 && m_p1 == it.m_p1);
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator
    Polygon2<Real>::EdgeIterator::operator+(size_t a_val) const
  {
    auto p0 = m_p0;
    auto p1 = m_p1;
    for (size_t i = 0; i < a_val; i++)
    {
      p0++;
      p1++;

      if (p1 == m_end)
        p1 = m_begin;
    }

    return EdgeIterator(p0, p1, m_begin, m_end);
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator
    Polygon2<Real>::EdgeIterator::operator-(size_t a_val) const
  {
    auto p0 = m_p0;
    auto p1 = m_p1;
    for (size_t i = 0; i < a_val; i++)
    {
      if (p1 == m_begin)
        p1 = m_end;

      p0--;
      p1--;
    }

    return EdgeIterator(p0, p1, m_begin, m_end);
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator &
    Polygon2<Real>::EdgeIterator::operator+=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
    {
      m_p0++;
      m_p1++;

      if (m_p1 == m_end)
        m_p1 = m_begin;
    }

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator &
    Polygon2<Real>::EdgeIterator::operator-=(size_t a_val)
  {
    for (size_t i = 0; i < a_val; i++)
    {
      if (m_p1 == m_begin)
        m_p1 = end;

      m_p0--;
      m_p1--;
    }

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator &
    Polygon2<Real>::EdgeIterator::operator++()
  {
    m_p0++;
    m_p1++;

    if (m_p1 == m_end)
      m_p1 = m_begin;

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator
    Polygon2<Real>::EdgeIterator::operator++(int)
  {
    EdgeIterator result(*this);
    ++(*this);
    return result;
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator &
    Polygon2<Real>::EdgeIterator::operator--()
  {
    if (m_p1 == m_begin)
      m_p1 = end;

    m_p0--;
    m_p1--;

    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator
    Polygon2<Real>::EdgeIterator::operator--(int)
  {
    EdgeIterator result(*this);
    --(*this);
    return result;
  }

  template<typename Real>
  Segment2<Real> Polygon2<Real>::EdgeIterator::ToSegment() const
  {
    Segment2<Real> seg;
    seg.Set(*m_p0, *m_p1); // TODO handle error
    return seg;
  }

  template<typename Real>
  Segment2<Real> Polygon2<Real>::EdgeIterator::operator*() const
  {
    Segment2<Real> seg;
    seg.Set(*m_p0, *m_p1); // TODO handle error
    return seg;
  }

  template<typename Real>
  Polygon2<Real>::EdgeIterator::operator typename Polygon2<Real>::ConstEdgeIterator() const
  {
    return ConstEdgeIterator(m_p0, m_p1, m_begin, m_end);
  }

  //--------------------------------------------------------------------------------
  //		Polygon2
  //--------------------------------------------------------------------------------
  template<typename Real>
  Polygon2<Real>::Polygon2()
    : m_points()
  {

  }

  template<typename Real>
  Polygon2<Real>::~Polygon2()
  {

  }

  template<typename Real>
  Polygon2<Real>::Polygon2(Polygon2 const &other)
    : m_points(other.m_points)
  {

  }

  template<typename Real>
  Polygon2<Real> & Polygon2<Real>::operator=(Polygon2 const & other)
  {
    if (this != &other)
    {
      m_points = other.m_points;
    }
    return *this;
  }

  template<typename Real>
  Polygon2<Real>::Polygon2(Polygon2 &&other) noexcept
    : m_points(std::move(other.m_points))
  {

  }

  template<typename Real>
  Polygon2<Real> &Polygon2<Real>::operator=(Polygon2 &&other) noexcept
  {
    if (this != &other)
    {
      m_points = std::move(other.m_points);
    }
    return *this;
  }

  template<typename Real>
  typename Polygon2<Real>::PointIterator Polygon2<Real>::PointsBegin()
  {
    return m_points.begin();
  }

  template<typename Real>
  typename Polygon2<Real>::PointIterator Polygon2<Real>::PointsEnd()
  {
    return m_points.end();
  }

  template<typename Real>
  typename Polygon2<Real>::ConstPointIterator Polygon2<Real>::cPointsBegin() const
  {
    return m_points.cbegin();
  }

  template<typename Real>
  typename Polygon2<Real>::ConstPointIterator Polygon2<Real>::cPointsEnd() const
  {
    return m_points.cend();
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator Polygon2<Real>::EdgesBegin()
  {
    return EdgeIterator(PointsBegin(), ++PointsBegin(), PointsBegin(), PointsEnd());
  }

  template<typename Real>
  typename Polygon2<Real>::EdgeIterator Polygon2<Real>::EdgesEnd()
  {
    return EdgeIterator(PointsEnd(), ++PointsBegin(), PointsBegin(), PointsEnd());
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator Polygon2<Real>::cEdgesBegin() const
  {
    return ConstEdgeIterator(cPointsBegin(), ++cPointsBegin(), cPointsBegin(), cPointsEnd());
  }

  template<typename Real>
  typename Polygon2<Real>::ConstEdgeIterator Polygon2<Real>::cEdgesEnd() const
  {
    return ConstEdgeIterator(cPointsEnd(), ++cPointsBegin(), cPointsBegin(), cPointsEnd());
  }

  template<typename Real>
  size_t Polygon2<Real>::Size() const
  {
    return m_points.size();
  }

  template<typename Real>
  void Polygon2<Real>::PushBack(Vector2<Real> const &point)
  {
    m_points.push_back(point);
  }

  template<typename Real>
  void Polygon2<Real>::PushFront(Vector2<Real> const &point)
  {
    m_points.push_front(point);
  }

  template<typename Real>
  typename Polygon2<Real>::PointIterator Polygon2<Real>::Insert(PointIterator const &position, Vector2<Real> const &point)
  {
    return m_points.insert(position, item);
  }

  template<typename Real>
  void Polygon2<Real>::PopBack()
  {
    m_points.pop_back();
  }

  template<typename Real>
  void Polygon2<Real>::PopFront()
  {
    m_points.pop_front();
  }

  template<typename Real>
  typename Polygon2<Real>::PointIterator Polygon2<Real>::Erase(PointIterator const &position)
  {
    return m_points.erase(position);
  }

  template<typename Real>
  void Polygon2<Real>::Clear()
  {
    m_points.clear();
  }

  template<typename Real>
  Polygon2<Real> Polygon2<Real>::ReverseWinding() const
  {
    Polygon2<Real> result = *this;
    result.ReverseThisWinding();
    return result;
  }

  template<typename Real>
  void Polygon2<Real>::ReverseThisWinding()
  {
    if (Size() < 2)
      return;

    auto itl = PointsBegin();
    auto itr = --PointsEnd();
    for (size_t i = 0; i < m_points.size() / 2; i++, itl++, itr--)
    {
      Vector2<Real> temp = *itl;
      *itl = *itr;
      *itr = temp;
    }
  }

  template<typename Real>
  Real Polygon2<Real>::Area() const
  {
    return abs(SignedArea());
  }

  template<typename Real>
  void Polygon2<Real>::SetWinding(Dg::Orientation a_winding)
  {
    Dg::Orientation winding = Winding();
    if (((a_winding != Dg::Orientation::CCW) && (a_winding != Dg::Orientation::CW)) ||
        ((winding   != Dg::Orientation::CCW) && (winding   != Dg::Orientation::CW)))
      return;

    if (a_winding != winding)
      ReverseThisWinding();
  }

  template<typename Real>
  Dg::Orientation Polygon2<Real>::Winding() const
  {
    if (Size() < 3)
      return Dg::Orientation::Colinear;

    Real signedArea = SignedArea();
    if (IsZero(signedArea))
      return Dg::Orientation::Colinear;
    if (signedArea > static_cast<Real>(0))
      return Dg::Orientation::CW;
    return Dg::Orientation::CCW;
  }

  template<typename Real>
  Real Polygon2<Real>::SignedArea() const
  {
    Real area = static_cast<Real>(0);
    for (auto edge_it = cEdgesBegin(); edge_it != cEdgesEnd(); edge_it++)
    {
      Segment2<Real> seg = *edge_it;
      area += (seg.GetP1().x() - seg.GetP0().x()) * (seg.GetP1().y() + seg.GetP0().y());
    }

    return area / static_cast<Real>(2);
  }
}

#endif