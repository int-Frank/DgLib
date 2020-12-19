//@group Misc

#ifndef DGBINPACKER_H
#define DGBINPACKER_H

#include <stdint.h>
#include <algorithm>

#include "DgTypes.h"
#include "DgBit.h"
#include "DgDynamicArray.h"
#include "DgDoublyLinkedlist.h"
#include "DgSet_AVL.h"
#include "DgError.h"

namespace Dg
{
  template<typename Real, typename IDType>
  class BinPacker
  {
  public:

    struct Item
    {
      IDType id;
      Real xy[2];
    };
    
    enum class Cut
    {
      Vertical   = 0,
      Horizontal = 1
    };

    typedef bool (*DefaultItemCompareFn)(Item const &, Item const &);
    typedef Cut(*DefaultCutNodeFn)(Real nodeSize[2], Real rectangleSize[2]);

    static bool DefaultCompare(Item const &, Item const &);
    static Cut DefaultCutNode(Real nodeSize[2], Real rectSize[2]);

  public:

    BinPacker();
    ~BinPacker();

    BinPacker & operator=(BinPacker const &);
    BinPacker(BinPacker const &);

    BinPacker & operator=(BinPacker &&);
    BinPacker(BinPacker &&);

    ErrorCode RegisterItem(IDType id, Real w, Real h);

    //! Fill a bin with currently registered items.
    //!
    //! @param cmp Binary predicate taking two values of type 'Item'. This shall return true if the first item
    //!            goes before the second and false otherwise. Before filling a bin, items will be 
    //!            sorted according to ItemCompare. The default is to compare the area of the two input items, 
    //!            but could also be orientation or longest side for example.
    //!
    //! @param cutNode Binary predicate taking two arguments of type Real[2] and returning a object of type 'Cut'.
    //!                where argument 1 is the node size and argument 2 is the inserted rectangle size.
    //!                The algorithm works by partitioning the space in the bin into a tree of rectangular
    //!                nodes. When inserting a rectangle, we find an empty node and place the rectangle in 
    //!                the top left corner (0, 0). We now need to cut the node to create 2 child nodes.
    //!                The node is cut either horizontally or vertically, starting from the 
    //!                bottom right corner of the placed rectangle. Pictured below is an example of a 
    //!                vertical and horizontal cut. Typically the node is cut such that each child node is 
    //!                closest to a square, but you might want to define your own method to determine what 
    //!                cut should be used. For example, favouring vertical cuts might yield different 
    //!                results if most of the rectangles we insert are long, thin and vertical.
    //!    ________________              ________________ 
    //!   |      |         |            |      |         |  
    //!   | Rect |         |            | Rect |         |
    //!   |______|         |            |______|.........|
    //!   |      .         | Node       |         Cut    | Node
    //!   |      .Cut      |            |                |
    //!   |      .         |            |                |
    //!   |______._________|            |________________|
    //!
    //! @return Number of remaining items.
    template <typename Container, typename ItemCompare = DefaultItemCompareFn, typename CutSpace = DefaultCutNodeFn>
    size_t Fill(Container &, Real width, Real height, ItemCompare cmp = DefaultCompare, CutSpace cutNode = DefaultCutNode);

    void Clear();

  private:

    struct Rect
    {
      Real position[2];
      Real size[2];
    };

    template<typename Container, typename CutSpace>
    bool Insert(Container & a_bin, Item const &, Set_AVL<Rect> &spaces, CutSpace a_cutSpace);

  private:

    DoublyLinkedList<Item> m_inputItems;
  };

  //------------------------------------------------------------------------------------------------
  // BinPacker
  //------------------------------------------------------------------------------------------------

