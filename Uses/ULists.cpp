//--- ULists.cpp
#pragma hdrstop
#include <vcl.h>
//---------------------------------------------------------------------------
/*
   Linear structures modelling Unit.
*/
#include "ULists.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Methods of the Sequential list class.
//---------------------------------------------------------------------------
TSeqList::TSeqList(const int pSize): TBaseList()
{
  size = pSize + 1;           // Para no utilizar el cero
  arrItems = new void*[size];
  inc = size / 10 + 1;
}
//---------------------------------------------------------------------------
TSeqList::TSeqList(const int pSize, const int pInc): TBaseList()
{
  size = pSize + 1; // Begins at index 1 of the array.          // Para no utilizar el cero
  arrItems = new void*[size];
  inc = pInc;
}
//---------------------------------------------------------------------------
TSeqList::~TSeqList()
{
  size = 0;
  for(int i = 0; i <= length; i++)
    delete arrItems[i];
  delete[] arrItems;
  length = 0;
  inc = 0;
}
//---------------------------------------------------------------------------
void TSeqList::Expand()
{
  size += inc;
  void** arrTemp = new void*[size];
  for (int i = 0; i <= length; i++)
    arrTemp[i] = arrItems[i];
  delete[] arrItems;
  arrItems = arrTemp;
}
//---------------------------------------------------------------------------
void TSeqList::Insert(void* pInfo)
{
  if (Full())
    Expand();
  arrItems[++length] = pInfo;
}

