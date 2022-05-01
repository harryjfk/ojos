//---------------------------------------------------------------------------
#ifndef EstructuraArbolBinH
#define EstructuraArbolBinH
//---------------------------------------------------------------------------
#include  "ULists.h"

typedef AnsiString TCad;

struct TSCad
{
  TCad sCad;
};

struct TSParents
{
  TCad sPersonName;
  TCad sParents;
};

class TBinTreeNode;

struct TSSons
{
  TBinTreeNode* sLeft;
  TBinTreeNode* sRight;
};

struct TSSubTree
{
  TCad sPersonName;
  TBinTreeNode* sLocation;
  TGLinkedList* sAncestors;
};

struct TSFather
{
   TBinTreeNode* sNode;
   TBinTreeNode* sFather;
};
//-----------------------
// class TNodoArbol
//-----------------------
class TBinTreeNode
{
  public:
    TBinTreeNode(void* pInfo) : aInfo(pInfo), aLeft(NULL), aRight(NULL) {}
    virtual int Degree();
    void* Info() {return aInfo;}
    virtual bool IsLeaf() {return (!aLeft && !aRight);}  //  Degree() == 0
    TBinTreeNode* Left() {return aLeft;}
    void Left(TBinTreeNode* pNode) {aLeft = pNode;}
    TBinTreeNode* Right() {return aRight;}
    void Right(TBinTreeNode* pNode) {aRight = pNode;}
  protected:
    void*  aInfo;
    TBinTreeNode* aLeft;
    TBinTreeNode* aRight;
};
//----------------------
// class TArbol
//----------------------
class TBinTree
{
  typedef void (*TActionFunction)(TBinTreeNode* pCursor, void* pReference = NULL);
  typedef bool (*TConditionalFunction)(TBinTreeNode* pCursor, void*  pReference = NULL);
  public:
     TBinTree() {aRoot = NULL;}
     ~TBinTree();
     void* DeleteNode(TBinTreeNode*);
     bool DivideTree(TBinTreeNode*, TBinTree* &, TBinTree* &);
     bool Empty(){return !aRoot;}
     virtual TBinTreeNode* FirstThat(TBinTreeNode*, TConditionalFunction, void*);
     virtual TBinTreeNode* GetFather(TBinTreeNode*);
     virtual TGLinkedList* GetLeaves();
     TSSons* GetSons(TBinTreeNode*);
     TGLinkedList* GetSubTree(TBinTreeNode*, char);
     bool InsertNode(TBinTreeNode*, char, TBinTreeNode*);
     virtual void InvPostOrderForEach(TBinTreeNode*, TActionFunction, void*);
     int NodeLevel(TBinTreeNode*);
     virtual void PostOrderForEach(TBinTreeNode*, TActionFunction, void*);
     virtual void PreOrderForEach(TBinTreeNode*, TActionFunction, void*);
     TBinTreeNode* Root() {return aRoot;}
     void Root(TBinTreeNode* pRoot) {aRoot = pRoot;}
     virtual void SymmetricForEach(TBinTreeNode*, TActionFunction, void*);
     int TotalNodes();
     int TreeDegree();
     int TreeLevel();
  protected:
     TBinTreeNode* aRoot;
  private:
    bool AllNodesDegreeTwo();
    virtual void DeleteNotRoot(TBinTreeNode*, int, TBinTreeNode*);
    virtual void DeleteRoot(TBinTreeNode*, int);
    TBinTreeNode* FirstIncompletedNode(TBinTreeNode*);
    void GetNodeSubTree(TBinTreeNode*, TBinTreeNode*, TBinTree* &);
    virtual int Level(TBinTreeNode*);
    void PreOrderFirstIncompletedNode(TBinTreeNode*, TBinTreeNode* &);
};
#endif

 