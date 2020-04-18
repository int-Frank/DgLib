//@group Misc

#ifndef DGBINPACKER_H
#define DGBINPACKER_H

#include <stdint.h>
#include <algorithm>

#include "DgTypes.h"
#include "DgBit.h"
#include "DgDynamicArray.h"
#include "DgDoublyLinkedlist.h"

namespace Dg
{
  typedef uint64_t BinPkr_ItemID;

  enum class BinPkrError : uint64_t
  {
    None                  = 0,
    DimensionsLessEqZero  = 1ULL << 60
  };

  BinPkrError BinPkr_GetError(BinPkr_ItemID a_id)
  {
    return static_cast<BinPkrError>(a_id & 0xF000'0000'0000'0000);
  }

  template<typename Real>
  class BinPacker
  {
  public:

    struct Item
    {
      BinPkr_ItemID id;
      Real xy[2];
    };

    struct Bin
    {
      Real dimensions[2];
      Real maxDimensions[2];
      DynamicArray<Item> items;
    };

    enum class Cut
    {
      Vertical   = 0,
      Horizontal = 1
    };

    //TODO these should be classes
    typedef bool (*ItemCompare)(Item const &, Item const &);
    typedef Cut (*CutNode)(Real nodeSize[2], Real rectangleSize[2]);

  public:

    // Before filling a bin, items will be sorted according to ItemCompare. The default
    // is to compare the area of the two input items, but could also be orientation or 
    // longest side for example.
    BinPacker(ItemCompare fn = nullptr);

    // The algorithm works by partitioning the space in the bin into a tree of rectangular
    // nodes. When inserting a rectangle, we find an empty node and place the rectangle in 
    // the top left corner (0, 0). We now need to cut the node to create 2 child nodes.
    // The node is cut either horizontally or vertically, starting from the 
    // bottom right corner of the placed rectangle. Pictured below is an example of a 
    // vertical and horizontal cut. Typically the node is cut such that each child node is 
    // closest to a square, but you might want to define your own method to determine what 
    // cut should be used. For example, favouring vertical cuts might yield different 
    // results if most of the rectangles we insert are long, thin and vertical.
    //    ________________              ________________ 
    //   |      |         |            |      |         |  
    //   | Rect |         |            | Rect |         |
    //   |______|         |            |______|.........|
    //   |      .         | Node       |         Cut    | Node
    //   |      .Cut      |            |                |
    //   |      .         |            |                |
    //   |______._________|            |________________|
    //
    BinPacker(ItemCompare, CutNode);
    ~BinPacker();

    BinPacker & operator=(BinPacker const &);
    BinPacker(BinPacker const &);

    BinPacker & operator=(BinPacker &&);
    BinPacker(BinPacker &&);

    void SetCompareFn(ItemCompare);
    void SetCutNodeFn(CutNode);

    BinPkr_ItemID RegisterItem(Real w, Real h);

    //Returns number of leftover items.
    size_t Fill(Bin &);

    void Clear();

  private:

    class BranchNode
    {
    public:

      enum Child { A = 0, B = 1 };

      BranchNode();

      Cut GetCut() const;
      void SetCut(Cut);
      void SetChildIndex(Child, size_t);
      size_t GetChildIndex(Child) const;
      void SetLeaf(Child);
      bool IsLeaf(Child) const;

      Real offset[2];

    private:
      uint64_t m_data;
    };

  private:

    bool RecursiveInsert(Item const &, size_t nodeIndex, Real nodeBounds[4], typename BranchNode::Child, Bin &);
    void Expand(Bin &, int direction, Real x, Real y);
    bool ExpandAndInsert(Item const & a_item, Bin &);

    static bool DefaultCompare(Item const &, Item const &);
    static Cut DefaultCutNode(Real nodeSize[2], Real rectSize[2]);

  private:

    BinPkr_ItemID m_nextID;
    ItemCompare m_fnCompare;
    CutNode m_fnCutNode;
    DoublyLinkedList<Item> m_inputItems;
    DynamicArray<BranchNode>  m_nodes;
  };

  //------------------------------------------------------------------------------------------------
  // BranchNode
  //------------------------------------------------------------------------------------------------

  template<typename Real>
  BinPacker<Real>::BranchNode::BranchNode()
    : m_data(0)
    , offset{static_cast<Real>(0), static_cast<Real>(0)}
  {
    SetLeaf(Child::A);
    SetLeaf(Child::B);
  }

  template<typename Real>
  typename BinPacker<Real>::Cut BinPacker<Real>::BranchNode::GetCut() const
  {
    return static_cast<Cut>(Dg::GetSubInt<uint64_t, 63, 1>(m_data));
  }

  template<typename Real>
  void BinPacker<Real>::BranchNode::SetCut(Cut a_cut)
  {
    m_data = Dg::SetSubInt<uint64_t, 63, 1>(m_data, static_cast<uint64_t>(a_cut));
  }

  template<typename Real>
  size_t BinPacker<Real>::BranchNode::GetChildIndex(Child a_child) const
  {
    uint64_t shft = 31ull * a_child;
    return Dg::GetSubInt<uint64_t, 0, 31>(m_data >> shft);
  }

  template<typename Real>
  void BinPacker<Real>::BranchNode::SetChildIndex(Child a_child, size_t a_index)
  {
    uint64_t shft = 31ull * a_child;
    m_data = Dg::SetSubInt<uint64_t>(m_data, a_index, shft, 31);
  }

  template<typename Real>
  void BinPacker<Real>::BranchNode::SetLeaf(Child a_child)
  {
    uint64_t shft = 31ull * a_child;
    m_data = Dg::SetSubInt<uint64_t>(m_data, Dg::Mask<uint64_t, 0, 31>::value, shft, 31);
  }

  template<typename Real>
  bool BinPacker<Real>::BranchNode::IsLeaf(Child a_child) const
  {
    uint64_t shft = 31ull * a_child;
    return Dg::GetSubInt<uint64_t, 0, 31>(m_data >> shft) == Dg::Mask<uint64_t, 0, 31>::value;
  }

  //------------------------------------------------------------------------------------------------
  // BinPacker
  //------------------------------------------------------------------------------------------------

  template<typename Real>
  BinPacker<Real>::BinPacker(ItemCompare a_fnCmp)
    : m_nextID(0)
    , m_fnCompare(a_fnCmp)
    , m_fnCutNode(DefaultCutNode)
  {
    if (m_fnCompare == nullptr)
      m_fnCompare = DefaultCompare;
  }

  template<typename Real>
  BinPacker<Real>::BinPacker(ItemCompare a_fnCmp, CutNode a_fnCut)
    : m_nextID(0)
    , m_fnCompare(a_fnCmp)
    , m_fnCutNode(a_fnCut)
  {
    if (m_fnCompare == nullptr)
      m_fnCompare = DefaultCompare;
    if (m_fnCutNode == nullptr)
      m_fnCutNode = DefaultCutNode;
  }

  template<typename Real>
  BinPacker<Real>::~BinPacker()
  {

  }

  template<typename Real>
  BinPacker<Real> & BinPacker<Real>::operator=(BinPacker const & a_other)
  {
    if (this != &a_other)
    {
      m_nextID = a_other.m_nextID;
      m_fnCompare = a_other.m_fnCompare;
      m_fnCutNode = a_other.m_fnCutNode;
      m_inputItems = a_other.m_inputItems;
      m_nodes = a_other.m_nodes;
    }
    return *this;
  }

  template<typename Real>
  BinPacker<Real>::BinPacker(BinPacker const & a_other)
    : m_nextID(a_other.m_nextID)
    , m_fnCompare(a_other.m_fnCompare)
    , m_fnCutNode(a_other.m_fnCutNode)
    , m_inputItems(a_other.m_inputItems)
    , m_nodes(a_other.m_nodes)
  {

  }

  template<typename Real>
  BinPacker<Real> & BinPacker<Real>::operator=(BinPacker && a_other)
  {
    if (this != &a_other)
    {
      m_nextID = a_other.m_nextID;
      m_fnCompare = a_other.m_fnCompare;
      m_fnCutNode = a_other.m_fnCutNode;
      m_inputItems = std::move(a_other.m_inputItems);
      m_nodes = std::move(a_other.m_nodes);

      a_other.m_pBinGenreator = nullptr;
    }
    return *this;
  }

  template<typename Real>
  BinPacker<Real>::BinPacker(BinPacker && a_other)
    : m_nextID(a_other.m_nextID)
    , m_fnCompare(std::move(a_other.m_fnCompare))
    , m_fnCutNode(std::move(a_other.m_fnCutNode))
    , m_inputItems(std::move(a_other.m_inputItems))
    , m_nodes(std::move(a_other.m_nodes))
  {
    a_other.m_pBinGenreator = nullptr;
  }

  template<typename Real>
  void BinPacker<Real>::SetCompareFn(ItemCompare a_fn)
  {
    if (a_fn != nullptr)
      m_fnCompare = a_fn;
  }

  template<typename Real>
  void BinPacker<Real>::SetCutNodeFn(CutNode a_fn)
  {
    if (a_fn != nullptr)
      m_fnCutNode = a_fn;
  }

  template<typename Real>
  BinPkr_ItemID BinPacker<Real>::RegisterItem(Real a_w, Real a_h)
  {
    if (a_w <= static_cast<Real>(0) || a_h <= static_cast<Real>(0))
      return static_cast<BinPkr_ItemID>(BinPkrError::DimensionsLessEqZero);

    Item item;
    item.id = m_nextID;
    m_nextID++;
    item.xy[Element::width] = a_w;
    item.xy[Element::height] = a_h;

    m_inputItems.push_back(item);
    return item.id;
  }

  template<typename Real>
  void BinPacker<Real>::Clear()
  {
    m_inputItems.clear();
    m_nodes.clear();
  }

  template<typename Real>
  size_t BinPacker<Real>::Fill(Bin & a_bin)
  {
    m_nodes.clear();

    //Insert root node
    m_nodes.push_back(BranchNode());
    m_nodes.back().offset[Element::x] = static_cast<Real>(0);
    m_nodes.back().offset[Element::y] = static_cast<Real>(0);
    m_nodes.back().SetLeaf(BranchNode::Child::A);
    m_nodes.back().SetLeaf(BranchNode::Child::B);
    m_nodes.back().SetCut(Cut::Horizontal);

    m_inputItems.sort(m_fnCompare);

    for (DoublyLinkedList<Item>::iterator it = m_inputItems.begin(); it != m_inputItems.end();)
    {
      Real binBounds[4];
      binBounds[Element::xmin] = static_cast<Real>(0);
      binBounds[Element::ymin] = static_cast<Real>(0);
      binBounds[Element::xmax] = a_bin.dimensions[Element::width];
      binBounds[Element::ymax] = a_bin.dimensions[Element::height];

      if (RecursiveInsert(*it, 0, binBounds, BranchNode::Child::A, a_bin))
        goto success;

      if (RecursiveInsert(*it, 0, binBounds, BranchNode::Child::B, a_bin))
        goto success;

      if (ExpandAndInsert(*it, a_bin))
        goto success;

      it++;
      continue;
    success:
      it = m_inputItems.erase(it);
    }

    m_nodes.clear();
    return m_inputItems.size();
  }

  template<typename Real>
  bool BinPacker<Real>::RecursiveInsert(Item const & a_item, size_t a_parentNodeIndex, Real a_parentBounds[4], typename BranchNode::Child a_child, Bin & a_bin)
  {
    Real nodeBounds[4];
    if (a_child == BranchNode::Child::A)
    {
      nodeBounds[Element::xmin] = a_parentBounds[Element::xmin];
      nodeBounds[Element::xmax] = m_nodes[a_parentNodeIndex].GetCut() == Cut::Vertical ? a_parentBounds[Element::xmin] + m_nodes[a_parentNodeIndex].offset[Element::x] : a_parentBounds[Element::xmax];
      nodeBounds[Element::ymin] = a_parentBounds[Element::ymin] + m_nodes[a_parentNodeIndex].offset[Element::y];
      nodeBounds[Element::ymax] = a_parentBounds[Element::ymax];
    }
    else
    {
      nodeBounds[Element::xmin] = a_parentBounds[Element::xmin] + m_nodes[a_parentNodeIndex].offset[Element::x];
      nodeBounds[Element::xmax] = a_parentBounds[Element::xmax];
      nodeBounds[Element::ymin] = a_parentBounds[Element::ymin];
      nodeBounds[Element::ymax] = m_nodes[a_parentNodeIndex].GetCut() == Cut::Horizontal ? a_parentBounds[Element::ymin] + m_nodes[a_parentNodeIndex].offset[Element::y] : a_parentBounds[Element::ymax];
    }

    Real nodeSize[2] = {nodeBounds[Element::xmax] - nodeBounds[Element::xmin],
                        nodeBounds[Element::ymax] - nodeBounds[Element::ymin]};

    if (m_nodes[a_parentNodeIndex].IsLeaf(a_child))
    {
      if ((a_item.xy[Element::width] <= nodeSize[Element::width]) && (a_item.xy[Element::height] <= nodeSize[Element::height]))
      {
        Item item;
        item.id = a_item.id;
        item.xy[Element::x] = nodeBounds[Element::xmin];
        item.xy[Element::y] = nodeBounds[Element::ymin];
        a_bin.items.push_back(item);

        m_nodes.push_back(BranchNode());
        size_t ind = m_nodes.size() - 1;

        m_nodes[a_parentNodeIndex].SetChildIndex(a_child, ind);

        m_nodes[ind].offset[Element::x] = a_item.xy[Element::width];
        m_nodes[ind].offset[Element::y] = a_item.xy[Element::height];
        m_nodes[ind].SetCut(m_fnCutNode(nodeSize, m_nodes[ind].offset));

        return true;
      }
    }
    else
    {
      size_t ind = m_nodes[a_parentNodeIndex].GetChildIndex(a_child);

      if (RecursiveInsert(a_item, ind, nodeBounds, BranchNode::Child::A, a_bin))
        return true;

      if (RecursiveInsert(a_item, ind, nodeBounds, BranchNode::Child::B, a_bin))
        return true;
    }

    return false;
  }

  template<typename Real>
  void BinPacker<Real>::Expand(Bin & a_bin, int a_direction, Real a_x, Real a_y)
  {
    Real xy[2] = {a_x, a_y};
    for (Item & item : a_bin.items)
      item.xy[a_direction] += xy[a_direction];

    a_bin.dimensions[a_direction] += xy[a_direction];

    m_nodes.push_back(BranchNode(m_nodes[0]));

    if (a_direction == Element::width)
    {
      m_nodes[0].SetCut(Cut::Vertical);
      m_nodes[0].SetLeaf(BranchNode::Child::A);
      m_nodes[0].SetChildIndex(BranchNode::Child::B, m_nodes.size() - 1);
    }
    else
    {
      m_nodes[0].SetCut(Cut::Horizontal);
      m_nodes[0].SetLeaf(BranchNode::Child::B);
      m_nodes[0].SetChildIndex(BranchNode::Child::A, m_nodes.size() - 1);
    }

    m_nodes[0].offset[Element::x] = xy[Element::width];
    m_nodes[0].offset[Element::y] = xy[Element::height];
  }

  template<typename Real>
  bool BinPacker<Real>::ExpandAndInsert(Item const & a_item, Bin & a_bin)
  {
    Real width(a_item.xy[Element::width]);
    Real height(a_item.xy[Element::height]);
    Real binWidth(a_bin.dimensions[Element::width]);
    Real binHeight(a_bin.dimensions[Element::height]);
    Real maxBinWidth(a_bin.maxDimensions[Element::width]);
    Real maxBinHeight(a_bin.maxDimensions[Element::height]);

    bool canExpandHeight = ((binHeight + height) <= maxBinHeight) && (width <= binWidth);
    bool canExpandWidth = ((binWidth + width) <= maxBinWidth) && (height <= binHeight);

    if (!canExpandHeight && !canExpandWidth)
      return false;

    int direction;

    //special case where we need to expand in both directions
    if (width > binWidth && height > binHeight && canExpandHeight && canExpandWidth)
    {
      Expand(a_bin, Element::width, a_item.xy[Element::width], static_cast<Real>(0));
      direction = Element::height;
    }
    else
    {
      if (!canExpandHeight)
      {
        direction = Element::width;
      }
      else if (!canExpandWidth)
      {
        direction = Element::height;
      }
      else
      {
        //Try to keep a 'square' shape
        double aH = (static_cast<double>(binHeight) + height) / binWidth;
        double aW = (static_cast<double>(binWidth) + width) / binHeight;

        if (aH < 1.0) aH = 1.0 / aH;
        if (aW < 1.0) aW = 1.0 / aW;

        direction = (aH < aW) ? Element::height : Element::width;
      }
    }

    Expand(a_bin, direction, a_item.xy[Element::width], a_item.xy[Element::height]);
    
    Item item;
    item.id = a_item.id;
    item.xy[0] = static_cast<Real>(0);
    item.xy[1] = static_cast<Real>(0);

    a_bin.items.push_back(item);
    return true;
  }

  template<typename Real>
  bool BinPacker<Real>::DefaultCompare(Item const & a_left, Item const & a_right)
  {
    Real areaLeft = a_left.xy[Element::width] * a_left.xy[Element::height];
    Real areaRight = a_right.xy[Element::width] * a_right.xy[Element::height];
    return areaLeft > areaRight;
  }

  template<typename Real>
  typename BinPacker<Real>::Cut BinPacker<Real>::DefaultCutNode(Real a_nodeSize[2], Real a_rectSize[2])
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