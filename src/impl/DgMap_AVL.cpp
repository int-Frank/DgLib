#include "../DgMap_AVL.h"

namespace Dg
{
  namespace impl
  {
    Node * GetNext(Node const * a_pNode)
    {
      //Try right
      if (a_pNode->pRight != nullptr)
      {
        //If right, take right, then take left all the way you can, then return.
        a_pNode = a_pNode->pRight;
        while (a_pNode->pLeft != nullptr)
          a_pNode = a_pNode->pLeft;
        return const_cast<Node*>(a_pNode);
      }

      //If no right, go up
      Node const * pOldNode;
      do
      {
        pOldNode = a_pNode;
        a_pNode = a_pNode->pParent;
      } while (pOldNode == a_pNode->pRight);
      return const_cast<Node*>(a_pNode);
    }

    Node * GetPrevious(Node const * a_pNode)
    {
      //Try left
      if (a_pNode->pLeft != nullptr)
      {
        //If left, take left, then take left all the way you can, then return.
        a_pNode = a_pNode->pLeft;
        while (a_pNode->pRight != nullptr)
          a_pNode = a_pNode->pRight;
        return const_cast<Node*>(a_pNode);
      }

      //If no left, go up
      Node const * pOldNode;
      do
      {
        pOldNode = a_pNode;
        a_pNode = a_pNode->pParent;
      } while (pOldNode == a_pNode->pLeft);
      return const_cast<Node*>(a_pNode);
    }
  }
}