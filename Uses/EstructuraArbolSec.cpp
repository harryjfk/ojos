#include "EstructuraArbolSec.h"

//---------------------------------------------------------------------------
//    Demonios para Colocación Secuencial
//---------------------------------------------------------------------------
namespace DaemonsSeq
{
  bool FindInfo(TBinTreeNode* pCursor, void* pReference)
  {
    return pReference == pCursor->Info();
  }

  void AddFamily(TBinTreeNode* pCursor, void* pReference)
  {
    ((TGSeqList*)pReference)->Insert(pCursor);
  }

  void AddPostOrder(TBinTreeNode* pCursor, void* pReference)
  {
    TPostOrderNode* node = new TPostOrderNode(pCursor->Info(), ((TGBinTreeNode*)pCursor)->Degree());
    ((TGSeqList*)pReference)->Insert(node);
  }

  void AddPreOrder(TBinTreeNode* pCursor, void* pReference)
  {
    ((TGSeqList*)pReference)->Insert(pCursor);
  }

  bool FindFamilyNode(void* pCursor, void* pReference)
  {
    if (*((int*)((TFamilyNode*)pCursor)->Info()) == ((TSSeqNode*)pReference)->info)
      return true;
    else
    {
      (((TSSeqNode*)pReference)->pos)++;
      return false;
    }
  }

  bool FindPostOrderNode(void* pCursor, void* pReference)
  {
    if (*((int*)((TPostOrderNode*)pCursor)->Info()) == ((TSSeqNode*)pReference)->info)
      return true;
    else
    {
      (((TSSeqNode*)pReference)->pos)++;
      return false;
    }
  }

  bool FindPreOrderNode(void* pCursor, void* pReference)
  {
    if (*((int*)((TPreOrderNode*)pCursor)->Info()) == ((TSSeqNode*)pReference)->info)
      return true;
    else
    {
      (((TSSeqNode*)pReference)->pos)++;
      return false;
    }
  }

  void InvertedPostOrder(void* pCursor, void* pReference)
  {
    TCad cad = IntToStr(*((int*)((TFamilyNode*)pCursor)->Info()));
    ((TSCad*)pReference)->sCad += cad + ", ";
  }

  void PreOrder(void* pCursor, void* pReference)
  {
    TCad cad = IntToStr(*((int*)((TPreOrderNode*)pCursor)->Info()));
    ((TSCad*)pReference)->sCad += cad + ", ";
  }

  void Symmetric(void* pCursor, void* pReference)
  {
    TCad cad = IntToStr(*((int*)((TPostOrderNode*)pCursor)->Info()));
    ((TSCad*)pReference)->sCad += cad + ", ";
  }

