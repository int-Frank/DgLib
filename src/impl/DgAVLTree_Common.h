#ifndef DGAVLTREE_COMMON_H
#define DGAVLTREE_COMMON_H

namespace Dg
{
  namespace impl
  {
    template<typename ValueType>
    bool Less(ValueType const & t0, ValueType const & t1)
    {
      return t0 < t1;
    }

    template<typename ValueType>
    ValueType Max(ValueType a, ValueType b)
    {
      return a > b ? a : b;
    }

    template<typename ValueType>
    struct Node
    {
      Node *       pParent;
      Node *       pLeft;
      Node *       pRight;
      int          height;
      ValueType    data;
    };

    template<typename ValueType>
    Node<ValueType> * GetNext(Node<ValueType> const * a_pNode)
    {
      //Try right
      if (a_pNode->pRight != nullptr)
      {
        //If right, take right, then take left all the way you can, then return.
        a_pNode = a_pNode->pRight;
        while (a_pNode->pLeft != nullptr)
          a_pNode = a_pNode->pLeft;
        return const_cast<Node<ValueType>*>(a_pNode);
      }

      //If no right, go up
      Node<ValueType> const * pOldNode;
      do
      {
        pOldNode = a_pNode;
        a_pNode = a_pNode->pParent;
      } while (pOldNode == a_pNode->pRight);
      return const_cast<Node<ValueType>*>(a_pNode);
    }

    template<typename ValueType>
    Node<ValueType> * GetPrevious(Node<ValueType> const * a_pNode)
    {
      //Try left
      if (a_pNode->pLeft != nullptr)
      {
        //If left, take left, then take right all the way you can, then return.
        a_pNode = a_pNode->pLeft;
        while (a_pNode->pRight != nullptr)
          a_pNode = a_pNode->pRight;
        return const_cast<Node<ValueType>*>(a_pNode);
      }

      //If no left, go up
      Node<ValueType> const * pOldNode;
      do
      {
        pOldNode = a_pNode;
        a_pNode = a_pNode->pParent;
      } while (pOldNode == a_pNode->pLeft);
      return const_cast<Node<ValueType>*>(a_pNode);
    }
  }
}

#endif