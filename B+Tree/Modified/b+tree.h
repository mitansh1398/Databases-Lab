#pragma once
#include "node.h"

class BPlusTree{
  public:
    BPlusTree(int numIndexPointers,int numDataPointers);
    IndexNode* toIndexNode(void *p);
    DataNode* toDataNode(void *p);
    void insert(int key);
    void *insertLeafNode(DataNode *x, int newKey, int &newValue);
    void *insertNonLeafNode(IndexNode *x, int newKey, int depth, int &newValue);
    void insertNonFull(void *node, int key, int depth);
    DataNode* search(void *node, int key, int depth);
    void LevelOrderTraversal(void *node, int depth);
  

    void* root;
    int numIndexPointers;
    int numDataPointers;
    int depth;
};