#include "b+tree.h"

/*Following notations and part of procedures are taken from algorithms of Btree in CLRS*/  

BPlusTree::BPlusTree(int numIndexPointers,int numDataPointers){
	root = new DataNode(numDataPointers);
	this->numIndexPointers = numIndexPointers;
	this->numDataPointers = numDataPointers;
	this->depth = 0;
}

IndexNode* BPlusTree::toIndexNode(void *p){
	return static_cast<IndexNode*>(p);
}

DataNode* BPlusTree::toDataNode(void *p){
	return static_cast<DataNode*>(p);
}

DataNode* BPlusTree::search(void *node, int key, int depth){
	
	if(depth==0){
		DataNode *x = toDataNode(node);	
		for(int i = 0; i < x->getNumKeys(); i++)
			if(x->getKeyAtIndex(i) == key)
				return x;
		return NULL;	
	}
	else{
		IndexNode *x = toIndexNode(node);
		int i = 0;
		while(i < x->getNumKeys() && key >= x->getKeyAtIndex(i))
			i = i + 1;
		return search(x->getIndexPointerAt(i), key, depth-1);
	}
}


void BPlusTree::insert(int key){
	if(depth==0){
		DataNode *root = toDataNode(this->root);
		int newValue;
		void *temp = insertLeafNode(root, key, newValue);
		if(temp!=NULL){
			IndexNode* newRoot = new IndexNode(numIndexPointers);
			newRoot->insertAtEnd(newValue);
			newRoot->setIndexPointerAt(root, 0);
			newRoot->setIndexPointerAt(temp, 1);
			this->root = newRoot;
			this->depth++;
		}
	}
	else{
		IndexNode *root = toIndexNode(this->root);
		int newValue;
		void* temp = insertNonLeafNode(root, key, 0, newValue);
	}
}


void* BPlusTree::insertLeafNode(DataNode *x, int newKey, int &newValue){
	cout<<"Value inserted : "<<newKey<<endl;
	int lb = x->nodeLowerBound(newKey);
	cout<<lb<<endl;
	for(int i=x->getNumKeys()-1; i>=lb; i--){
		x->setKeyAtIndex(x->getKeyAtIndex(i), i+1);
	}
	x->setKeyAtIndex(newKey, lb);
	x->incrementNumKeys();
	if(x->getNumKeys() == this->numDataPointers+1){
		DataNode* newChild = new DataNode(numDataPointers);
		for(int i=numDataPointers/2+1; i<=numDataPointers; i++){
			newChild->insertAtEnd(x->getKeyAtIndex(i));
		}
		x->setNumKeys(numDataPointers/2+1);
		newChild->setDataNodeRight(x->getDataNodeRight());
		newChild->setDataNodeLeft(x);
		if(x->getDataNodeRight()!=NULL){
			x->getDataNodeRight()->setDataNodeLeft(newChild);
		}
		x->setDataNodeRight(newChild);
		newValue = x->getKeyAtIndex(x->getNumKeys()-1);
		// x->printNode();
		// newChild->printNode();
		return newChild;
	}
	return NULL;
}

void* BPlusTree::insertNonLeafNode(IndexNode *x, int newKey, int depth, int &newValue){
	cout<<"Index"<<depth<<endl;
	int lb = x->nodeLowerBound(newKey);
	cout<<lb<<endl;
	void* temp;
	if(depth+1==this->depth){
		temp = insertLeafNode(toDataNode(x->getIndexPointerAt(lb)), newKey, newValue);
	} else {
		temp = insertNonLeafNode(toIndexNode(x->getIndexPointerAt(lb)), newKey, depth+1, newValue);
	}
	if(temp==NULL){
		return NULL;
	} else {
		// cout<<"node being inserted"<<newValue<<endl;
		for(int i=x->getNumKeys()-1; i>=lb; i--){
			x->setKeyAtIndex(x->getKeyAtIndex(i), i+1);
		}
		x->setKeyAtIndex(newValue, lb);
		x->incrementNumKeys();
		for(int i=x->getNumKeys(); i>lb; i--){
			x->setIndexPointerAt(x->getIndexPointerAt(i), i+1);
		}
		x->setIndexPointerAt(temp, lb+1);
		if(x->getNumKeys()==numIndexPointers){
			// cout<<"chalta hai ye"<<endl;
			// cout<<toDataNode(temp)->getKeyAtIndex(0)<<endl;
			IndexNode* newNode = new IndexNode(numIndexPointers);
			newValue = x->getKeyAtIndex((numIndexPointers)/2);
			for(int i=(numIndexPointers)/2+1; i<numIndexPointers; i++){
				newNode->insertAtEnd(x->getKeyAtIndex(i));
			}
			for(int i=(numIndexPointers)/2+1; i<=numIndexPointers; i++){
				newNode->setIndexPointerAt(x->getIndexPointerAt(i), i-((numIndexPointers)/2+1));
			}
			x->setNumKeys((numIndexPointers)/2);
			// x->printNode();
			if(depth==0){
				IndexNode* newRoot = new IndexNode(numIndexPointers);
				newRoot->setIndexPointerAt(x, 0);
				newRoot->setIndexPointerAt(newNode, 1);
				newRoot->insertAtEnd(newValue);
				this->root = newRoot;
				this->depth++;
				return newRoot;
			}
			return newNode;
		}
	}
	return NULL;
}

void BPlusTree::LevelOrderTraversal(void *root, int depth){
	queue<pair<int,void *> > q;
	queue<int> level;
	int nodeID = 1;
	pair<int, void*> curNode;
	int curLevel=0,prevLevel=-1;
	depth = depth + 1;
	q.push({nodeID, root});
	level.push(0);
	cout<<"\n*** NodeID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		if(prevLevel!=level.front()){
			depth = depth-1;
			prevLevel = curLevel++;
		}
		level.pop();
		cout<<curNode.first<< " :: ";
		if(depth==0){
			DataNode *node = toDataNode(curNode.second);
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
		else{
			IndexNode *node = toIndexNode(curNode.second);
			for(int i=0; i <= node->getNumKeys(); i++){
				q.push({++nodeID, node->getIndexPointerAt(i)});
				level.push(curLevel);
			}
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
	}
	cout<<endl<<endl;
}