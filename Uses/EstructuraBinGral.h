#ifndef EstructuraBinGralH
#define EstructuraBinGralH
#include "EstructuraArbolBin.h"

class TGBinTreeNode: public TBinTreeNode
{
  public:
    TGBinTreeNode(void* pInfo): TBinTreeNode(pInfo) {}
    bool IsLeaf() {return !aLeft;}     // virtual
    int Degree();                      // virtual
};

class TGBinTree: public TBinTree
{
  typedef void (*TActionFunction)(TBinTreeNode* pCursor, void* pReference = NULL);
  typedef bool (*TConditionalFunction)(TBinTreeNode* pCursor, void*  pReference = NULL);

  public:
    void* DeleteNode(TGBinTreeNode*);
    virtual TBinTreeNode* FirstThat(TBinTreeNode*, TConditionalFunction, void*);
    TBinTreeNode* GetFather(TBinTreeNode*);            // virtual
    TGLinkedList* GetLeaves();                         // virtual
    TGLinkedList* GetSons(TBinTreeNode*);
    bool InsertNode(TGBinTreeNode*, TGBinTreeNode*);
    virtual void InvPostOrderForEach(TBinTreeNode*, TActionFunction, void*);
    virtual void PostOrderForEach(TBinTreeNode*, TActionFunction, void*);
    virtual void PreOrderForEach(TBinTreeNode*, TActionFunction, void*);
    int TreeLevel();
  private:
    virtual int Level(TBinTreeNode*);
};
#endif

