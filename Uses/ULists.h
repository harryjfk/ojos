//--- ULists.h
#ifndef UListsH
#define UListsH
//---------------------------------------------------------------------------
/*
  Linear structures modelling unit.
*/
#include "UFiles.h"

//---------------------------------------------------------------------------
// Generic list exception class.
//---------------------------------------------------------------------------
class EListException{};

//---------------------------------------------------------------------------
// Derived list exception classes.
//---------------------------------------------------------------------------
class EListOutOfMemory: public EListException{};
class EListOutOfRange: public EListException{};
class EListEmpty: public EListException{};

//--------------------------------------------------
// Base list class.
//--------------------------------------------------
class TBaseList
{
public:
  TBaseList() {length = 0;}
  virtual ~TBaseList() {};
  bool Empty() const {return !length;}
  virtual void Insert(void*) = 0;
  int Length() const {return length;}
protected:
  int length;
};

//---------------------------------------------------------------------------
// Sequential list class.
//---------------------------------------------------------------------------
class TSeqList: public TBaseList
{
public:
  TSeqList(const int);
  TSeqList(const int, const int);
  virtual ~TSeqList();
  bool Full() const {return size == length + 1;}  // Begins at index 1 of the array.
  virtual void Insert(void*);
protected:
  void** arrItems;
  int inc;
  int size;
  void Expand();
};

//--------------------------------------------------
//  Sequential stack class.
//--------------------------------------------------
class TSeqStack: public TSeqList
{
public:
  TSeqStack(int pSize): TSeqList(pSize){}
  virtual ~TSeqStack();
  void* Delete();
  void* Top() const;
};

//--------------------------------------------------
// Sequential queue class.
//--------------------------------------------------
class TSeqQueue: public TSeqList
{
public:
  TSeqQueue(int pSize): TSeqList(pSize), first(1), last(1){}
  virtual ~TSeqQueue();
  void* Delete();
  void* First() const;
  virtual void Insert(void*);
protected:
  int first;
  int last;
  void Expand();
};

//---------------------------------------------------------------------------
// General sequential list class (Sequential list with general operations).
//---------------------------------------------------------------------------
class TGSeqList: public TSeqList
{
typedef void (*TDActionFunct)(void*, void*);
typedef bool (*TDConditionalFunct)(void*, void*);
public:
  TGSeqList(int pSize): TSeqList(pSize){}
  virtual ~TGSeqList();
  void* Delete(int);
  void* FirstThat(TDConditionalFunct, void* = NULL);
  void ForEach(TDActionFunct, void* = NULL);
  int IndexOf(void*);
  virtual void Insert(void* pItemInfo);
  void Insert(void*, int);
  void* ItemInfo(int) const;
  void LoadFromFile(String pFileName, TLoadFunct pLoadFunct, void *pRef = NULL);
  void SaveToFile(String pFileName, TSaveFunct pSaveFunct, void* pRef = NULL);
};

//---------------------------------------------------------------------------
// Node class (for the Linked list).
//---------------------------------------------------------------------------
class TNode
{
public:
  TNode(void* pInfo): info(pInfo), next(NULL){}
  virtual ~TNode();
  void* Info() const {return info;}
  void Info(void* pInfo) {info = pInfo;}
  TNode* Next() const {return next;}
  void Next(TNode* pNext) {next = pNext;}
protected:
  void* info;
  TNode* next;
};

//---------------------------------------------------------------------------
// Linked list class.
//---------------------------------------------------------------------------
class TLinkedList: public TBaseList
{
public:
  TLinkedList(): TBaseList(), first(NULL) {}
  virtual ~TLinkedList();
  TNode* First() {return first;}
  void First(TNode* pFirst) {first = pFirst;}
  virtual void Insert(void*);
protected:
  TNode* first;
};

//--------------------------------------------------
//  Linked stack class.
//--------------------------------------------------
class TLinkedStack: public TLinkedList
{
public:
  TLinkedStack(): TLinkedList(){}
  virtual ~TLinkedStack();
  void* Delete();
  virtual void Insert(void*);
  void* Top() const;
};

//--------------------------------------------------
//  Linked queue class.
//--------------------------------------------------
class TLinkedQueue: public TLinkedList
{
public:
  TLinkedQueue(): TLinkedList(), last(NULL){}
  virtual ~TLinkedQueue();
  void* Delete();
  void* FirstItem() const;
  virtual void Insert(void*);
protected:
  TNode* last;
};

//---------------------------------------------------------------------------
// General linked list class (Linked list with general operations).
//---------------------------------------------------------------------------
class TGLinkedList: public TLinkedList
{
typedef void (*TDActionFunct)(void*, void*);
typedef bool (*TDConditionalFunct)(void*, void*);
public:
  TGLinkedList(): TLinkedList(){}
  virtual ~TGLinkedList();
  void* Delete(int);
  void* FirstThat(TDConditionalFunct, void* = NULL);
  void ForEach(TDActionFunct, void* = NULL);
  int IndexOf(void*);
  virtual void Insert(void* pItem);
  void Insert(void*, int);
  TNode* Item(int) const;
  void* ItemInfo(int) const;
  void LoadFromFile(String pFileName, TLoadFunct pLoadFunct, void* pRef = NULL);
  void SaveToFile(String pFileName, TSaveFunct pSaveFunct, void* pRef = NULL);
};

//---------------------------------------------------------------------------
// Circular linked list class.
//---------------------------------------------------------------------------
class TCircLinkedList: public TGLinkedList
{
public:
  TCircLinkedList(): TGLinkedList() {}
  virtual ~TCircLinkedList();
  void* Delete(int);
  virtual void Insert(void*);
  void Insert(void*, int);
  TNode* Successor(TNode*);
};

//---------------------------------------------------------------------------
// Node class (for the Double linked list).
//---------------------------------------------------------------------------
class TDoubleNode: public TNode
{
public:
  TDoubleNode(void* pItemInfo): TNode(pItemInfo), previous(NULL){}
  virtual ~TDoubleNode();
  TDoubleNode* Previous() {return previous;}
  void Previous(TDoubleNode* pPrev) {previous = pPrev;}
protected:
  TDoubleNode* previous;
};

//---------------------------------------------------------------------------
// Double linked list class.
//---------------------------------------------------------------------------
class TDoubleLinkedList: public TGLinkedList
{
public:
  TDoubleLinkedList(): TGLinkedList() {}
  virtual ~TDoubleLinkedList();
  void* Delete(int);
  virtual void Insert(void*);
  void Insert(void*, int);
};

//---------------------------------------------------------------------------
// Circular double linked list class.
//---------------------------------------------------------------------------
class TDoubleCircLinkedList: public TDoubleLinkedList
{
public:
  TDoubleCircLinkedList(): TDoubleLinkedList() {}
  virtual ~TDoubleCircLinkedList();
  void* Delete(int);
  virtual void Insert(void*);
  void Insert(void*, int);
};

#endif

