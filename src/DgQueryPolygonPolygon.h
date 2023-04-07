//@group Math/Geometry/Query

#ifndef DGQUERYPOLYGONPOLYGON_H
#define DGQUERYPOLYGONPOLYGON_H

#include "DgQuery.h"
#include "DgVector.h"
#include "DgPolygon.h"
#include "DgQueryPointPolygon.h"
#include "DgQuerySegmentSegment.h"
#include "DgDynamicArray.h"
#include "DgMap_AVL.h"

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
      Polygon2<Real> boundary;
      Dg::DynamicArray<Polygon2<Real>> polyA;
      Dg::DynamicArray<Polygon2<Real>> polyB;
      Dg::DynamicArray<Polygon2<Real>> intersection;
      Dg::DynamicArray<Polygon2<Real>> holes;
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

  namespace _FI2PolygonPolygon
  {
    enum EdgeFlag : uint32_t
    {
      EF_InsideA = 1 << 0,
      EF_InsideB  = 1 << 1,

      EF_InsideBoth = EF_InsideA | EF_InsideB,
    };

    class EdgeValue
    {
    public:

      EdgeValue()
        : m_edgeflag(0)
      {

      }

      void Add(EdgeFlag flag)
      {
        m_edgeflag |= flag;
      }

      operator EdgeFlag() 
      { 
        return (EdgeFlag)m_edgeflag; 
      }

    private:

      uint32_t m_edgeflag;
    };

    typedef uint32_t NodeID;
    typedef uint64_t EdgeID;

    template<typename Real>
    struct Node
    {
      Vector2<Real> vertex;
      Dg::DynamicArray<NodeID> neighbours;
    };

    template<typename Real>
    struct Graph
    {
      Dg::Map_AVL<NodeID, Node<Real>> nodes;
      Dg::Map_AVL<EdgeID, EdgeValue> edgeMap;
    };
    
    template<typename Real>
    using cNodeIt = Dg::Map_AVL<NodeID, Node<Real>>::const_iterator;

    template<typename Real>
    using NodeIt = Dg::Map_AVL<NodeID, Node<Real>>::iterator;

    inline EdgeID GetEdgeID(NodeID a, NodeID b)
    {
      return (((EdgeID)a) << 32) | ((EdgeID)b);
    }

    template<typename Real>
    class GraphBuilder
    {
    public:

      QueryCode Build(Polygon2<Real> const &polyA, Polygon2<Real> const &polyB, Graph<Real> *pGraph, Real epsilon = Constants<Real>::EPSILON)
      {
        if (pGraph == nullptr)
          return QueryCode::Fail;

        m_currentID = 0;

        pGraph->edgeMap.clear();
        pGraph->nodes.clear();

        AddPolygon(pGraph, polyA, EF_InsideA);
        AddPolygon(pGraph, polyB, EF_InsideB);

        bool hasIntersections = false;

        hasIntersections = hasIntersections || MergeNodes(pGraph, epsilon);
        hasIntersections = hasIntersections || MergeNodeAndEdges(pGraph, epsilon);
        hasIntersections = hasIntersections || FindIntersections(pGraph, epsilon);

      }

    private:

      void AddPolygon(Graph<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
      {
        uint32_t count = (uint32_t)polygon.Size();
        uint32_t startID = m_currentID;
        uint32_t currentIndex = 0;

        for (auto it = polygon.cPointsBegin(); it != polygon.cPointsEnd(); it++)
        {
          uint32_t previousIndex = (currentIndex + count - 1) % count;
          uint32_t nextIndex = (currentIndex + 1) % count;

          NodeID currentID = startID + currentIndex;
          NodeID previousID = startID + previousIndex;
          NodeID nextID = startID + nextIndex;

          Node<Real> node;
          node.vertex = *it;
          node.neighbours.push_back(previousID);
          node.neighbours.push_back(nextID);

          pGraph->nodes.insert();
          pGraph->edgeMap.insert(GetEdgeID(currentID, nextID), flag);

          currentIndex++;
        }

        m_currentID += count;
      }

      bool FindIntersections(Graph<Real> *pGraph, Real epsilon)
      {

      }

      bool MergeNodeAndEdges(Graph<Real> *pGraph, Real epsilon)
      {

      }

      bool MergeNodes(Graph<Real> *pGraph, Real epsilon)
      {
        bool hasIntersections = false;
        auto it = pGraph->nodes.cbegin();
        while (it != pGraph->nodes.cend())
        {
          NodeID nodeID = it->first;
          Vector2<Real> point = it->second.vertex;
          it++;

          // Look ahead in the map to find any nodes we should merge with the current node.
          for (; it != pGraph->nodes.cend();)
          {
            auto v = it->second.vertex;
            if (Dg::MagSq(v - point) < epsilon)
            {
              UpdateNeighbours(pGraph, it->first, nodeID);
              it = pGraph->nodes.erase(it);
              hasIntersections = true;
            }
            else
            {
              it++;
            }
          }

          // We need to find the original node and move to the next.
          it = pGraph->nodes.find(nodeID);
          it++;
        }

        return hasIntersections;
      }

      void UpdateNeighbours(Graph<Real> *pGraph, NodeIt itOld, NodeIt itNew)
      {
        NodeID oldID = itOld->first;
        NodeID newID = itNew->first;

        // First, erase the oldID from the neighbour list of newID if it exists.
        for (size_t i = itNew->second.neighbours.Size(); i > 0; i--)
        {
          NodeID neighbourID = itNew->second.neighbours[i - 1];
          if (neighbourID == oldID)
          {
            pGraph->edgeMap.erase(GetEdgeID(oldID, newID));
            pGraph->edgeMap.erase(GetEdgeID(newID, oldID));
            itNew->second.neighbours.erase_swap(i);
            break;
          }
        }

        // Swap all instances of oldID to newID
        for (auto it = pGraph->nodes.cbegin_rand(); pGraph->nodes.cend_rand(); it++)
        {
          for (size_t i = 0; i < it->second.neighbours.Size(); i++)
          {
            if (it->second.neighbours[i] != oldID)
              continue;

            it->second.neighbours[i] = newID;

            // Update the edge map
            EdgeID id0 = GetEdgeID(it->first, oldID);
            EdgeID id1 = GetEdgeID(oldID, it->first);

            auto eit = pGraph->edgeMap.find(id0);
            if (eit != pGraph->edgeMap.end())
            {
              EdgeValue value = eit->second;
              pGraph->edgeMap.erase(id0);
              id0 = GetEdgeID(it->first, newID);
              pGraph->edgeMap.insert(id0, value);
            }

            eit = pGraph->edgeMap.find(id1);
            if (eit != pGraph->edgeMap.end())
            {
              EdgeValue value = eit->second;
              pGraph->edgeMap.erase(id1);
              id1 = GetEdgeID(newID, it->first);
              pGraph->edgeMap.insert(id1, value);
            }

            break; // We don't need to continue;
          }
        }

        // Merge edges

        // Find common neighbours
        for (size_t i = 0; i < itNew->second.neighbours.size(); i++)
        {
          NodeID neighbourNew = itNew->second.neighbours[i];
          for (size_t j = 0; j < itOld->second.neighbours.size(); j++)
          {
            NodeID neighbourOld = itOld->second.neighbours[j];
            if (neighbourOld != neighbourNew)
              continue;

            EdgeID id = GetEdgeID(oldID, neighbourOld);
            auto it = pGraph->edgeMap.find(id);
            if (it != pGraph->nodes.end())
            {
              pGraph->edgeMap[GetEdgeID(newID, neighbourOld)].Add(it->second);
              continue;
            }

            id = GetEdgeID(neighbourOld, oldID);
            auto it = pGraph->edgeMap.find(id);
            if (it != pGraph->nodes.end())
              pGraph->edgeMap[GetEdgeID(neighbourOld, newID)].Add(it->second);
          }
        }
      }

    private:

      NodeID m_currentID;
    };
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::operator()
    (Polygon2<Real>  const &polyA, Polygon2<Real> const &polyB)
  {
    _FI2PolygonPolygon::Graph<Real> graph;
    _FI2PolygonPolygon::GraphBuilder<Real> builder;
    Result result;

    QueryCode code = builder.Build(polyA, polyB, &graph);

    if (code == QueryCode::Fail)
    {
      result.code = QueryCode::Fail;
    }

    else if (code == QueryCode::NotIntersecting)
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

      result.code = code;
    }

    else
    {

    }

    return result;
  }
}

#endif