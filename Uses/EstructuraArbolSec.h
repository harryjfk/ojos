#ifndef EstructuraArbolSecH
#define EstructuraArbolSecH
#include "EstructuraBinGral.h"
#include <vcl.h>

typedef int TIndex;

struct TSArray {
  void** array;
  void** links;
  TIndex index;
};

struct TSIndex {
  TIndex indexNode;
  TIndex indexFather;
  TIndex index;
};

struct TSSeqNode
{
  int info;
  TIndex pos;
};

struct TSStack
{
  int degree;
};

class TPreOrderNode
{
public:
  TPreOrderNode(void* pInfo, TIndex pRightLink, bool pEnd) :
                aInfo(pInfo), aRightLink (pRightLink), aEnd(pEnd){}
  bool End() {return aEnd;}
  void* Info() {return aInfo;}
  TIndex RightLink () {return aRightLink;}
  void RightLink(TIndex pRightLink) {aRightLink = pRightLink;}
private:
  bool aEnd;
  void* aInfo;
  TIndex aRightLink;
};

class TFamilyNode
{
public:
  TFamilyNode(void* pInfo, TIndex pLeftLink, bool pFamily) :
              aInfo(pInfo), aLeftLink(pLeftLink), aFamily(pFamily){}
  bool Family() {return aFamily;}
  void* Info() {return aInfo;}
  void Info(void* pInfo) { aInfo = pInfo;}
  TIndex LeftLink () {return aLeftLink;}
  void LeftLink (TIndex pLeftLink) {aLeftLink = pLeftLink;}
private:
  bool aFamily;
  void* aInfo;
  TIndex aLeftLink;
};

class TPostOrderNode
{
public:
  TPostOrderNode(void* pInfo, int pDegree) : aInfo(pInfo),
                                             aDegree(pDegree){}
  int Degree() {return aDegree;}
  void* Info() {return aInfo;}
private:
  int aDegree;
  void* aInfo;
};

class TSeqPreOrderTree : public TGSeqList
{
public:
  TSeqPreOrderTree(TGBinTree*);
  void Brothers(TIndex, TGSeqList*);
  void Sons(TIndex, TGSeqList*);
  TIndex Father(TIndex);
  void Cousins(TIndex, TGSeqList*);
  void Nephews(TIndex, TGSeqList*);
  void Uncles(TIndex, TGSeqList*);
};

class TSeqFamilyTree : public TGSeqList
{
public:
  TSeqFamilyTree(TBinTree*);
  TSeqFamilyTree(int pMaxSize): TGSeqList(pMaxSize) {};
  void Brothers(TIndex, TGSeqList*);
  void Sons(TIndex, TGSeqList*);
  TIndex Father(TIndex);
  void Cousins(TIndex, TGSeqList*);
  void Nephews(TIndex, TGSeqList*);
  void Uncles(TIndex, TGSeqList*);

  void SaveToFile(String pFileName, TSaveFunct pSaveFunction, void *ref = NULL);
  void LoadFromFile(String pFileName, TLoadFunct pLoadFunction, void *ref = NULL);
  void *ObtainLinked();

private:
  TGBinTreeNode *LocalizeNode(TGBinTree *a, void *info);
};

class TSeqPostOrderTree : public TGSeqList
{
public:
  TSeqPostOrderTree(TGBinTree*);
  void Brothers(TIndex, TGSeqList*);
  void Sons(TIndex, TGSeqList*);
  TIndex Father(TIndex);
  void Cousins(TIndex, TGSeqList*);
  void Nephews(TIndex, TGSeqList*);
  void Uncles(TIndex, TGSeqList*);
};
#endif