//---------------------------------------------------------------------------
// Methods of the sequential stack class.
//---------------------------------------------------------------------------
TSeqStack::~TSeqStack()
{
  size = 0;
  for(int i = 0; i <= length; i++)
    delete arrItems[i];
  delete[] arrItems;
  length = 0;
  inc = 0;
};
//---------------------------------------------------------------------------
void* TSeqStack::Delete()
{
  if (Empty())
    throw EListEmpty();
  void* deleted = Top();
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void* TSeqStack::Top() const
{
  if (Empty())
    throw EListEmpty();
  return arrItems[length];
}

//---------------------------------------------------------------------------
// Methods of the sequential queue class.
//---------------------------------------------------------------------------
TSeqQueue::~TSeqQueue()
{
  for(int i = 0; i < length; i++)
  {
    void* deleted = Delete();
    delete deleted;
  }
  delete[] arrItems;
  length = 0;
  inc = 0;
  size = 0;
};
//---------------------------------------------------------------------------
void* TSeqQueue::Delete()
{
  if (Empty())
    throw EListEmpty();
  void* deleted = arrItems[first];
  // Moves the "first" indicator "forward".
  first = (first % (size - 1))  + 1;
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void TSeqQueue::Expand()
{
  size += inc;
  void** arrTemp = new void*[size];
  int cursor = first;
  for (int i = 1; i <= length; i++)
  {
    arrTemp[i] = arrItems[cursor];
    cursor = (cursor % length) + 1;
  }
  delete [] arrItems;
  arrItems = arrTemp;
}
//---------------------------------------------------------------------------
void* TSeqQueue::First() const
{
  if (Empty())
    throw EListEmpty();
  return arrItems[first];
}
//---------------------------------------------------------------------------
void TSeqQueue::Insert(void* pItem)
{
  if(!Full())
  {
    arrItems[last] = pItem;
    last = (last % (size - 1)) + 1;
    length++;
  }
  else  // Array of Queue is full!
  {
    Expand();
    arrItems[++length] = pItem;
    // Reassign the values of "first" and "last" indicators.
    first = 1;
    last = length + 1;
  }
}

//---------------------------------------------------------------------------
// Methods of the General sequential list class (Sequential list with general operations).
//---------------------------------------------------------------------------
TGSeqList::~TGSeqList()
{
  size = 0;
  for (int i = 0; i < length; i++)
  {
    void* deleted = arrItems[i];
    delete deleted;
  }
  delete[] arrItems;
  length = 0;
  inc = 0;
}
//---------------------------------------------------------------------------
void* TGSeqList::Delete(int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange(); // Given Item positión out of range.
  void* deleted = arrItems[pPos];
  for (int i = pPos + 1; i <= length; i++)
    arrItems[i - 1] = arrItems[i];
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void* TGSeqList::FirstThat(TDConditionalFunct pCondition, void* pReference)
{
  int i = 1;
  bool found = false;
  while ((i <= length) && (!found))
    if (pCondition(arrItems[i], pReference))
      found = true;
    else
      i++;
  return ((found)?arrItems[i]:NULL);
}
//---------------------------------------------------------------------------
void TGSeqList::ForEach(TDActionFunct pAction, void* pReference)
{
  for (int i = 1; i <= length; i++)
    pAction(arrItems[i], pReference);
}
//---------------------------------------------------------------------------
int TGSeqList::IndexOf(void* pInfo)
{
  if (pInfo)
  {
    int i = 1;
    while ((i <= length) && (arrItems[i] != pInfo))
      i++;
    return (i <= length)? i : -1;
  }
  else
    return -1;
}
//---------------------------------------------------------------------------
void TGSeqList::Insert(void* pInfo, int pPos)
{
  if (pPos <= 0 || pPos > length)
    throw EListOutOfRange();
  if (Full())
  {
    size += inc;
    void** arrTemp = new void*[size];
    for (int i = 0; i <= pPos; i++)                // copiar hasta pos
      arrTemp[i] = arrItems[i];
    arrTemp[pPos] = pInfo;                        // colocar el nuevo elemento
    for (int i = pPos; i <= length; i++)  // copiar de pos al final
      arrTemp[i + 1] = arrItems[i];
    delete [] arrItems;
    arrItems = arrTemp;
  }
  else
  {
    for (int i = length; i >= pPos; i--)
      arrItems[i] = arrItems[i - 1];
    arrItems[pPos] = pInfo;
  }
  length++;
}
//---------------------------------------------------------------------------
void* TGSeqList::ItemInfo(int pPos) const
{
 if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange(); //Posición fuera de rango
 return arrItems[pPos];
}
//---------------------------------------------------------------------------
void TGSeqList::LoadFromFile(String pFileName, TLoadFunct pLoadFunct, void* pRef)
{
  TFile *file = new TFile(pFileName);
  file->Open("rb");
  if (file->IsOpen())
  {
    void *v = pLoadFunct(file, pRef);
    while (!file->Eof())
    {
      if (v)
        Insert(v);
      v = pLoadFunct(file, pRef);
    }
    file->Close();
  }
}

void TGSeqList::Insert(void* pItemInfo) {TSeqList::Insert(pItemInfo);}

//---------------------------------------------------------------------------
void TGSeqList::SaveToFile(String pFileName, TSaveFunct pSaveFunct, void* pRef)
{
  TFile *file = new TFile(pFileName);
  file->Open("wb");
  if (file->IsOpen())
  {
    for (int i = 1; i <= length; i++)
      pSaveFunct(arrItems[i], file, pRef);
    file->Close();
  }
}

//---------------------------------------------------------------------------
// Methods of the Node class.
//---------------------------------------------------------------------------
TNode::~TNode()
{
  if (info)
    delete info;
}
//---------------------------------------------------------------------------
// Methods of the Linked list class.
//---------------------------------------------------------------------------
TLinkedList::~TLinkedList()
{
  TNode* cursor;
  while (first)
  {
    cursor = first;
    first = first->Next();
    delete cursor;
  }
  length = 0;
}
//---------------------------------------------------------------------------
void TLinkedList::Insert(void* pItem)
{
  if (Empty())
    first = new TNode(pItem);
  else
  {
    TNode* cursor = first;
    while (cursor->Next())
      cursor = cursor->Next();
    cursor->Next(new TNode(pItem));
  }
  length++;
}

//---------------------------------------------------------------------------
// Methods of the Linked stack class.
//---------------------------------------------------------------------------
TLinkedStack::~TLinkedStack()
{
  TNode* cursor;
  while (first)
  {
    cursor = first;
    first = first->Next();
    delete cursor;
  }
  length = 0;
}
//---------------------------------------------------------------------------
void* TLinkedStack::Delete()
{
  if (Empty())
    throw EListEmpty();
  TNode* cursor = first;
  first = first->Next();
  void* deleted = cursor->Info();
  // The node must be deleted and the info returned!
  cursor->Info(NULL);
  delete cursor;
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void TLinkedStack::Insert(void* pItem)
{
  if (Empty())
    TLinkedList::Insert(pItem);
  else
  {
    TNode* newNode = new TNode(pItem);
    newNode->Next(first);
    first = newNode;
    length++;
  }
}
//---------------------------------------------------------------------------
void* TLinkedStack::Top() const
{
  if (!first)
    throw EListEmpty();
  return first->Info();
}

//---------------------------------------------------------------------------
// Methods of the Linked queue class.
//---------------------------------------------------------------------------
TLinkedQueue::~TLinkedQueue()
{
  TNode* cursor;
  while (first)
  {
    cursor = first;
    first = first->Next();
    delete cursor;
  }
}
//---------------------------------------------------------------------------
void* TLinkedQueue::Delete()
{
  if (Empty())
    throw EListEmpty();
  if (first == last)
    last = NULL;
  TNode* cursor = first;
  first = first->Next();
  void* deleted = cursor->Info();
  // The node must be destroyed and the info returned!
  cursor->Info(NULL);
  delete cursor;
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void* TLinkedQueue::FirstItem() const
{
  if (Empty())
    throw EListEmpty();
  return first->Info();
}
//---------------------------------------------------------------------------
void TLinkedQueue::Insert(void* pItem)
{
  TLinkedList::Insert(pItem);
  if (length == 1)
    last = first;
  else
    last = last->Next();
}

//--------------------------------------------------------------
// Methods of the General linked list class (Linked list with general operations).
//--------------------------------------------------------------
TGLinkedList::~TGLinkedList()
{
  TNode* cursor;
  while (first)
  {
    cursor = first;
    first = first->Next();
    delete cursor;
  }
}
//--------------------------------------------------------------
void* TGLinkedList::Delete(int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TNode* cursor = Item(pPos);
  if (pPos != 1)
    Item(pPos - 1)->Next(cursor->Next());
  else  // Delete the node at first position.
    first = first->Next();
  void* deleted = cursor->Info();
  // The node must be destroyed and the info returned!
  cursor->Info(NULL);
  delete cursor;
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void* TGLinkedList::FirstThat(TDConditionalFunct pCondition, void* pReference)
{
  TNode* cursor = first;
  bool found = false;
  int i = 1;
  while ((i <= length) && (!found))
    if (pCondition(cursor->Info(), pReference))
      found = true;
    else
    {
      cursor = cursor->Next();
      i++;
    }
  return (found ? cursor->Info() : NULL);
}
//---------------------------------------------------------------------------
void TGLinkedList::ForEach(TDActionFunct pAction, void* pReference)
{
  TNode* cursor = first;
  int i = 1;
  while (i <= length)
  {
    pAction(cursor->Info(), pReference);
    cursor = cursor->Next();
    i++;
  }
}
//---------------------------------------------------------------------------
int TGLinkedList::IndexOf(void* pInfo)
{
  if (pInfo)
  {
    int i = 1;
    while ((i <= length) && (ItemInfo(i) != pInfo))
      i++;
    return (i <= length)? i : -1;
  }
  else
    return -1;
}
//---------------------------------------------------------------------------
void TGLinkedList::Insert(void* pItem)
{
  TNode *node = new TNode(pItem);
  if (Empty())
    first = node;
  else
    Item(length)->Next(node);
  length++;
}
//---------------------------------------------------------------------------
void TGLinkedList::Insert(void* pItem, int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TNode *temp = new TNode(pItem);
  if (pPos != 1)
  {
    TNode* cursor = Item(pPos - 1);
    temp->Next(cursor->Next());
    cursor->Next(temp);
  }
  else
  {
    // Insert at the first position
    temp->Next(first);
    first = temp;
  }
  length++;
}
//---------------------------------------------------------------------------
TNode* TGLinkedList::Item(int pPos) const
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange(); //Posición fuera de rango
  TNode* cursor = first;
  for (int i = 1; i < pPos; i++)
    cursor = cursor->Next();
  return cursor;
}
//---------------------------------------------------------------------------
void* TGLinkedList::ItemInfo(int pPos) const
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TNode* cursor = first;
  for (int i = 1; i < pPos; i++)
    cursor = cursor->Next();
  return cursor->Info();
}
//---------------------------------------------------------------------------
void TGLinkedList::LoadFromFile(String pFileName, TLoadFunct pLoadFunct, void* pReference)
{
  TFile *file = new TFile(pFileName);
  file->Open("rb");
  if (file->IsOpen())
  {
    void *v = pLoadFunct(file, pReference);
    while (!file->Eof())
    {
      if (v)
        Insert(v);
      v = pLoadFunct(file, pReference);
    }
    file->Close();
  }
}
//---------------------------------------------------------------------------
void TGLinkedList::SaveToFile(String pFileName, TSaveFunct pSaveFunct, void* pReference)
{
  TFile *file = new TFile(pFileName);
  file->Open("wb");
  if (file->IsOpen())
  {
    TNode* cursor = first;
    for (int i = 1; i <= length; i++)
    {
      pSaveFunct(cursor->Info(), file, pReference);
      cursor = cursor->Next();
    }
    file->Close();
  }
}

//---------------------------------------------------------------------------
// Circular linked list class.
//---------------------------------------------------------------------------
TCircLinkedList::~TCircLinkedList()
{
  if (!Empty())
  {
    TNode* cursor;
    while (first->Next() != first)
    {
      cursor = first->Next();
      first->Next(cursor->Next());
      delete cursor;
    }
    delete first;
  }
}
//---------------------------------------------------------------------------
void* TCircLinkedList::Delete(int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  void* deleted = TGLinkedList::Delete(pPos);
  if (Empty())
    first = NULL;
  else
    if (pPos == 1) // Casos extremo: el primero.
      Item(length)->Next(first);
  return deleted;
}
//---------------------------------------------------------------------------
void TCircLinkedList::Insert(void* pItem)
{
  TGLinkedList::Insert(pItem);
  Item(length)->Next(first);
}
//---------------------------------------------------------------------------
void TCircLinkedList::Insert(void* pItem, int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TGLinkedList::Insert(pItem, pPos);
  if (pPos == 1)
    Item(length)->Next(first);
}
//---------------------------------------------------------------------------
TNode* TCircLinkedList::Successor(TNode* pNode)
{
  return pNode->Next();
}

//----------------------------------------------------------------------------
// Methods of the Node class (for the Double linked list).
//----------------------------------------------------------------------------
TDoubleNode::~TDoubleNode()
{
  if (info)
    delete info;
}

//----------------------------------------------------------------------------
// Methods of the Double linked list class.
//----------------------------------------------------------------------------
TDoubleLinkedList::~TDoubleLinkedList()
{
  TDoubleNode* cursor;
  while (first)
  {
    cursor = (TDoubleNode *)first;
    first = first->Next();
    delete cursor;
  }
}
//----------------------------------------------------------------------------
void* TDoubleLinkedList::Delete(int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange(); //Posición fuera de rango
  void* deleted = TGLinkedList::Delete(pPos);
  if (!Empty())
  {
    if (pPos == 1)
      ((TDoubleNode*)Item(1))->Previous(NULL);
    else
       if (pPos < length)
         ((TDoubleNode*)Item(pPos))->Previous((TDoubleNode*)Item(pPos - 1));
  }
  return deleted;
}
//---------------------------------------------------------------------------
void TDoubleLinkedList::Insert(void* pItem)
{
  TDoubleNode* newItem = new TDoubleNode(pItem);
  if (Empty())
    first = newItem;
  else
  {
    TDoubleNode* cursor = (TDoubleNode *)first;
    while (cursor->Next())
      cursor = (TDoubleNode *)cursor->Next();
    cursor->Next(newItem);
    newItem->Previous(cursor);
  }
  length++;
}
//---------------------------------------------------------------------------
void TDoubleLinkedList::Insert(void* pItem, int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TDoubleNode *temp = new TDoubleNode(pItem);
  if (pPos != 1)
  {
    TDoubleNode* cursor = (TDoubleNode*)Item(pPos - 1);
    temp->Next(cursor->Next());
    temp->Previous(cursor);
    cursor->Next(temp);
    ((TDoubleNode*)(temp->Next()))->Previous(temp);
  }
  else
  {
    // Insert at the first position
    temp->Next(first);
    first = temp;
    ((TDoubleNode*)(temp->Next()))->Previous(temp);
  }
  length++;
}

//---------------------------------------------------------------------------
// Methods of the Circular double linked list class.
//---------------------------------------------------------------------------
TDoubleCircLinkedList::~TDoubleCircLinkedList()
{
  if (!Empty())
  {
    TDoubleNode* cursor;
    while (first->Next() != first)
    {
      cursor = (TDoubleNode*)(first->Next());
      first->Next(cursor->Next());
      delete cursor;
    }
    delete first;
  }
}
//---------------------------------------------------------------------------
void* TDoubleCircLinkedList::Delete(int pPos)
{
  if ((pPos <= 0) || (pPos > length))
    throw EListOutOfRange();
  TDoubleNode* cursor = (TDoubleNode*)Item(pPos);
  if (pPos != 1)
  {
    TDoubleNode* prev = cursor->Previous();
    prev->Next(cursor->Next());
    ((TDoubleNode*)(cursor->Next()))->Previous(prev);
  }
  else  // Delete the node at first position.
  {
    if (length == 1) // There is only one item.
      first = NULL;
    else
    {
      first = first->Next(); // Now, the first item must be thus following the one pointed by cursor.
      ((TDoubleNode*)first)->Previous(cursor->Previous());
      ((TDoubleNode*)first)->Previous()->Next(first);
    }
  }
  void* deleted = cursor->Info();
  // The node must be destroyed and the info returned!
  cursor->Info(NULL);
  delete cursor;
  length--;
  return deleted;
}
//---------------------------------------------------------------------------
void TDoubleCircLinkedList::Insert(void* pItem)
{
  TDoubleLinkedList::Insert(pItem);
  TDoubleNode* cursor = (TDoubleNode*)Item(length);
  cursor->Next(first);
  ((TDoubleNode*)first)->Previous(cursor);
}
//---------------------------------------------------------------------------
void TDoubleCircLinkedList::Insert(void* pItem, int pPos)
{
  TDoubleLinkedList::Insert(pItem, pPos);
  if ((pPos == 1) || (pPos == length))
  {
    TDoubleNode* last = (TDoubleNode*)Item(length);
    ((TDoubleNode*)first)->Previous(last);
    last->Next(first);
  }
}
//---------------------------------------------------------------------------

