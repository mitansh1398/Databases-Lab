#include "node.h"


int Node::getNumKeys(){
  return numKeys;
}

void Node::incrementNumKeys(){
	numKeys++;
}

void Node::decrementNumKeys(){
	numKeys--;
} 

int Node::nodeLowerBound(int i){
	auto it = lower_bound(keys.begin(), keys.begin()+numKeys, i);
	return (it-keys.begin());
}


void Node::setNumKeys(int val){
	numKeys = val;
}

int Node::getKeyAtIndex(int i){
  return keys[i];
}

void Node::setKeyAtIndex(int key, int i){
	keys[i] = key;
}

void Node::insertAtEnd(int key){
	keys[numKeys] = key;
	incrementNumKeys();
}

Node::Node(int totalKeys){
	numKeys = 0;
	keys.resize(totalKeys+1);
}


IndexNode::IndexNode(int numIndexPointers)
	: Node(numIndexPointers-1)
{
	indexPointers.resize(numIndexPointers);
}

void* IndexNode::getIndexPointerAt(int i){
	return indexPointers[i];
}

void IndexNode::setIndexPointerAt(void* p, int i){
	indexPointers[i] = p;
}

DataNode::DataNode(int numDataPointers)
	: Node(numDataPointers)
{
	left = NULL;
	right = NULL;
}

DataNode* DataNode::getDataNodeLeft(){
	return left;
}

void DataNode::setDataNodeLeft(DataNode *p){
	left = p;
}

DataNode* DataNode::getDataNodeRight(){
	return right;
}

void DataNode::setDataNodeRight(DataNode *p){
	right = p;
}