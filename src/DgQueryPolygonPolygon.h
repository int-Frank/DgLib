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
#include "DgGraph.h"

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
    using namespace Graph;

    enum EdgeFlag : uint32_t
    {
      EF_None     = 0,
      EF_InsideA  = 1 << 0,
      EF_InsideB  = 1 << 1,

      EF_InsideBoth = EF_InsideA | EF_InsideB,
    };

    //! Given a valid graph, this class will extract each sub-polygon.
    template<typename Real>
    class PolygonExtractor
    {
    public:

      void Execute(Graph_t<Real> const *pGraph, typename FI2PolygonPolygon<Real>::Result *pResult)
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

      static void ExtractBoundary(Graph_t<Real> const *pGraph,
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

      static void _ExtractPolygon(Graph_t<Real> const *pGraph,
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

      static void ExtractPolygon(Graph_t<Real> const *pGraph,
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
      static uint32_t TurnLeft(Graph_t<Real> const *pGraph, uint32_t prevID, Node<Real> const *pCurrent, Vector2<Real> const &vPrevCurrent)
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

    //! This helper class to build a mostly unconnected graph from two polygons.
    //! Runs the graph through the GraphBuilder to build a valid graph.
    template<typename Real>
    class PolygonsToGraph
    {
    public:

      QueryCode Execute(Polygon2<Real> const &polyA, Polygon2<Real> const &polyB, Graph_t<Real> *pGraph, Real epsilon = Constants<Real>::EPSILON)
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

      static bool TryAddPolygon(Graph_t<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
      {
        Orientation winding = polygon.Winding();
        if (winding == Orientation::CW)
          return TryAddCWPolygon(pGraph, polygon, flag);
        else if (winding == Orientation::CCW)
          return TryAddCCWPolygon(pGraph, polygon, flag);
        return false;
      }

      static bool TryAddCWPolygon(Graph_t<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
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

      static bool TryAddCCWPolygon(Graph_t<Real> *pGraph, Polygon2<Real> const &polygon, EdgeFlag flag)
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
  }

  template<typename Real>
  typename Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::Result
    Query<QueryType::FindIntersection, Real, 2, Polygon2<Real>, Polygon2<Real>>::operator()
    (Polygon2<Real> const &polyA, Polygon2<Real> const &polyB)
  {
    Graph::Graph_t<Real> graph;
    Graph::GraphBuilder<Real> builder;
    impl_FI2PolygonPolygon::PolygonsToGraph<Real> polysToGraph;
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