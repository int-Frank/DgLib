//@group Math/Geometry/Query

#ifndef DGQUERYPOLYGONPOLYGON_H
#define DGQUERYPOLYGONPOLYGON_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPolygon.h"
#include "DgQueryPointPolygon.h"
#include "DgQuerySegmentSegment.h"
#include "DgQueryPointSegment.h"
#include "DgDynamicArray.h"
#include "DgSet_AVL.h"

namespace Dg
{
  template <typename Real>
  class Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;
    };

    Result operator()(Polygon2<Real> const &, Polygon2<Real> const &);
  };

  template <typename Real>
  class Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      QueryCode code;

      Dg::DynamicArray<Vector2<Real>> vertices;

      Dg::DynamicArray<uint32_t> boundary;
      Dg::DynamicArray<Dg::DynamicArray<uint32_t>> polyA;
      Dg::DynamicArray<Dg::DynamicArray<uint32_t>> polyB;
      Dg::DynamicArray<Dg::DynamicArray<uint32_t>> intersection;
      Dg::DynamicArray<Dg::DynamicArray<uint32_t>> holes;
    };

    Result operator()(Polygon2<Real> const &A, Polygon2<Real> const &B);
  };

  //---------------------------------------------------------------------------------------
  // Useful typedefs
  //---------------------------------------------------------------------------------------

  template<typename Real>
  using TI2PolygonPolygon = Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>;

  template<typename Real>
  using FI2PolygonPolygon = Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>;

  //---------------------------------------------------------------------------------------
  // TestForIntersection Implementation
  //---------------------------------------------------------------------------------------

  template<typename Real>
  typename Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::Result
    Query<QueryType::TestForIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::operator()
    (Polygon2<Real>  const &poly_a, Polygon2<Real> const &poly_b)
  {
    Result result;
    for (auto ea = poly_a.cEdgesBegin(); ea != poly_a.cEdgesEnd(); ea++)
    {
      Segment2<Real> sa = *(ea);
      for (auto eb = poly_b.cEdgesBegin(); eb != poly_b.cEdgesEnd(); eb++)
      {
        Segment2<Real> sb = *(eb);

        TI2SegmentSegment<Real> query;
        TI2SegmentSegment<Real>::Result tempResult = query(sa, sb);

        if (tempResult.code != QueryCode::NotIntersecting)
        {
          result.code = Dg::QueryCode::Intersecting;
          goto epilogue;
        }
      }
    }

    TI2PointPolygon<Real> query;
    TI2PointPolygon<Real>::Result tempResult;
      
    tempResult = query(*poly_b.cPointsBegin(), poly_a);
    if (tempResult.code != Dg::QueryCode::CompletelyOutside)
    {
      result.code = Dg::QueryCode::B_in_A;
      goto epilogue;
    }
    
    tempResult = query(*poly_a.cPointsBegin(), poly_b);
    if (tempResult.code != Dg::QueryCode::CompletelyOutside)
    {
      result.code = Dg::QueryCode::A_in_B;
      goto epilogue;
    }

    result.code = Dg::QueryCode::NotIntersecting;
  epilogue:
    return result;
  }

  //---------------------------------------------------------------------------------------
  // FindIntersection Implementation
  //---------------------------------------------------------------------------------------

  namespace impl_FI2PolygonPolygon
  {
    enum EdgeFlag : uint32_t
    {
      EF_None     = 0,
      EF_InsideA  = 1 << 0,
      EF_InsideB  = 1 << 1,

      EF_InsideBoth = EF_InsideA | EF_InsideB,
    };

    enum NodeIndex : uint32_t
    {
      NI_Invalid = 0xFFFFFFFF
    };

    struct Neighbour
    {
      uint32_t id;
      uint32_t flags;
    };

    class EdgePair
    {
    public:

      EdgePair(uint64_t e0, uint64_t e1);

      uint64_t edge0;
      uint64_t edge1;
    };

    bool EdgePairLess(EdgePair const &p0, EdgePair const &p1);

    uint64_t GetUndirectedEdgeID(uint32_t a, uint32_t b);
    uint64_t GetDirectedEdgeID(uint32_t a, uint32_t b);

    // Returns a value (1 <= x < -1) depending on the orientation of vector b to vector a.
    //
    //     1       : a and b are aligned
    // 1 > x > 0   : b is left of  a
    //     0       : a and b are in opposite directions
    // 0 > x > -1  : b is right of  a
    //
    template<typename Real>
    Real GetOrientation(Vector2<Real> a, Vector2<Real> b)
    {
      Real aLenSq = Dg::MagSq(a);
      if (aLenSq == Real(0))
        a = XAxis<Real, 2>();
      else
        a = a / sqrt(aLenSq);

      Real bLenSq = Dg::MagSq(b);
      if (bLenSq == Real(0))
        b = XAxis<Real, 2>();
      else
        b = b / sqrt(bLenSq);

      Real dot = Dot(a, b);
      Real perp = PerpDot(a, b);
      Real result;

      if (perp > Real(0))
        result = Real(0.5) + dot / Real(2);
      else if (perp < Real(0))
        result = Real(-0.5) - dot / Real(2);
      else if (dot > 0) // a and b aligned
        result = Real(1);
      else // a and b in opposite directions
        result = Real(0);

      return result;
    }

    template<typename Real>
    struct Node
    {
      Vector2<Real> vertex;
      Dg::DynamicArray<Neighbour> neighbours;
    };

    template<typename Real>
    struct Graph
    {
      Dg::DynamicArray<Node<Real>> nodes;
    };
    
    template<typename Real>
    class PolygonExtractor
    {
    public:

      void Execute(Graph<Real> const *pGraph, typename FI2PolygonPolygon<Real>::Result *pResult)
      {
        Dg::Set_AVL<uint64_t> processedEdges;

        for (uint32_t i = 0; i < (uint32_t)pGraph->nodes.size(); i++)
          pResult->vertices.push_back(pGraph->nodes[i].vertex);

        ExtractBoundary(pGraph, pResult, &processedEdges);

        for (uint32_t i = 0; i < (uint32_t)pGraph->nodes.size(); i++)
        {
          Node<Real> const *pNode = &pGraph->nodes[i];
          for (uint32_t j = 0; j < (uint32_t)pNode->neighbours.size(); j++)
          {
            uint64_t key = GetDirectedEdgeID(i, pNode->neighbours[j].id);

            if (processedEdges.exists(key))
              continue;

            ExtractPolygon(pGraph, i, &pNode->neighbours[j], pResult, &processedEdges);
          }
        }
      }

    private:

      void ExtractBoundary(Graph<Real> const *pGraph,
                           typename FI2PolygonPolygon<Real>::Result *pResult,
                           Dg::Set_AVL<uint64_t> *pProcessedEdges)
      {
        Vector2<Real> current = pGraph->nodes[0].vertex;
        uint32_t id = 0;
        for (uint32_t i = 1; i < (uint32_t)pGraph->nodes.size(); i++)
        {
          Vector2<Real> temp = pGraph->nodes[i].vertex;

          if (temp.y() > current.y())
            continue;

          if (temp.y() < current.y() || temp.x() < current.x())
          {
            id = i;
            current = temp;
          }
        }

        Node<Real> const *pNode = &pGraph->nodes[id];
        uint32_t neighbourIndex = TurnLeft(pGraph, NI_Invalid, pNode, XAxis<Real, 2>());
        uint32_t flags = 0;
        _ExtractPolygon(pGraph, id, &pNode->neighbours[neighbourIndex], pProcessedEdges, &pResult->boundary, &flags);
      }

      void _ExtractPolygon(Graph<Real> const *pGraph,
                          uint32_t currentID,
                          Neighbour const *pNeighbour,
                          Dg::Set_AVL<uint64_t> *pProcessedEdges,
                          Dg::DynamicArray<uint32_t> *pPolygon,
                          uint32_t *pFlags)
      {
        uint32_t startID = currentID;
        *pFlags = 0;

        do
        {
          Node<Real> const *pCurrent = &pGraph->nodes[currentID];
          uint32_t nextID = pNeighbour->id;
          Node<Real> const *pNext = &pGraph->nodes[nextID];

          // Output point
          pPolygon->push_back(currentID);
          *pFlags = *pFlags | pNeighbour->flags;

          // Register edge
          uint64_t key = GetDirectedEdgeID(currentID, nextID);
          pProcessedEdges->insert(key);

          // Move to next edge
          Vector2<Real> vecA = pNext->vertex - pCurrent->vertex;
          uint32_t nextIndex = TurnLeft(pGraph, currentID, pNext, vecA);
          currentID = nextID;
          pNeighbour = &pNext->neighbours[nextIndex];
          
        } while (currentID != startID);
      }

      void ExtractPolygon(Graph<Real> const *pGraph, 
                          uint32_t currentID, 
                          Neighbour const *pNeighbour,
                          typename FI2PolygonPolygon<Real>::Result *pResult, 
                          Dg::Set_AVL<uint64_t> *pProcessedEdges)
      {
        uint32_t flags = 0;
        Dg::DynamicArray<uint32_t> polygon;
        _ExtractPolygon(pGraph, currentID, pNeighbour, pProcessedEdges, &polygon, &flags);

        switch (flags)
        {
        case EF_None:
        {
          pResult->holes.push_back(polygon);
          break;
        }
        case EF_InsideA:
        {
          pResult->polyA.push_back(polygon);
          break;
        }
        case EF_InsideB:
        {
          pResult->polyB.push_back(polygon);
          break;
        }
        default:
        {
          pResult->intersection.push_back(polygon);
          break;
        }
        }
      }

      // Returns the index of neighbour from pCurrent
      uint32_t TurnLeft(Graph<Real> const *pGraph, uint32_t prevID, Node<Real> const *pCurrent, Vector2<Real> const &vPrevCurrent)
      {
        uint32_t neighbourIndex = 0;
        Real currentOrientation = Real(42); // Just pick a number higher than 2pi

        for (uint32_t i = 0; i < (uint32_t)pCurrent->neighbours.size(); i++)
        {
          uint32_t neighbourID = pCurrent->neighbours[i].id;

          if (neighbourID == prevID)
            continue;

          Node<Real> const *pNeighbour = &pGraph->nodes[neighbourID];
          Vector2<Real> p2 = pNeighbour->vertex;
          Vector2<Real> vecB = p2 - pCurrent->vertex;

          Real orientation = GetOrientation(-vPrevCurrent, vecB);
          if (orientation < currentOrientation)
          {
            currentOrientation = orientation;
            neighbourIndex = i;
          }
        }

        return neighbourIndex;
      }
    };

    template<typename Real>
    class PolygonsToGraph
    {
    public:

      QueryCode Execute(Polygon2<Real> const &polyA, Polygon2<Real> const &polyB, Graph<Real> *pGraph, Real epsilon = Constants<Real>::EPSILON)
      {
        if (pGraph == nullptr)
          return QueryCode::Fail;

        pGraph->nodes.clear();

        if (!TryAddPolygon(pGraph, polyA, EF_InsideA))
          return QueryCode::Fail;

        if (!TryAddPolygon(pGraph, polyB, EF_InsideB))
          return QueryCode::Fail;

        return QueryCode::Success;
      }

    private:

      bool TryAddPolygon(Graph<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
      {
        Orientation winding = polygon.Winding();
        if (winding == Orientation::CW)
          return TryAddCWPolygon(pGraph, polygon, flag);
        else if (winding == Orientation::CCW)
          return TryAddCCWPolygon(pGraph, polygon, flag);
        return false;
      }

      bool TryAddCWPolygon(Graph<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
      {
        uint32_t count = (uint32_t)polygon.Size();
        uint32_t startIndex = (uint32_t)pGraph->nodes.size();
        uint32_t currentLocalIndex = 0;

        for (auto it = polygon.cPointsBegin(); it != polygon.cPointsEnd(); it++)
        {
          uint32_t currentIndex = startIndex + currentLocalIndex;
          uint32_t nextIndex = startIndex + (currentLocalIndex + count - 1) % count;
          uint32_t previousIndex = startIndex + (currentLocalIndex + 1) % count;

          Node<Real> node;
          node.vertex = *it;
          node.neighbours.push_back({ previousIndex, EF_None });
          node.neighbours.push_back({ nextIndex, flag });

          pGraph->nodes.push_back(node);

          currentLocalIndex++;
        }

        return true;
      }

      bool TryAddCCWPolygon(Graph<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
      {
        uint32_t count = (uint32_t)polygon.Size();
        uint32_t startIndex = (uint32_t)pGraph->nodes.size();
        uint32_t currentLocalIndex = 0;

        for (auto it = polygon.cPointsBegin(); it != polygon.cPointsEnd(); it++)
        {
          uint32_t currentIndex = startIndex + currentLocalIndex;
          uint32_t previousIndex = startIndex + (currentLocalIndex + count - 1) % count;
          uint32_t nextIndex = startIndex + (currentLocalIndex + 1) % count;

          Node<Real> node;
          node.vertex = *it;
          node.neighbours.push_back({ previousIndex, EF_None });
          node.neighbours.push_back({ nextIndex, flag });

          pGraph->nodes.push_back(node);

          currentLocalIndex++;
        }

        return true;
      }
    };

    // Polygon must not:
    // - have vertices within an epsilon of each other
    // - must not be self intersecting, and no  overlapping edges
    // - must not be a degenerate polygon
    template<typename Real>
    class GraphBuilder
    {
    public:

      //void PrintGraph(Graph<Real> const *pGraph)
      //{
      //  std::cout << "\nGraph\n";
      //
      //  for (size_t n = 0; n < pGraph->nodes.size(); n++)
      //  {
      //    Node<Real> const *pNode = &pGraph->nodes[n];
      //
      //    std::cout << n << ": [" << pNode->vertex.x() << ", " << pNode->vertex.y() << "] [";
      //    for (size_t i = 0; i < pNode->neighbours.size(); i++)
      //    {
      //      std::cout << pNode->neighbours[i].id;
      //      if (i + 1 != pNode->neighbours.size())
      //        std::cout << ", ";
      //    }
      //    std::cout << "]\n";
      //  }
      //}

      QueryCode Execute(Graph<Real> *pGraph, Real epsilon = Constants<Real>::EPSILON)
      {
        int hasIntersections = 0;

        hasIntersections += MergeNodes(pGraph, epsilon) ? 0 : 1;
        hasIntersections += MergeNodeAndEdges(pGraph, epsilon) ? 0 : 1;
        hasIntersections += FindIntersections(pGraph, epsilon) ? 0 : 1;

        return hasIntersections > 0 ? QueryCode::Intersecting : QueryCode::NotIntersecting;
      }

    private:

      bool FindIntersections(Graph<Real> *pGraph, Real epsilon)
      {
        // TODO testedEdgePairs should be a hash set
        Set_AVL<EdgePair, EdgePairLess> testedEdgePairs;
        bool hasIntersections = false;

        for (uint32_t id00 = 0; id00 < (uint32_t)pGraph->nodes.size(); id00++)
        {
          for (uint32_t id10 = id00 + 1; id10 < (uint32_t)pGraph->nodes.size(); )
          {
            // Continue processing until there are no more intersections
            // coming from this node pair.
            if (ProcessNodePair(pGraph, id00, id10, &testedEdgePairs))
              hasIntersections = true;
            else
              id10++;
          }
        }
        return hasIntersections;
      }

      bool ProcessNodePair(Graph<Real> *pGraph, uint32_t id00, uint32_t id10, Set_AVL<EdgePair, EdgePairLess> *pTestedEdgePairs)
      {
        Node<Real> *pNode00 = &pGraph->nodes[id00];
        Node<Real> *pNode10 = &pGraph->nodes[id10];

        for (size_t n0 = 0; n0 < pNode00->neighbours.size(); n0++)
        {
          uint32_t id01 = pNode00->neighbours[n0].id;

          if (id01 == id10)
            continue;

          Node<Real> *pNode01 = &pGraph->nodes[id01];
          Segment2<Real> seg0(pNode00->vertex, pNode01->vertex);

          for (size_t n1 = 0; n1 < pNode10->neighbours.size(); n1++)
          {
            uint32_t id11 = pNode10->neighbours[n1].id;

            if (id11 == id00 || id11 == id01)
              continue;

            EdgePair edgePair(GetUndirectedEdgeID(id00, id01), GetUndirectedEdgeID(id10, id11));

            if (pTestedEdgePairs->exists(edgePair))
              continue;

            pTestedEdgePairs->insert(edgePair);

            Node<Real> *pNode11 = &pGraph->nodes[id11];
            Segment2<Real> seg1(pNode10->vertex, pNode11->vertex);

            FI2SegmentSegment<Real> query;
            auto result = query(seg0, seg1);

            if (result.code != QueryCode::Intersecting)
              continue;

            DoEdgeEdgeIntersection(pGraph, id00, id01, id10, id11, result.pointResult.point);
            return true;
          }
        }

        return false;
      }

      static void DoEdgeEdgeIntersection(Graph<Real> *pGraph, uint32_t id00, uint32_t id01, uint32_t id10, uint32_t id11, Vector2<Real> const &point)
      {
        Node<Real>*pNode00 = &pGraph->nodes[id00];
        Node<Real>*pNode01 = &pGraph->nodes[id01];
        Node<Real>*pNode10 = &pGraph->nodes[id10];
        Node<Real>*pNode11 = &pGraph->nodes[id11];

        uint32_t flags00_01 = EF_None;
        uint32_t flags01_00 = EF_None;
        uint32_t flags10_11 = EF_None;
        uint32_t flags11_10 = EF_None;

        uint32_t newID = (uint32_t)pGraph->nodes.size();

        for (uint32_t i = 0; i < (uint32_t)pNode00->neighbours.size(); i++)
        {
          if (pNode00->neighbours[i].id == id01)
          {
            pNode00->neighbours[i].id = newID;
            flags00_01 = pNode00->neighbours[i].flags;
            break;
          }
        }

        for (uint32_t i = 0; i < (uint32_t)pNode01->neighbours.size(); i++)
        {
          if (pNode01->neighbours[i].id == id00)
          {
            pNode01->neighbours[i].id = newID;
            flags01_00 = pNode01->neighbours[i].flags;
            break;
          }
        }

        for (uint32_t i = 0; i < (uint32_t)pNode10->neighbours.size(); i++)
        {
          if (pNode10->neighbours[i].id == id11)
          {
            pNode10->neighbours[i].id = newID;
            flags10_11 = pNode10->neighbours[i].flags;
            break;
          }
        }

        for (uint32_t i = 0; i < (uint32_t)pNode11->neighbours.size(); i++)
        {
          if (pNode11->neighbours[i].id == id10)
          {
            pNode11->neighbours[i].id = newID;
            flags11_10 = pNode11->neighbours[i].flags;
            break;
          }
        }

        Node<Real> newNode;
        newNode.vertex = point;

        newNode.neighbours.push_back({ id00, flags01_00 });
        newNode.neighbours.push_back({ id01, flags00_01 });
        newNode.neighbours.push_back({ id10, flags11_10 });
        newNode.neighbours.push_back({ id11, flags10_11 });

        pGraph->nodes.push_back(newNode);
      }

      static void EraseNeighbour(Node<Real> *pNode, uint32_t neighbourID)
      {
        for (uint32_t i = 0; i < (uint32_t)pNode->neighbours.size();)
        {
          if (pNode->neighbours[i].id == neighbourID)
            pNode->neighbours.erase_swap(i);
          else
            i++;
        }
      }

      static void DoPointEdgeIntersection(Graph<Real> *pGraph, uint32_t nodeID, uint32_t edgeID0, uint32_t edgeID1)
      {
        Node<Real>*pNode = &pGraph->nodes[nodeID];
        Node<Real>*pEdge0 = &pGraph->nodes[edgeID0];
        Node<Real>*pEdge1 = &pGraph->nodes[edgeID1];

        uint32_t flags01 = EF_None;
        uint32_t flags10 = EF_None;

        for (uint32_t i = 0; i < (uint32_t)pEdge0->neighbours.size(); i++)
        {
          if (pEdge0->neighbours[i].id == edgeID1)
          {
            flags01 = pEdge0->neighbours[i].flags;
            break;
          }
        }

        for (uint32_t i = 0; i < (uint32_t)pEdge1->neighbours.size(); i++)
        {
          if (pEdge1->neighbours[i].id == edgeID0)
          {
            flags10 = pEdge1->neighbours[i].flags;
            break;
          }
        }

        EraseNeighbour(pEdge0, edgeID1);
        EraseNeighbour(pEdge0, nodeID);

        EraseNeighbour(pEdge1, edgeID0);
        EraseNeighbour(pEdge1, nodeID);

        pEdge0->neighbours.push_back({ nodeID, flags01 });
        pEdge1->neighbours.push_back({ nodeID, flags10 });
        pNode->neighbours.push_back({ edgeID0, flags10 });
        pNode->neighbours.push_back({ edgeID1, flags01 });
      }

      bool MergeNodeAndEdges(Graph<Real> *pGraph, Real epsilon)
      {
        bool hasIntersections = false;
        Dg::Set_AVL<uint64_t> testedEdges; // TODO Should be hash map
        for (uint32_t pointID = 0; pointID < (uint32_t)pGraph->nodes.size(); pointID++)
        {
          testedEdges.clear();
          Node<Real> *pPointNode = &pGraph->nodes[pointID];
          Vector2<Real> point = pPointNode->vertex;

          for (uint32_t edgeID0 = 0; edgeID0 < (uint32_t)pGraph->nodes.size(); edgeID0++)
          {
            if (pointID == edgeID0)
              continue;

            Node<Real> *pEdge0Node = &pGraph->nodes[edgeID0];

            for (uint32_t n = 0; n < (uint32_t)pEdge0Node->neighbours.size(); n++)
            {
              uint32_t edgeID1 = pEdge0Node->neighbours[n].id;

              if (pointID == edgeID1)
                continue;

              uint64_t edgeID = GetUndirectedEdgeID(edgeID0, edgeID1);

              if (testedEdges.exists(edgeID))
                continue;

              testedEdges.insert(edgeID);

              Node<Real> *pEdge1Node = &pGraph->nodes[edgeID1];
              Segment2<Real> seg(pEdge0Node->vertex, pEdge1Node->vertex);

              CP2PointSegment<Real> query;
              auto result = query(point, seg);

              if (Dg::MagSq(result.cp - point) < epsilon)
              {
                hasIntersections = true;
                pPointNode->vertex = result.cp;
                DoPointEdgeIntersection(pGraph, pointID, edgeID0, edgeID1);

                // We assume no other edges connected to the node will be close to the test point.
                // Of course, this may well be the case, but we will be encroaching on degenerate polygon
                // territory, something this algorithm does not support.
                break;
              }
            }
          }
        }

        return hasIntersections;
      }

      void EraseSwap(Graph<Real> *pGraph, uint32_t id)
      {
        pGraph->nodes.erase_swap(id);
        uint32_t oldID = (uint32_t)pGraph->nodes.size();
        Node<Real> *pNode = &pGraph->nodes[id];

        for (uint32_t i = 0; i < (uint32_t)pNode->neighbours.size(); i++)
        {
          Node<Real> *pOther = &pGraph->nodes[pNode->neighbours[i].id];

          for (uint32_t j = 0; j < (uint32_t)pOther->neighbours.size(); j++)
          {
            if (pOther->neighbours[j].id == oldID)
            {
              pOther->neighbours[j].id = id;
              break;
            }
          }
        }
      }

      bool MergeNodes(Graph<Real> *pGraph, Real epsilon)
      {
        bool hasIntersections = false;
        for (uint32_t i = 0; i < (uint32_t)pGraph->nodes.size(); i++)
        {
          auto vi = pGraph->nodes[i].vertex;

          // Look ahead in the map to find any nodes we should merge with the current node.
          for (uint32_t j = i + 1; j < (uint32_t)pGraph->nodes.size();)
          {
            auto vj = pGraph->nodes[j].vertex;
            if (Dg::MagSq(vi - vj) < epsilon)
            {
              UpdateNeighbours(pGraph, j, i);
              EraseSwap(pGraph, j);
              hasIntersections = true;
            }
            else
            {
              j++;
            }
          }
        }

        return hasIntersections;
      }

      static void SwapNeighbour(Node<Real> *pNode, uint32_t oldID, uint32_t newID)
      {
        uint32_t indexOfNew = NI_Invalid;
        uint32_t indexOfOld = NI_Invalid;

        for (uint32_t n = 0; n < (uint32_t)pNode->neighbours.size(); n++)
        {
          uint32_t id = pNode->neighbours[n].id;
          if (id == oldID)
            indexOfOld = n;
          else if (id == newID)
            indexOfNew = n;
        }

        if (indexOfOld == NI_Invalid)
          return;

        pNode->neighbours[indexOfOld].id = newID;

        // Merge the edge pNode points to both the old and new nodes.
        if (indexOfNew != NI_Invalid)
        {
          pNode->neighbours[indexOfOld].flags |= pNode->neighbours[indexOfNew].flags;
          pNode->neighbours.erase_swap(indexOfNew);
        }
      }

      void UpdateNeighbours(Graph<Real> *pGraph, uint32_t oldID, uint32_t newID)
      {
        Node<Real>*pOldNode = &pGraph->nodes[oldID];
        Node<Real>*pNewNode = &pGraph->nodes[newID];
        
        // Merge common edges from the old node
        for (size_t i = 0; i < pNewNode->neighbours.size(); i++)
        {
          uint32_t neighbour_i = pNewNode->neighbours[i].id;
          for (size_t j = 0; j < pOldNode->neighbours.size(); j++)
          {
            uint32_t neighbour_j = pOldNode->neighbours[j].id;

            if (neighbour_i == neighbour_j)
            {
              pNewNode->neighbours[i].flags |= pOldNode->neighbours[j].flags;
              break;
            }
          }
        }

        // Add unique neighbours
        for (size_t i = 0; i < pOldNode->neighbours.size(); i++)
        {
          uint32_t neighbour_i = pOldNode->neighbours[i].id;
          bool found = false;
          for (size_t j = 0; j < pNewNode->neighbours.size(); j++)
          {
            uint32_t neighbour_j = pNewNode->neighbours[j].id;

            if (neighbour_i == neighbour_j)
            {
              found = true;
              break;
            }
          }

          if (!found)
            pNewNode->neighbours.push_back(pOldNode->neighbours[i]);
        }

        // Swap all links to old, to now point to the new node.
        for (uint32_t i = 0; i < (uint32_t)pOldNode->neighbours.size(); i++)
        {
          uint32_t id = pOldNode->neighbours[i].id;
          if (id != newID)
            SwapNeighbour(&pGraph->nodes[id], oldID, newID);
        }
      }
    };
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::operator()
    (Polygon2<Real> const &polyA, Polygon2<Real> const &polyB)
  {
    impl_FI2PolygonPolygon::Graph<Real> graph;
    impl_FI2PolygonPolygon::PolygonsToGraph<Real> polysToGraph;
    impl_FI2PolygonPolygon::GraphBuilder<Real> builder;
    Result result;
    QueryCode code;

    code = polysToGraph.Execute(polyA, polyB, &graph);
    if (code != QueryCode::Success) goto epilogue;

    code = builder.Execute(&graph);

    if (code == QueryCode::NotIntersecting)
    {
      QueryCode code = QueryCode::NotIntersecting;
      TI2PointPolygon<Real> query;
      auto result = query(*polyA.cPointsBegin(), polyB);
      if (result.code != QueryCode::CompletelyOutside)
      {
        code = QueryCode::A_in_B;
      }
      else
      {
        result = query(*polyB.cPointsBegin(), polyA);
        if (result.code != QueryCode::CompletelyOutside)
          code = QueryCode::B_in_A;
      }
    }

    else if (code == QueryCode::Intersecting)
    {
      impl_FI2PolygonPolygon::PolygonExtractor<Real> extractor;
      extractor.Execute(&graph, &result);
    }

    epilogue:

    result.code = code;
    return result;
  }
}

#endif