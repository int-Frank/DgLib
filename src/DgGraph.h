//@group Math

#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>

#include "DgVector.h"
#include "DgQuerySegmentSegment.h"
#include "DgQueryPointSegment.h"
#include "DgDynamicArray.h"
#include "DgSet_AVL.h"

namespace Dg
{
  namespace Graph
  {
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

    enum NodeIndex : uint32_t
    {
      NI_Invalid = 0xFFFFFFFF
    };

    struct Neighbour
    {
      uint32_t id;
      uint32_t flags;
    };

    template<typename Real>
    struct Node
    {
      Vector2<Real> vertex;
      Dg::DynamicArray<Neighbour> neighbours;
    };

    template<typename Real>
    struct Graph_t
    {
      Dg::DynamicArray<Node<Real>> nodes;
    };

    //! Finds all intersections within a graph.
    template<typename Real>
    class GraphBuilder
    {
    public:

      QueryCode Execute(Graph_t<Real> *pGraph, Real epsilon = Constants<Real>::EPSILON)
      {
        int hasIntersections = 0;

        hasIntersections += MergeNodes(pGraph, epsilon) ? 1 : 0;
        hasIntersections += MergeNodeAndEdges(pGraph, epsilon) ? 1 : 0;
        hasIntersections += FindIntersections(pGraph, epsilon) ? 1 : 0;

        return hasIntersections > 0 ? QueryCode::Intersecting : QueryCode::NotIntersecting;
      }

    private:

      static bool FindIntersections(Graph_t<Real> *pGraph, Real epsilon)
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

      static bool ProcessNodePair(Graph_t<Real> *pGraph, uint32_t id00, uint32_t id10, Set_AVL<EdgePair, EdgePairLess> *pTestedEdgePairs)
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

      static void DoEdgeEdgeIntersection(Graph_t<Real> *pGraph, uint32_t id00, uint32_t id01, uint32_t id10, uint32_t id11, Vector2<Real> const &point)
      {
        Node<Real> *pNode00 = &pGraph->nodes[id00];
        Node<Real> *pNode01 = &pGraph->nodes[id01];
        Node<Real> *pNode10 = &pGraph->nodes[id10];
        Node<Real> *pNode11 = &pGraph->nodes[id11];

        uint32_t flags00_01 = EraseNeighbour(pNode00, id01);
        uint32_t flags01_00 = EraseNeighbour(pNode01, id00);
        uint32_t flags10_11 = EraseNeighbour(pNode10, id11);
        uint32_t flags11_10 = EraseNeighbour(pNode11, id10);

        uint32_t newID = (uint32_t)pGraph->nodes.size();

        Node<Real> newNode;
        newNode.vertex = point;

        pNode00->neighbours.push_back({ newID, flags00_01 });
        pNode01->neighbours.push_back({ newID, flags01_00 });
        pNode10->neighbours.push_back({ newID, flags10_11 });
        pNode11->neighbours.push_back({ newID, flags11_10 });

        newNode.neighbours.push_back({ id00, flags01_00 });
        newNode.neighbours.push_back({ id01, flags00_01 });
        newNode.neighbours.push_back({ id10, flags11_10 });
        newNode.neighbours.push_back({ id11, flags10_11 });

        pGraph->nodes.push_back(newNode);
      }

      static uint32_t EraseNeighbour(Node<Real> *pNode, uint32_t neighbourID)
      {
        uint32_t flag = 0;
        for (uint32_t i = 0; i < (uint32_t)pNode->neighbours.size();)
        {
          if (pNode->neighbours[i].id == neighbourID)
          {
            flag = pNode->neighbours[i].flags;
            pNode->neighbours.erase_swap(i);
          }
          else
          {
            i++;
          }
        }
        return flag;
      }

      static void DoPointEdgeIntersection(Graph_t<Real> *pGraph, uint32_t nodeID, uint32_t edgeID0, uint32_t edgeID1)
      {
        Node<Real> *pNode = &pGraph->nodes[nodeID];
        Node<Real> *pEdge0 = &pGraph->nodes[edgeID0];
        Node<Real> *pEdge1 = &pGraph->nodes[edgeID1];

        uint32_t flags01 = EraseNeighbour(pEdge0, edgeID1);
        uint32_t flags10 = EraseNeighbour(pEdge1, edgeID0);

        EraseNeighbour(pEdge0, nodeID);
        EraseNeighbour(pEdge1, nodeID);

        pEdge0->neighbours.push_back({ nodeID, flags01 });
        pEdge1->neighbours.push_back({ nodeID, flags10 });
        pNode->neighbours.push_back({ edgeID0, flags10 });
        pNode->neighbours.push_back({ edgeID1, flags01 });
      }

      static bool MergeNodeAndEdges(Graph_t<Real> *pGraph, Real epsilon)
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

      static void EraseSwap(Graph_t<Real> *pGraph, uint32_t id)
      {
        pGraph->nodes.erase_swap(id);

        if (id == pGraph->nodes.size())
          return;

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

      static bool MergeNodes(Graph_t<Real> *pGraph, Real epsilon)
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

      static void UpdateNeighbours(Graph_t<Real> *pGraph, uint32_t oldID, uint32_t newID)
      {
        Node<Real> *pOldNode = &pGraph->nodes[oldID];
        Node<Real> *pNewNode = &pGraph->nodes[newID];

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
}

#endif