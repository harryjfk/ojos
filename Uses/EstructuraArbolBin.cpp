//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EstructuraArbolBin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//    Demonios
//---------------------------------------------------------------------------
namespace Daemons
{
  bool AllNodesDegreeTwo(TBinTreeNode* pCursor, void* pReference)
  {
    return pCursor->Degree() == 1;
  }

  void CountNodes(TBinTreeNode* pCursor, void* pReference)
  {
    (*(int*)pReference)++;
  }

  void Destroy(TBinTreeNode* pCursor, void* pReference)
  {
    delete pCursor;
  }

  bool AreBrothers(TBinTreeNode* pCursor, void* pReference)
  {
    return (pCursor->Left() == ((TSFather*)pReference)->sNode && pCursor->Right() == ((TSFather*)pReference)->sFather);
  }

  bool FindFather(TBinTreeNode* pCursor, void* pReference)
  {
    if ((pCursor->Left() == ((TBinTreeNode*)pReference)) ||
        (pCursor->Right() == ((TBinTreeNode*)pReference)))
        return true;
      else
        return false;
  }

/*  bool FindNodeToDelete(TBinTreeNode* pCursor, void* pReference)
  {
    if ((pCursor->Left() &&
         pCursor->Left()== ((TSFather*)pReference)->sNode) ||
        (pCursor->Right() &&
         pCursor->Right()==((TSFather*)pReference)->sNode))
    {
      ((TSFather*)pReference)->sFather = pCursor;
      return true;
    }
    else
      return false;
  }*/

  void GetDegree(TBinTreeNode* pCursor, void* pReference)
  {
    if (pCursor->Degree() > *((int*)pReference))
      *((int*)pReference) = pCursor->Degree();
  }

  bool IncompletedNode(TBinTreeNode* pCursor, void* pReference)
  {
    return pCursor->Degree() < 2;
  }

  bool IsFather(TBinTreeNode* pCursor, void* pReference)
  {
    return pCursor == (TBinTreeNode*)pReference;
  }

  void Leaves(TBinTreeNode* pCursor, void* pReference)
  {
    if (!pCursor->Left() && !pCursor->Right())
    {
      TNode* newNode = new TNode(pCursor);
      ((TGLinkedList*)pReference)->Insert(newNode);
    }
  }

  void SubTree(TBinTreeNode* pCursor, void* pReference)
  {
    TNode* newNode = new TNode(pCursor);
    ((TSSubTree*)pReference)->sAncestors->Insert(newNode);
  }

