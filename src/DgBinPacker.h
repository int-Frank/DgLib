//@group Misc

#ifndef DGBINPACKER_H
#define DGBINPACKER_H

#include <stdint.h>
#include <algorithm>

#include "DgTypes.h"
#include "DgBit.h"
#include "DgDynamicArray.h"

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

  private:

    class BranchNode
    {
    public:

      enum Child
      {
        A = 0,
        B = 1
      };

      enum Cut
      {
        Vertical   = 0,
        Horizontal = 1
      };

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

  public:

    BinPacker();
    ~BinPacker();

    BinPacker & operator=(BinPacker const &);
    BinPacker(BinPacker const &);

    BinPacker & operator=(BinPacker &&);
    BinPacker(BinPacker &&);

    BinPkr_ItemID RegisterItem(Real w, Real h);
    size_t Fill(Bin &); //returns number of leftover items

    void Clear();

  private:

    bool InsertItem(Item const &, Bin &);
    bool RecursiveInsert(Item const &, BranchNode *, Real nodeBounds[4], typename BranchNode::Child, Bin &);
    void SetCut(BranchNode *, Real nodeBounds[4]);
    bool GrowAndInsert(Item const & a_item, Bin &);
    void SortInput();

  private:

    uint64_t m_nextID;
    DynamicArray<Item> m_inputItems;
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
  typename BinPacker<Real>::BranchNode::Cut BinPacker<Real>::BranchNode::GetCut() const
  {
    return static_cast<Cut>(Dg::GetSubInt<uint64_t, 63, 1>(m_data));
  }

  template<typename Real>
  void BinPacker<Real>::BranchNode::SetCut(Cut a_cut)
  {
    m_data = Dg::SetSubInt<uint64_t, 63, 1>(m_data, a_cut);
  }

  template<typename Real>
  size_t BinPacker<Real>::BranchNode::GetChildIndex(Child a_child) const
  {
    uint64_t shft = 31ull * a_child;
    return static_cast<Cut>(Dg::GetSubInt<uint64_t, 0, 31>(m_data >> shft));
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
  BinPacker<Real>::BinPacker()
    : m_nextID(0)
  {

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
      m_inputItems = a_other.m_inputItems;
      m_nodes = a_other.m_nodes;
    }
    return *this;
  }

  template<typename Real>
  BinPacker<Real>::BinPacker(BinPacker const & a_other)
    : m_nextID(a_other.m_nextID)
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
      m_inputItems = std::move(a_other.m_inputItems);
      m_nodes = std::move(a_other.m_nodes);

      a_other.m_pBinGenreator = nullptr;
    }
    return *this;
  }

  template<typename Real>
  BinPacker<Real>::BinPacker(BinPacker && a_other)
    : m_nextID(a_other.m_nextID)
    , m_inputItems(std::move(a_other.m_inputItems))
    , m_nodes(std::move(a_other.m_nodes))
  {
    a_other.m_pBinGenreator = nullptr;
  }

  template<typename Real>
  BinPkr_ItemID BinPacker<Real>::RegisterItem(Real a_w, Real a_h)
  {
    if (a_w <= static_cast<Real>(0) || a_h <= static_cast<Real>(0))
      return static_cast<uint64_t>(BinPkrError::DimensionsLessEqZero);

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
    DynamicArray<Item> leftovers;

    m_nodes.clear();
    SortInput();

    for (Item const & item : m_inputItems)
    {
      if (!InsertItem(item, a_bin))
        leftovers.push_back(item);
    }

    m_inputItems = leftovers;
    m_nodes.clear();
    return m_inputItems.size();
  }

  template<typename Real>
  bool BinPacker<Real>::RecursiveInsert(Item const & a_item, BranchNode * a_pParent, Real a_parentBounds[4], typename BranchNode::Child a_child, Bin & a_bin)
  {
    Real itemWidth(a_item.xy[Element::width]);
    Real itemHeight(a_item.xy[Element::height]);

    Real nodeBounds[4];

    if (a_child == BranchNode::Child::A)
    {
      nodeBounds[Element::xmin] = a_parentBounds[Element::xmin];
      nodeBounds[Element::xmax] = a_pParent->GetCut() == BranchNode::Cut::Vertical ? a_parentBounds[Element::xmin] + a_pParent->offset[Element::x] : a_parentBounds[Element::xmax];
      nodeBounds[Element::ymin] = a_parentBounds[Element::ymin] + a_pParent->offset[Element::y];
      nodeBounds[Element::ymax] = a_parentBounds[Element::ymax];
    }
    else
    {
      nodeBounds[Element::xmin] = a_parentBounds[Element::xmin] + a_pParent->offset[Element::x];
      nodeBounds[Element::xmax] = a_parentBounds[Element::xmax];
      nodeBounds[Element::ymin] = a_parentBounds[Element::ymin];
      nodeBounds[Element::ymax] = a_pParent->GetCut() == BranchNode::Cut::Horizontal ? a_parentBounds[Element::ymin] + a_pParent->offset[Element::y] : a_parentBounds[Element::ymax];
    }

    Real nodeWidth = nodeBounds[Element::xmax] - nodeBounds[Element::xmin];
    Real nodeHeight = nodeBounds[Element::ymax] - nodeBounds[Element::ymin];

    if (a_pParent->IsLeaf(a_child))
    {
      if ((itemWidth <= nodeWidth) && (itemHeight <= nodeHeight))
      {
        Item item;
        item.id = a_item.id;
        item.xy[Element::x] = nodeBounds[Element::xmin];
        item.xy[Element::y] = nodeBounds[Element::ymin];
        a_bin.items.push_back(item);

        m_nodes.push_back(BranchNode());
        size_t ind = m_nodes.size() - 1;
        a_pParent->SetChildIndex(a_child, ind);
        BranchNode * pNewNode = &m_nodes[ind];

        pNewNode->offset[Element::x] = itemWidth;
        pNewNode->offset[Element::y] = itemHeight;
        SetCut(pNewNode, nodeBounds);

        return true;
      }
    }
    else
    {
      BranchNode * pNode = &m_nodes[a_pParent->GetChildIndex(a_child)];

      Real childNodeBounds[4];
      childNodeBounds[Element::xmin] = nodeBounds[Element::xmin];
      childNodeBounds[Element::xmax] = pNode->GetCut() == BranchNode::Cut::Vertical ? nodeBounds[Element::xmin] + pNode->offset[Element::x] : nodeBounds[Element::xmax];
      childNodeBounds[Element::ymin] = nodeBounds[Element::ymin] + pNode->offset[Element::y];
      childNodeBounds[Element::ymax] = nodeBounds[Element::ymax];

      if (RecursiveInsert(a_item, pNode, childNodeBounds, BranchNode::Child::A, a_bin))
        return true;

      childNodeBounds[Element::xmin] = nodeBounds[Element::xmin] + pNode->offset[Element::x];
      childNodeBounds[Element::xmax] = nodeBounds[Element::xmax];
      childNodeBounds[Element::ymin] = nodeBounds[Element::ymin];
      childNodeBounds[Element::ymax] = pNode->GetCut() == BranchNode::Cut::Horizontal ? nodeBounds[Element::ymin] + pNode->offset[Element::y] : nodeBounds[Element::ymax];

      if (RecursiveInsert(a_item, pNode, childNodeBounds, BranchNode::Child::B, a_bin))
        return true;
    }

    return false;
  }

  template<typename Real>
  void BinPacker<Real>::SetCut(BranchNode * a_pNode, Real a_nodeBounds[4])
  {
    Real x = a_pNode->offset[Element::x];
    Real y = a_pNode->offset[Element::y];

    Real nodeWidth = a_nodeBounds[Element::xmax]- a_nodeBounds[Element::xmin];
    Real nodeHeight =  a_nodeBounds[Element::ymax] - a_nodeBounds[Element::ymin];

    Real Av = (nodeHeight - y) * x;
    Real Bv = nodeHeight * (nodeWidth - x);

    Real Ah = (nodeHeight - y) * nodeWidth;
    Real Bh = y * (nodeWidth - x) ;

    Av *= Av;
    Bv *= Bv;
    Ah *= Ah;
    Bh *= Bh;

    if (Av + Bv > Ah + Bh)
      a_pNode->SetCut(BranchNode::Cut::Vertical);
    else
      a_pNode->SetCut(BranchNode::Cut::Horizontal);
  }

  template<typename Real>
  bool BinPacker<Real>::InsertItem(Item const & a_item, Bin & a_bin)
  {
    Real binBounds[4];
    binBounds[Element::xmin] = static_cast<Real>(0);
    binBounds[Element::ymin] = static_cast<Real>(0);
    binBounds[Element::xmax] = a_bin.dimensions[Element::width];
    binBounds[Element::ymax] = a_bin.dimensions[Element::height];

    bool result;
    if (m_nodes.empty())
    {
      //Create a dummy node
      BranchNode node;
      node.offset[Element::x] = static_cast<Real>(0);
      node.offset[Element::y] = static_cast<Real>(0);
      node.SetLeaf(BranchNode::Child::A);
      node.SetLeaf(BranchNode::Child::B);
      node.SetCut(BranchNode::Cut::Vertical);
      result = RecursiveInsert(a_item, &node, binBounds, BranchNode::Child::B, a_bin);
    }
    else
    {
      result = RecursiveInsert(a_item, &m_nodes[0], binBounds, BranchNode::Child::B, a_bin);
    }

    if (result)
      return true;

    if (!GrowAndInsert(a_item, a_bin))
      return false;

    return true;
  }

  template<typename Real>
  bool BinPacker<Real>::GrowAndInsert(Item const & a_item, Bin & a_bin)
  {
    Real width(a_item.xy[Element::width]);
    Real height(a_item.xy[Element::height]);
    Real binWidth(a_bin.dimensions[Element::width]);
    Real binHeight(a_bin.dimensions[Element::height]);
    Real maxBinWidth(a_bin.maxDimensions[Element::width]);
    Real maxBinHeight(a_bin.maxDimensions[Element::height]);

    if (width > binWidth && height > binHeight)
      return false;

    bool canGrowHeight = ((binHeight + height) <= maxBinHeight) && (width <= binWidth);
    bool canGrowWidth = ((binWidth + width) <= maxBinWidth) && (height <= binHeight);

    if (!canGrowHeight && !canGrowWidth)
      return false;

    int direction;

    if (!canGrowHeight)
    {
      direction = Element::width;
    }
    else if (!canGrowWidth)
    {
      direction = Element::height;
    }
    else
    {
      double aH = (static_cast<double>(binHeight) + height) / binWidth;
      double aW = (static_cast<double>(binWidth) + width) / binHeight;

      if (aH < 1.0) aH = 1.0 / aH;
      if (aW < 1.0) aW = 1.0 / aW;

      direction = (aH < aW) ? Element::height : Element::width;
    }

    for (Item & item : a_bin.items)
      item.xy[direction] += a_item.xy[direction];

    a_bin.dimensions[direction] += a_item.xy[direction];

    //Move the root
    if (m_nodes.size() > 0)
      m_nodes.push_back(BranchNode(m_nodes[0]));
    else
      m_nodes.push_back(BranchNode());

    m_nodes[0].offset[Element::x] = a_item.xy[Element::width];
    m_nodes[0].offset[Element::y] = a_item.xy[Element::height];
    
    if (direction == Element::width)
    {
      m_nodes[0].SetCut(BranchNode::Cut::Vertical);
      m_nodes[0].SetLeaf(BranchNode::Child::A);
      if (m_nodes.size() == 1)
        m_nodes[0].SetLeaf(BranchNode::Child::B);
      else
        m_nodes[0].SetChildIndex(BranchNode::Child::B, m_nodes.size() - 1);
    }
    else
    {
      m_nodes[0].SetCut(BranchNode::Cut::Horizontal);
      m_nodes[0].SetLeaf(BranchNode::Child::B);
      if (m_nodes.size() == 1)
        m_nodes[0].SetLeaf(BranchNode::Child::A);
      else
        m_nodes[0].SetChildIndex(BranchNode::Child::A, m_nodes.size() - 1);
    }

    Item item;
    item.id = a_item.id;
    item.xy[0] = static_cast<Real>(0);
    item.xy[1] = static_cast<Real>(0);

    a_bin.items.push_back(item);
    return true;
  }

  template<typename Real>
  void BinPacker<Real>::SortInput()
  {
    std::sort(m_inputItems.data(), 
              m_inputItems.data() + m_inputItems.size(),
      [](Item const & a_left, Item const & a_right)
      {
        Real areaLeft = a_left.xy[Element::width] * a_left.xy[Element::height];
        Real areaRight = a_right.xy[Element::width] * a_right.xy[Element::height];
        return areaLeft > areaRight;
      });
  }
}

#endif