  void TotalSons(void* pCursor, void* pReference)
  {
    *((int*)pReference) += ((TPostOrderNode*)pCursor)->Degree();
  }
}
//---------------------------------------------------------------------------
//    Implementación de TArbolSecPreorden
//---------------------------------------------------------------------------
TSeqPreOrderTree::TSeqPreOrderTree(TGBinTree* gBinTree) : TGSeqList(gBinTree->TotalNodes())
{
  TGSeqList* tempList = new TGSeqList(size);
  gBinTree->PreOrderForEach(gBinTree->Root(), DaemonsSeq::AddPreOrder, tempList);
  TPreOrderNode* preOrderNode;
  TBinTreeNode* node;
  for (int i = 1; i < size; i++)
  {
    node = (TBinTreeNode*)tempList->ItemInfo(i);
    preOrderNode = new TPreOrderNode(node->Info(), tempList->IndexOf(node->Right()), (TGBinTreeNode*)node->IsLeaf());
    Insert(preOrderNode);
  }
}
//---------------------------------------------------------------------------
void TSeqPreOrderTree::Brothers(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    TIndex index;
    TIndex posFather = Father(pPos);
    void* pos;
    if (posFather > 0)        // Existe el padre
    {
      index = posFather + 1;
      // Recorrer los hermanos
      TPreOrderNode* cursor = (TPreOrderNode*)arrItems[index];
      if (index != pPos)
      {
        pos = new void*;
        *(int*)pos = index;
        pSeqList->Insert(pos);
      }
      while (cursor->RightLink() > -1)
      {
        index = cursor->RightLink();
        cursor = (TPreOrderNode*)arrItems[index];
        if (index != pPos)
        {
          pos = new void*;
          *(int*)pos = index;
          pSeqList->Insert(pos);
        }
      }
    }
    else        // Es la raíz o uno de sus hermanos
    {
      // Hermanos a la izquierda
      int i = pPos - 1;
      int posBrother = pPos;
      while (i > 0)
      {
        if (((TPreOrderNode*)arrItems[i])->RightLink() == posBrother)
        {
          pos = new void*;
          *(int*)pos = i;
          pSeqList->Insert(pos);
          posBrother = i;
        }
        i--;
      }  
      // Hermanos a la derecha
      TPreOrderNode* cursor = (TPreOrderNode*)arrItems[pPos];
      while (cursor->RightLink() > -1)
      {
        index = cursor->RightLink();
        cursor = (TPreOrderNode*)arrItems[index];
        pos = new void*;
        *(int*)pos = index;
        pSeqList->Insert(pos);
      }
    }
  }
}
//---------------------------------------------------------------------------
void TSeqPreOrderTree::Cousins(TIndex pPos, TGSeqList* pSeqList)
{
  // Los primos de un nodo son los hijos de los tios
  TGSeqList* unclesList = new TGSeqList(size);
  Uncles(pPos, unclesList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= unclesList->Length(); i++)
    Sons(*(int*)unclesList->ItemInfo(i), pSeqList);
  delete unclesList;
}
//---------------------------------------------------------------------------
TIndex TSeqPreOrderTree::Father(TIndex pos)
{
  if (pos > 0 && pos <= length)
  {
    if (pos == 1)
      return -1; // El nodo raíz no tiene padre
    else
    {
      int index = 1;
      bool found = false;
      // Recorrer el arreglo hasta la posicion del nodo,
      //pues siempre si padre estará antes.
      while (!found && index < pos)
      {
        TPreOrderNode* cursor = (TPreOrderNode*)arrItems[index];
        // Si no es terminal, tiene hijos y su primer hijo es el siguiente
        if (!cursor->End())
        {
          cursor = (TPreOrderNode*)arrItems[index + 1];
          if (index + 1 == pos)
          {
            found = true;
            return index; // Posición del padre del nodo
          }
          else
          {
            // Recorrer los hermanos
            while (cursor->RightLink() > -1)
            {
              if (pos == cursor->RightLink())
              {
                found = true;
                return index;  // Posición del padre del nodo
              }
              cursor = (TPreOrderNode*)arrItems[cursor->RightLink()];
            }
          }
        }
        index++;
      }
      if (!found)
        return -1;
    }
  }
  return -1;
}
//---------------------------------------------------------------------------
void TSeqPreOrderTree::Nephews(TIndex pPos, TGSeqList* pSeqList)
{
  // Los sobrinos de un nodo son los hijos de los hermanos
  TGSeqList* brothersList = new TGSeqList(size);
  Brothers(pPos, brothersList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= brothersList->Length(); i++)
  {
    Sons(*(int*)brothersList->ItemInfo(i), pSeqList);
  }
  delete brothersList;
}
//---------------------------------------------------------------------------
void TSeqPreOrderTree::Sons(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    TIndex index = pPos;
    // Si no es terminal, tiene hijos y su primer hijo es el siguiente
    if (!((TPreOrderNode*)arrItems[index])->End())
    {
      TPreOrderNode* cursor = (TPreOrderNode*)arrItems[index + 1];
      void* pos = new void*;
      *(int*)pos = index + 1;
      pSeqList->Insert(pos);
      // Recorrer los hermanos
      while (cursor->RightLink() > -1)
      {
        pos = new void*;
        *(int*)pos = cursor->RightLink();
        pSeqList->Insert(pos);
        cursor = (TPreOrderNode*)arrItems[cursor->RightLink()];
      }
    }
  }
}
//---------------------------------------------------------------------------
void TSeqPreOrderTree::Uncles(TIndex pPos, TGSeqList* pSeqList)
{
  // Si no es la raiz o uno de sus hermanos, tiene padre y puede tener tíos
  if (pPos > 1)
    Brothers(Father(pPos), pSeqList);
}
//---------------------------------------------------------------------------
//    Implementación de la Colocación Secuencial en Familiar
//---------------------------------------------------------------------------
TSeqFamilyTree::TSeqFamilyTree(TBinTree* pBinTree):TGSeqList(pBinTree->TotalNodes())
{
  TGSeqList* tempList = new TGSeqList(size);
  pBinTree->InvPostOrderForEach(pBinTree->Root(), DaemonsSeq::AddFamily, tempList);
  TFamilyNode* familyNode;
  TBinTreeNode* node;
  for (int i = 1; i < size; i++)
  {
    node = (TBinTreeNode*)tempList->ItemInfo(i);
    familyNode = new TFamilyNode(node->Info(), tempList->IndexOf(node->Left()), !node->Right());
    Insert(familyNode);
  }
}
//---------------------------------------------------------------------------
void TSeqFamilyTree::Brothers(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    TIndex posFather = Father(pPos);
    TIndex index = 1;
    void* pos;
    if (posFather > 0)
      index = ((TFamilyNode*)arrItems[posFather])->LeftLink(); // Posicion del primer hijo
    // Recorrer los hermanos
    while (!((TFamilyNode*)arrItems[index])->Family())
    {
     if (index != pPos)
     {
       pos = new void*;
       *(int*)pos = index;
       pSeqList->Insert(pos);
     }
     index++;
    }
     if (index != pPos && ((TFamilyNode*)arrItems[index])->Family())
     {
       pos = new void*;
       *(int*)pos = index;
       pSeqList->Insert(pos);
     }
  }
}
//---------------------------------------------------------------------------
void TSeqFamilyTree::Cousins(TIndex pPos, TGSeqList* pSeqList)
{
  // Los primos de un nodo son los hijos de los tios
  TGSeqList* unclesList = new TGSeqList(size);
  Uncles(pPos, unclesList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= unclesList->Length(); i++)
    Sons(*(int*)unclesList->ItemInfo(i), pSeqList);
  delete unclesList;
}
//---------------------------------------------------------------------------
TIndex TSeqFamilyTree::Father(TIndex pPos)
{
  if (pPos > 0 && pPos <= length)
  {
    if (pPos == 1)
      return -1; // El nodo raiz no tiene padre
    else
    {
      int index = 1;
      bool found = false;
      // Recorrer el arreglo hasta la posicion del nodo, pues siempre padre estará antes
      while (!found && index < pPos)
      {
        TFamilyNode* cursor = (TFamilyNode*)arrItems[index];
        // Si tiene enlace izquierdo, tiene hijos y su primer hijo es ese enlace
        if (cursor->LeftLink() > -1)
        {
          TIndex i = cursor->LeftLink(); // Posicion del primer hijo
          cursor = (TFamilyNode*)arrItems[i];
          if (*((int*)cursor->Info()) == *((int*)((TFamilyNode*)arrItems[pPos])->Info()))
          {
            found = true;
            return index; // Posicion del padre del nodo
          }
          else
          {
            // Recorrer los hermanos secuencialmente hasta Familiar: último hermano
            while (!cursor->Family())
            {
              i++;
              cursor = (TFamilyNode*)arrItems[i];
              if (*((int*)((TFamilyNode*)arrItems[pPos])->Info()) == *((int*)cursor->Info()))
              {
                found = true;
                return index;  // Posicion del padre del nodo
              }
            }
          }
        }
        index++;
      }
      if (!found)
        return -1;
    }
  }
  return -1;
}
//---------------------------------------------------------------------------
void TSeqFamilyTree::Nephews(TIndex pPos, TGSeqList* pSeqList)
{
  // Los sobrinos de un nodo son los hijos de los hermanos
  TGSeqList* brothersList = new TGSeqList(size);
  Brothers(pPos, brothersList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= brothersList->Length(); i++)
    Sons(*(int*)brothersList->ItemInfo(i), pSeqList);
  delete brothersList;
}
//---------------------------------------------------------------------------
void TSeqFamilyTree::Sons(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    if (((TFamilyNode*)arrItems[pPos])->LeftLink() > -1)  // Si tiene enlace izquierdo,
                                                          // tiene hijos
    {
      TIndex index = ((TFamilyNode*)arrItems[pPos])->LeftLink();  // Posicion del primer hijo
      TFamilyNode* cursor = (TFamilyNode*)arrItems[index];
      void* pos = new void*;
      *(int*)pos = index;
      pSeqList->Insert(pos);
      // Recorrer los hermanos
      while (!cursor->Family())
      {
        index++;
        cursor = (TFamilyNode*)arrItems[index];
        pos = new void*;
        *(int*)pos = index;
        pSeqList->Insert(pos);
      }
    }
  }
}
//---------------------------------------------------------------------------
void TSeqFamilyTree::Uncles(TIndex pPos, TGSeqList* pSeqList)
{
  // Si no es la raiz o uno de sus hermanos, tiene padre y puede tener tíos
  if (pPos > 1)
    Brothers(Father(pPos), pSeqList);
}
//---------------------------------------------------------------------------
//    Implementación de la Colocación Secuencial en Postorden
//---------------------------------------------------------------------------
TSeqPostOrderTree::TSeqPostOrderTree(TGBinTree* pGBinTree) : TGSeqList(pGBinTree->TotalNodes())
{
  TGSeqList* tempList = new TGSeqList(size);
  pGBinTree->SymmetricForEach(pGBinTree->Root(), DaemonsSeq::AddPostOrder, tempList);
  for (int i = 1; i < size; i++)
    Insert(tempList->ItemInfo(i));
}
//---------------------------------------------------------------------------
void TSeqPostOrderTree::Brothers(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    TIndex posFather = Father(pPos);
    void* pos;
    int degree;
    TIndex index;
    if (posFather > 0)
    {
      degree = ((TPostOrderNode*)arrItems[posFather])->Degree();
      index = posFather;
    }
    else   // Se trata de la raíz o de un hermano de la raíz
    {
      int totalSons = 0;
      ForEach(DaemonsSeq::TotalSons, &totalSons);  // Cantidad de nodos con padre
      degree = Length() - totalSons;               // Cantidad de nodos sin padre
      index = Length() + 1;                            // Siempre aparecen al final
    }
    if (degree > 1)                         // Si el padre tiene grado 1 => no tiene hermanos
    {
      TSeqStack* seqStack = new TSeqStack(size - 1);
      TSStack* sStack;
      while (degree != 0)
      {
        index--;
        if (seqStack->Empty())
        {
          if (index != pPos)
          {
            pos = new void*;
            *(int*)pos = index;
            pSeqList->Insert(pos);
          }
          degree--;
          if (degree > 0)
          {
            if (((TPostOrderNode*)arrItems[index])->Degree() > 0)
            {
              sStack = new TSStack;
              sStack->degree = ((TPostOrderNode*)arrItems[index])->Degree();
              seqStack->Insert(sStack);
            }
          }
        }
        else
        {
          sStack = (TSStack*)seqStack->Delete();
          sStack->degree--;
          if (sStack->degree > 0)
            seqStack->Insert(sStack);
          if (((TPostOrderNode*)arrItems[index])->Degree() > 0)
          {
            sStack = new TSStack;
            sStack->degree = ((TPostOrderNode*)arrItems[index])->Degree();
            seqStack->Insert(sStack);
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
void TSeqPostOrderTree::Cousins(TIndex pPos, TGSeqList* pSeqList)
{
  // Los primos de un nodo son los hijos de los tios
  TGSeqList* unclesList = new TGSeqList(size);
  Uncles(pPos, unclesList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= unclesList->Length(); i++)
    Sons(*(int*)unclesList->ItemInfo(i), pSeqList);
  delete unclesList;
}
//---------------------------------------------------------------------------
TIndex TSeqPostOrderTree::Father(TIndex pPos)
{
  if (pPos > 0 && pPos <= length)
  {
     // Recorrer la lista empezando por el final
     TIndex index = length;
     bool found = false;
     while (!found && index > 0)
     {
       if (((TPostOrderNode*)arrItems[index])->Degree() > 0)  // Si grado > 0 tiene hijos
       {
         // Recorrer los hijos del nodo elementos[indice]
         int degree = ((TPostOrderNode*)arrItems[index])->Degree();
         if (degree > 0)
         {
           TSeqStack* seqStack = new TSeqStack(size - 1);
           TSStack* sStack;
           TIndex i = index;
           while (degree != 0)
           {
             i--;
             if (seqStack->Empty())
             {
              if (i == pPos)
               {
                 found = true;
                 return index;  // Posicion del padre del nodo
               }
               degree--;
               if (degree > 0)
               {
                 if (((TPostOrderNode*)arrItems[i])->Degree() > 0)
                 {
                   sStack = new TSStack;
                   sStack->degree = ((TPostOrderNode*)arrItems[i])->Degree();
                   seqStack->Insert(sStack);
                 }
               }
             }
             else
             {
               sStack = (TSStack*)seqStack->Delete();
               sStack->degree--;
               if (sStack->degree > 0)
                 seqStack->Insert(sStack);
                 if (((TPostOrderNode*)arrItems[i])->Degree() > 0)
                 {
                   sStack = new TSStack;
                   sStack->degree = ((TPostOrderNode*)arrItems[i])->Degree();
                   seqStack->Insert(sStack);
                 }
             } // else
           } // while (grado != 0)
         } // if (grado > 0)
       }
       index--;
     } // while (!encontrado && indice < longitud)
     if (!found)
       return -1;
  } // if (pos > -1 && pos < longitud)
  return -1;
}
//---------------------------------------------------------------------------
void TSeqPostOrderTree::Nephews(TIndex pPos, TGSeqList* pSeqList)
{
  // Los sobrinos de un nodo son los hijos de los hermanos
  TGSeqList* brothersList = new TGSeqList(size);
  Brothers(pPos, brothersList);
  // Recuperar los hijos de cada tío en la lista
  for (int i = 1; i <= brothersList->Length(); i++)
  {
    Sons(*(int*)brothersList->ItemInfo(i), pSeqList);
  }
  delete brothersList;
}
//---------------------------------------------------------------------------
void TSeqPostOrderTree::Sons(TIndex pPos, TGSeqList* pSeqList)
{
  if (pPos > 0 && pPos <= length)
  {
    int degree = ((TPostOrderNode*)arrItems[pPos])->Degree();
    if (degree > 0)
    {
      TSeqStack* seqStack = new TSeqStack(size - 1);
      TSStack* sStack;
      void* pos;
      TIndex index = pPos;
      while (degree != 0)
      {
        index--;
        if (seqStack->Empty())
        {
          pos = new void*;
          *(int*)pos = index;
          pSeqList->Insert(pos);
          degree--;
          if (degree > 0)
          {
            if (((TPostOrderNode*)arrItems[index])->Degree() > 0)
            {
              sStack = new TSStack;
              sStack->degree = ((TPostOrderNode*)arrItems[index])->Degree();
              seqStack->Insert(sStack);
            }
          }
        }
        else
        {
          sStack = (TSStack*)seqStack->Delete();
          sStack->degree--;
          if (sStack->degree > 0)
            seqStack->Insert(sStack);
          if (((TPostOrderNode*)arrItems[index])->Degree() > 0)
          {
            sStack = new TSStack;
            sStack->degree = ((TPostOrderNode*)arrItems[index])->Degree();
            seqStack->Insert(sStack);
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
void TSeqPostOrderTree::Uncles(TIndex pPos, TGSeqList* gSeqList)
{
  // Si no es la raiz o uno de sus hermanos, tiene padre y puede tener tíos
  if (pPos < length)
    Brothers(Father(pPos), gSeqList);
}

void TSeqFamilyTree::SaveToFile(String pFileName, TSaveFunct pSaveFunction, void *ref)
{
  TFamilyNode* familyNode;
  TFile *fich = new TFile(pFileName);
  fich->Open("wb");
  if (fich->IsOpen())
  {
    fich->Write(&size, sizeof(int));
    for (int i = 1; i < size; i++)
    {
      familyNode = (TFamilyNode *)ItemInfo(i);
      bool b = familyNode->Family();
      fich->Write(&b, sizeof(bool));
      int l = familyNode->LeftLink();
      fich->Write(&l, sizeof(int));
      pSaveFunction(familyNode->Info(), fich, ref);
    }
    fich->Close();
  }
  delete fich;  
}

void TSeqFamilyTree::LoadFromFile(String pFileName, TLoadFunct pLoadFunction, void *ref)
{
  TFamilyNode* familyNode;
  TFile *fich = new TFile(pFileName);
  fich->Open("rb");
  int size;
  if (fich->IsOpen())
  {
    fich->Read(&size, sizeof(int));
    for (int i = 1; i < size; i++)
    {
      bool b;
      fich->Read(&b, sizeof(bool));
      int l;
      fich->Read(&l, sizeof(int));
      void *v = pLoadFunction(fich, ref);
      familyNode = new TFamilyNode(v, l, b);
      Insert(familyNode);
    }
    fich->Close();
  }
  delete fich;
}

TGBinTreeNode *TSeqFamilyTree::LocalizeNode(TGBinTree *a, void *info)
{
  return (TGBinTreeNode *)a->FirstThat(a->Root(), DaemonsSeq::FindInfo, info);
}

void *TSeqFamilyTree::ObtainLinked()
{
  TGBinTree *a = new TGBinTree();
  TGBinTreeNode *nodo, *padre;
  padre = NULL;
  nodo = new TGBinTreeNode(((TFamilyNode *)ItemInfo(1))->Info());
  a->InsertNode(nodo, padre);
  for (int i = 1; i <= length; i++)
  {
     TFamilyNode * item = (TFamilyNode *)ItemInfo(i);
     if (item->LeftLink() != -1)
     {
       padre = LocalizeNode(a, item->Info());
       int j = item->LeftLink();
       TFamilyNode * item_j = (TFamilyNode *)ItemInfo(j);
       while ( ! item_j->Family() )
       {
         nodo = new TGBinTreeNode(item_j->Info());
         a->InsertNode(nodo, padre);
         item_j = (TFamilyNode *)ItemInfo(++j);
       }
       nodo = new TGBinTreeNode(item_j->Info());
       a->InsertNode(nodo, padre);
     }
  }
  return a;
}

