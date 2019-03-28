#pragma once
#include<bits/stdc++.h>

using namespace std; 

class Node{
	public:
		Node(int numValues);
		void sortedInsert(int val);
    	int getNumKeys();
    	void setNumKeys(int val);
	    int getKeyAtIndex(int i);
    	void setKeyAtIndex(int key, int i);
		void incrementNumKeys();
		void decrementNumKeys();
		int nodeLowerBound(int i);
		void insertAtEnd(int key);
	private:
		int numKeys;
		vector<int> keys;
};

class IndexNode: public Node{
	public:
		IndexNode(int numDataValues);
		void *getIndexPointerAt(int i);
		void setIndexPointerAt(void *p,int i);
	private:
		vector<void*> indexPointers;
		int numIndexPointers;
};

class DataNode: public Node{
	public:
		DataNode(int numDataPointers);
		DataNode* getDataNodeLeft();
		void setDataNodeLeft(DataNode *p);
		DataNode* getDataNodeRight();
		void setDataNodeRight(DataNode *p);
	private:
		DataNode *left, *right;
		int numDataPointers;
};