  void Traversals(TBinTreeNode* pCursor, void* pReference)
  {
    TNode* newNode = new TNode(pCursor);
    ((TGLinkedList*)pReference)->Insert(newNode);
  }
} // Fin del namespace Demonios
//---------------------------------------------------------------------------
//    Implementaci�n del Nodo
//---------------------------------------------------------------------------
int TBinTreeNode::Degree()
{
  int degree = 0;
  if (aLeft)
      degree++;
  if (aRight)
      degree++;
  return degree;
}
//---------------------------------------------------------------------------
//    Implementaci�n del Arbol
//---------------------------------------------------------------------------
TBinTree::~TBinTree()
{
  PostOrderForEach(aRoot, Daemons::Destroy, NULL);
}
//---------------------------------------------------------------------------
bool TBinTree::AllNodesDegreeTwo()
{
  return !FirstThat(aRoot, Daemons::AllNodesDegreeTwo,NULL);
}
//---------------------------------------------------------------------------
void* TBinTree::DeleteNode(TBinTreeNode* pNode)
//Si tuviera hijos, hay que dejarlos conectado en el �rbol. Si tuviera
//un solo hijo, este ocupa su lugar. Si tuviera dos hijos, lo mejor
//ser�a que el nodo a eliminar no tuviera hermanos y as� se colocan
//los dos hijos en el padre del padre (si no es la raiz). En caso
//contrario, el hijo izquierdo ocupa su lugar y el derecho se coloca
//en el primer nodo que no tenga dos hijos.
{
  if (pNode)
  {
    if (aRoot == pNode) // Se va a eliminar la raiz
      DeleteRoot(aRoot, aRoot->Degree());
    else               // El nodo a eliminar no es ra�z
    {
      TBinTreeNode* father = GetFather(pNode);
      DeleteNotRoot(pNode, pNode->Degree(), father);
    }
    return pNode->Info();
  }
  else
    return NULL;
}
//---------------------------------------------------------------------------
void TBinTree::DeleteNotRoot(TBinTreeNode* pNode, int pDegree,
                             TBinTreeNode* pFather)
{
  switch (pDegree)
  {
    case 0:
     (pFather->Left()== pNode)? pFather->Left(NULL): pFather->Right(NULL);
      break;
    case 1:
      if (pNode->Left())     // El hijo es izquierdo
        (pFather->Left()== pNode)? pFather->Left(pNode->Left()) : pFather->Right(pNode->Left());
      else                   // El hijo es derecho
        (pFather->Left()== pNode)? pFather->Left(pNode->Right()): pFather->Right(pNode->Right());
      break;
    case 2:
      if (pFather->Degree() != 2)
      {
        pFather->Left(pNode->Left());
        pFather->Right(pNode->Right());
      }
      else  // El hijo izq. lo sustituye y se ubica al derecho
      {
        (pFather->Left()== pNode)? pFather->Left(pNode->Left()): pFather->Right(pNode->Left());
        //Ubicaci�n del hijo derecho del nodo a eliminar
        TBinTreeNode* incompletedNode;
        if (AllNodesDegreeTwo())
        {
          incompletedNode =  FirstIncompletedNode(aRoot);
          // Nunca va a coincidir con el nodo que se va a mover
          // pues se mueve el de la derecha y se busca en preorden.
          if (incompletedNode->Left())
            incompletedNode->Right(pNode->Right());
          else
            incompletedNode->Left(pNode->Right());
        }
        else
        {
          incompletedNode = FirstIncompletedNode(pNode->Left());
          if (incompletedNode->Left())
            incompletedNode->Right(pNode->Right());
          else
            incompletedNode->Left(pNode->Right());
        }
      }
      break;
  }
}
//---------------------------------------------------------------------------
void TBinTree::DeleteRoot(TBinTreeNode* pRoot, int pDegree)
{
  switch (pDegree)
  {
    case 0:
      aRoot = NULL;
      break;
    case 1:  // El hijo pasa a ser la ra�z
      Root(pRoot->Left()? pRoot->Left() : pRoot->Right());
      break;
    case 2:  // El hijo izq. es la ra�z y el derecho se ubica en el primer
             // nodo
      //Ubicaci�n del hijo derecho del nodo a eliminar
      TBinTreeNode* incompletedNode = FirstIncompletedNode(aRoot);

      if (incompletedNode->Left())
        incompletedNode->Right(pRoot->Right());
      else
        incompletedNode->Left(pRoot->Right());
      Root(pRoot->Left());
      break;
  }
}
//---------------------------------------------------------------------------
bool TBinTree::DivideTree(TBinTreeNode* pNode, TBinTree* &pTreeA, TBinTree* &pTreeB)
//Dividir el �rbol en dos sub�rboles a partir del nodo de informaci�n
//X. Uno de los sub�rboles ser� a partir del nodo de informaci�n X, que
//ser� la ra�z y el resto de los nodos del �rbol pertenecer�n al otro
//sub�rbol.
{
  bool divided = true;
  if (pNode)
  {
    if (pNode == Root())
    {
      GetNodeSubTree(aRoot, NULL, pTreeA);
      pTreeB->Root(NULL);
    }
    else
    {
      GetNodeSubTree(aRoot, pNode, pTreeA);
      GetNodeSubTree(pNode, NULL, pTreeB);
    }
  }
  else
  {
    pTreeA->Root(NULL);
    pTreeB->Root(NULL);
    divided = false;
  }
  return divided;
}
//---------------------------------------------------------------------------
TBinTreeNode* TBinTree::GetFather(TBinTreeNode* pNode)
{
  if (pNode == Root())
    return NULL;
  else
    return FirstThat(aRoot, Daemons::FindFather, pNode);
}
//---------------------------------------------------------------------------
TBinTreeNode* TBinTree::FirstIncompletedNode(TBinTreeNode* pCursor)
{
  return FirstThat(pCursor, Daemons::IncompletedNode, NULL);
}
//---------------------------------------------------------------------------
TGLinkedList* TBinTree::GetLeaves()
{
  TGLinkedList* leavesList = new TGLinkedList();
  PreOrderForEach(aRoot, Daemons::Leaves, leavesList);
  return leavesList;
}
//---------------------------------------------------------------------------
void TBinTree::GetNodeSubTree(TBinTreeNode* pRoot, TBinTreeNode* pNode, TBinTree* &pTree)
{
  if ((pRoot) && (pRoot != pNode))
  {
    TBinTreeNode* cursor = new TBinTreeNode(pRoot->Info());
    if (pRoot->Left() && pRoot->Left() != pNode)
    {
      GetNodeSubTree(pRoot->Left(), pNode, pTree);
      cursor->Left(pTree->Root());
    }
    else
      cursor->Left(NULL);

    if (pRoot->Right() && pRoot->Right() != pNode)
    {
      GetNodeSubTree(pRoot->Right(), pNode, pTree);
      cursor->Right(pTree->Root());
    }
    else
      cursor->Right(NULL);

    pTree->Root(cursor);
  }
}
//---------------------------------------------------------------------------
TSSons* TBinTree::GetSons(TBinTreeNode* pNode)
{
  TSSons* sSons = new TSSons;
  if (pNode)
  {
    if (pNode->Left())
      sSons->sLeft = pNode->Left();
    else
      sSons->sLeft = NULL;
    if (pNode->Right())
      sSons->sRight = pNode->Right();
    else
      sSons->sRight = NULL;
  }
  return sSons;
}
//---------------------------------------------------------------------------
TGLinkedList* TBinTree::GetSubTree(TBinTreeNode* pNode, char pType)
{
  if (pNode)
  {
    TSSubTree* sSubTree = new TSSubTree;
    sSubTree->sLocation = pNode;
    sSubTree->sAncestors = new TGLinkedList();
    if (pType == 'L')
    {
      if (pNode->Left())
        PostOrderForEach(pNode->Left(), Daemons::SubTree, sSubTree);
    }
    else
      if (pNode->Right())
        PostOrderForEach(pNode->Right(), Daemons::SubTree, sSubTree);
    return sSubTree->sAncestors;
  }
  else
    return NULL;
}
//---------------------------------------------------------------------------
bool TBinTree::InsertNode(TBinTreeNode* pNode, char pType, TBinTreeNode* pFather)
{
  bool inserted = false;
  TBinTreeNode* father;
  if (pNode)
  {
    switch (pType)
    {
      case 'R':
        if (Empty())
          Root(pNode);
        else
        {
          pNode->Left(Root());
          Root(pNode);
        }
        inserted = true;
        break;
      case 'L':
      case 'G':
        TBinTreeNode* existFather = FirstThat(aRoot, Daemons::IsFather, pFather);
        if (existFather)
        {
          if (pType == 'L')
          {
            pNode->Left(pFather->Left());
            pFather->Left(pNode);
          }
          else
          {
            pNode->Right(pFather->Right());
            pFather->Right(pNode);
          }
          inserted = true;
        }
        break;
    }
  }
  return inserted;
}
//---------------------------------------------------------------------------
int TBinTree::Level(TBinTreeNode* pCursor)
{
  if (pCursor)
  {
    int pLevelLST = Level(pCursor->Left());
    int pLevelRST = Level(pCursor->Right());
    return ((pLevelLST >= pLevelRST? pLevelLST : pLevelRST)+1);
  }
  else
    return -1;
}
//---------------------------------------------------------------------------
int TBinTree::NodeLevel(TBinTreeNode* pNode)
{
  int level = 0;
  if (pNode != aRoot)
  {
    TBinTreeNode* father = pNode;
    while (father != aRoot)
    {
      father = GetFather(father);
      level++;
    }
  }
  return level;
}
//---------------------------------------------------------------------------
void TBinTree::PostOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    TBinTree::PostOrderForEach(pCursor->Left(), pAction, pReference);
    TBinTree::PostOrderForEach(pCursor->Right(), pAction, pReference);
    pAction(pCursor, pReference);
   }
}
//---------------------------------------------------------------------------
void TBinTree::InvPostOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    pAction(pCursor, pReference);
    TBinTree::InvPostOrderForEach(pCursor->Right(), pAction, pReference);
    TBinTree::InvPostOrderForEach(pCursor->Left(), pAction, pReference);
   }
}
//---------------------------------------------------------------------------
TBinTreeNode* TBinTree::FirstThat(TBinTreeNode* pCursor, TConditionalFunction pCondition, void* pReference)
{
  if (pCursor)
  {
    if (pCondition(pCursor, pReference))
      return (pCursor)?pCursor:NULL;
    else
    {
      TBinTreeNode* node = TBinTree::FirstThat(pCursor->Left(), pCondition, pReference);
      if (!node)
        node = TBinTree::FirstThat(pCursor->Right(), pCondition, pReference);
      return node;  
    }
  }
  else
    return NULL;
}
//---------------------------------------------------------------------------
void TBinTree::PreOrderForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    pAction(pCursor, pReference);
    TBinTree::PreOrderForEach(pCursor->Left(), pAction, pReference);
    TBinTree::PreOrderForEach(pCursor->Right(), pAction, pReference);
  }
}
//---------------------------------------------------------------------------
void TBinTree::SymmetricForEach(TBinTreeNode* pCursor, TActionFunction pAction, void* pReference)
{
  if (pCursor)
  {
    TBinTree::SymmetricForEach(pCursor->Left(), pAction, pReference);
    pAction(pCursor, pReference);
    TBinTree::SymmetricForEach(pCursor->Right(), pAction, pReference);
  }
}
//---------------------------------------------------------------------------
int TBinTree::TotalNodes()
{
  int count = 0;
  PreOrderForEach(aRoot, Daemons::CountNodes, &count);
  return count;
}
//---------------------------------------------------------------------------
int TBinTree::TreeDegree()
{
  int degree = -1;
  if (!Empty())
    PreOrderForEach(aRoot, Daemons::GetDegree, &degree);
  return degree;
}
//---------------------------------------------------------------------------
int TBinTree::TreeLevel()
{
  return Level(aRoot);
}
//---------------------------------------------------------------------------