  template<typename Real, typename IDType>
  bool operator<(BinPacker<Real, IDType>::Rect const & a, BinPacker<Real, IDType>::Rect const & b)
  {
    return (a.size[0] * a.size[1]) < (b.size[0] * b.size[1]);
  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType>::BinPacker()
  {

  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType>::~BinPacker()
  {

  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType> & BinPacker<Real, IDType>::operator=(BinPacker const & a_other)
  {
    if (this != &a_other)
    {
      m_inputItems = a_other.m_inputItems;
      m_nodes = a_other.m_nodes;
    }
    return *this;
  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType>::BinPacker(BinPacker const & a_other)
    : m_inputItems(a_other.m_inputItems)
    , m_nodes(a_other.m_nodes)
  {

  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType> & BinPacker<Real, IDType>::operator=(BinPacker && a_other)
  {
    if (this != &a_other)
    {
      m_inputItems = std::move(a_other.m_inputItems);
      m_nodes = std::move(a_other.m_nodes);
    }
    return *this;
  }

  template<typename Real, typename IDType>
  BinPacker<Real, IDType>::BinPacker(BinPacker && a_other)
    : m_inputItems(std::move(a_other.m_inputItems))
    , m_nodes(std::move(a_other.m_nodes))
  {

  }

  template<typename Real, typename IDType>
  ErrorCode BinPacker<Real, IDType>::RegisterItem(IDType a_id, Real a_w, Real a_h)
  {
    ErrorCode result;

    if (a_w <= static_cast<Real>(0) || a_h <= static_cast<Real>(0))
      DG_ERROR_SET(ErrorCode::BadInput);

    Item item;
    item.id = a_id;
    item.xy[Element::width] = a_w;
    item.xy[Element::height] = a_h;

    m_inputItems.push_back(item);

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<typename Real, typename IDType>
  void BinPacker<Real, IDType>::Clear()
  {
    m_inputItems.clear();
    m_nodes.clear();
  }

  template<typename Real, typename IDType>
  template <typename Container, typename ItemCompare, typename CutSpace>
  size_t BinPacker<Real, IDType>::Fill(Container & a_bin, Real width, Real height, ItemCompare a_cmp, CutSpace a_cutSpace)
  {
    Set_AVL<Rect> spaces;
    spaces.push_back({static_cast<Real>(0), static_cast<Real>(0), width, height});
    m_inputItems.sort(a_cmp);

    for (DoublyLinkedList<Item>::iterator it = m_inputItems.begin(); it != m_inputItems.end();)
    {
      if (Insert(a_bin, *it, spaces, a_cutSpace))
      {
        a_bin.push_back(*it);
        it = m_inputItems.erase(it);
      }
      else
      {
        it++;
      }
    }
    return m_inputItems.size();
  }

  template<typename Real, typename IDType>
  template<typename Container, typename CutSpace>
  bool BinPacker<Real, IDType>::Insert(Container & a_bin, Item const & item, Set_AVL<Rect> & spaces, CutSpace a_cutSpace)
  {
    Rect r{static_cast<Real>(0), static_cast<Real>(0), item.xy[0], item.xy[1]};
    Set_AVL<Rect>::iterator it = spaces.lower_bound(r);

    if (it == spaces.end())
      return false;

    Rect ra = {it->position[0], it->position[1] + item.xy[1], static_cast<Real>(0), it->size[1] - item.xy[1]};
    Rect rb = {it->position[0] + item.xy[0], it->position[1], it->size[0] - item.xy[0], static_cast<Real>(0)};

    Cut cut = a_cutSpace(it->size, item.xy);

    if (cut == Cut::Vertical)
    {
      ra.size[0] = item.xy[0];
      rb.size[1] = it->size[1];
    }
    else
    {
      ra.size[0] = it->size[0];
      rb.size[1] = item.xy[1];
    }

    spaces.erase(it);
    if (ra.size[0] * ra.size[1] != static_cast<Real>(0))
      spaces.insert(ra);
    if (rb.size[0] * rb.size[1] != static_cast<Real>(0))
      spaces.insert(rb);

    return true;
  }

  template<typename Real, typename IDType>
  bool BinPacker<Real, IDType>::DefaultCompare(Item const & a_left, Item const & a_right)
  {
    Real areaLeft = a_left.xy[Element::width] * a_left.xy[Element::height];
    Real areaRight = a_right.xy[Element::width] * a_right.xy[Element::height];
    return areaLeft > areaRight;
  }

  template<typename Real, typename IDType>
  typename BinPacker<Real, IDType>::Cut BinPacker<Real, IDType>::DefaultCutNode(Real a_nodeSize[2], Real a_rectSize[2])
  {
    Real Av = (a_nodeSize[Element::height] - a_rectSize[Element::y]) * a_rectSize[Element::x];
    Real Bv = a_nodeSize[Element::height] * (a_nodeSize[Element::width] - a_rectSize[Element::x]);

    Real Ah = (a_nodeSize[Element::height] - a_rectSize[Element::y]) * a_nodeSize[Element::width];
    Real Bh = a_rectSize[Element::y] * (a_nodeSize[Element::width] - a_rectSize[Element::x]);

    Av *= Av;
    Bv *= Bv;
    Ah *= Ah;
    Bh *= Bh;

    if (Av + Bv > Ah + Bh)
      return Cut::Vertical;
    return Cut::Horizontal;
  }
}

#endif