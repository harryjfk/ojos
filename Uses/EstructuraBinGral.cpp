//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EstructuraBinGral.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)




#include "EstructuraBinGral.h"

namespace GeneralTreeDaemons
{
  bool IsThere(TBinTreeNode* pCursor, void* pReference)
  {
     return pCursor == (TBinTreeNode*)pReference;
  }

  void Leaves(TBinTreeNode* pCursor, void* pReference)
  {
    if (!pCursor->Left())
    {
      ((TGLinkedList*)pReference)->Insert(pCursor);
    }
  }

  bool Prev(TBinTreeNode* pCursor, void* pReference)
  {
    return ((pCursor->Left() == pReference) || (pCursor->Right() == pReference));
  }
} // Fin Demonios

//----------------------------------
// Implementación de TNodoBinGral
//----------------------------------
int TGBinTreeNode::Degree()
{
  int degree = 0;
  TBinTreeNode* cursor = Left();
  while (cursor)
  {
     degree++;
     cursor = cursor->Right();
  }
  return degree;
}
//----------------------------------
// Implementación de TArbolBinGral
//----------------------------------
void* TGBinTree::DeleteNode(TGBinTreeNode* pNode)
// Se busca el nodo que lo apunta. Su lugar lo ocupa el nodo que es apuntado por la derecha.
// Si no existe ninguno, se trata de la raíz.
{
  TBinTreeNode* prev = FirstThat(aRoot, GeneralTreeDaemons::Prev, pNode);
  if (prev)
  {
    if (prev->Left() == pNode)
      prev->Left(pNode->Right());
    else
      prev->Right(pNode->Right());
  }
  else  // Es la raíz
    Root(pNode->Right());
  return pNode->Info();
}
//----------------------------------
TBinTreeNode* TGBinTree::GetFather(TBinTreeNode* pNode)
{
  if (Empty() || aRoot == pNode)
     return NULL;
  else
  {
     TBinTreeNode* prev = FirstThat(aRoot, GeneralTreeDaemons::Prev, pNode);
     if (prev->Right() == pNode)
       prev = GetFather(prev);
     return prev;
  }
}
//----------------------------------
TGLinkedList* TGBinTree::GetLeaves()
{
  if (!Empty())
  {
     TGLinkedList* leavesList = new TGLinkedList();
     PreOrderForEach(aRoot, GeneralTreeDaemons::Leaves, leavesList);
     return leavesList;
  }
  else
    return NULL;
}
//----------------------------------
TGLinkedList* TGBinTree::GetSons(TBinTreeNode* pNode)
{
  if (pNode->Left())
  {
    TGLinkedList* sonsList = new TGLinkedList();
    sonsList->Insert(pNode->Left()->Info());
    if (pNode->Left()->Right())
    {
      pNode = pNode->Left();
      while (pNode->Right())
      {
        sonsList->Insert(pNode->Right()->Info());
        pNode = pNode->Right();
      }
    }
    return sonsList;
  }
  else
    return NULL;
}
//----------------------------------
bool TGBinTree::InsertNode(TGBinTreeNode* pNode, TGBinTreeNode* pFather)
{
  if  (Empty())
  {
    if (!pFather)
    {
       aRoot = pNode;
       return true;
    }
    else
      return false;
  }
  else
  {
     if (pFather)
     {
       TBinTreeNode* isFather = FirstThat(aRoot, GeneralTreeDaemons::IsThere, pFather);
       if (isFather)
       {
          TBinTreeNode* cursor = pFather->Left();
          if (!cursor)
             pFather->Left(pNode);
          else
          {
             while (cursor->Right())
               cursor = cursor->Right();
             cursor->Right(pNode);
          }
       }
       return isFather;
     }
     else
     { // Insertarlo como último hermano de la raíz
       if (aRoot->Right())
       {
         TBinTreeNode* cursor = aRoot->Right();
         while (cursor->Right())
           cursor = cursor->Right();
         cursor->Right(pNode);
       }
       else
         aRoot->Right(pNode);
       return true;
     }
   }
}
//----------------------------------
int TGBinTree::Level(TBinTreeNode* pCursor)
{
  if (pCursor)
  {
    int pLevelLST = Level(pCursor->Left());
    int pLevelRST = Level(pCursor->Right());
    return ( pLevelRST > pLevelLST? pLevelRST: pLevelLST + 1);
  }
  else
    return -1;
}
//----------------------------------

void TGBinTree::PostOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    TBinTree::PostOrderForEach(pCursor->Left(), pAction, pReference);
    if (pCursor == aRoot) //se hace para un nodo distinto de la raiz empezar de ahi "pa bajo" sin incluirlo a él
      pAction(pCursor, pReference);
  }
}

//----------------------------------

void TGBinTree::InvPostOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    pAction(pCursor, pReference);
    TBinTree::InvPostOrderForEach(pCursor->Left(), pAction, pReference);
  }
}

//----------------------------------

TBinTreeNode* TGBinTree::FirstThat(TBinTreeNode* pCursor, TConditionalFunction pCondition, void* pReference)
{
  if (pCursor != aRoot) //se hace para un nodo distinto de la raiz empezar de ahi "pa bajo" sin incluirlo a él
  {
    if (pCursor)
        return TBinTree::FirstThat(pCursor->Left(), pCondition, pReference);
    else
      return NULL;
  }
  else
  {
    if (pCursor)
    {
      if (pCondition(pCursor, pReference))
        return pCursor;
      else
        return TBinTree::FirstThat(pCursor->Left(), pCondition, pReference);
    }
    else
      return NULL;
  }
}

//----------------------------------

void TGBinTree::PreOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    if (pCursor == aRoot) // si el recorrido se hace para un nodo distinto de la raiz empezar de ahi "pa bajo" sin incluirlo a él
      pAction(pCursor, pReference);
    TBinTree::PreOrderForEach(pCursor->Left(), pAction, pReference);
  }
}

//----------------------------------



