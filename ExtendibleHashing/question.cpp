#include "ExtendibleHashing.h"


void ExtendibleHashing::createHashTable(int globalDepth,int bucketSize){
	this->globalDepth = globalDepth;
	this->bucketSize = bucketSize;
	int numBuckets = twoPower(globalDepth);
	this->dir.resize(numBuckets);
	for(int i=0; i<numBuckets; i++){
		this->dir[i] = new Bucket;
		this->dir[i]->localDepth = globalDepth;
	}
}

int ExtendibleHashing::getBucketIndex(int val){
	int allOnes = twoPower(this->globalDepth) - 1;
	return allOnes&val;
}

void ExtendibleHashing::insertElement(int val){
	if(searchElement(val)){
		cout<<"can't insert duplicate element"<<endl;
		return;
	}
	int bucketIndex = getBucketIndex(val);
	while(this->dir[bucketIndex]->bucket.size()==this->bucketSize){
		this->splitBucket(bucketIndex&(twoPower(dir[bucketIndex]->localDepth)-1));
		bucketIndex = getBucketIndex(val);
	}
	bucketIndex = getBucketIndex(val);
	this->dir[bucketIndex]->bucket.push_back(val);
}

bool ExtendibleHashing::searchElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto it:(this->dir[bucketIndex])->bucket){
		if(it==val){
			return true;
		}
	}
	return false;
}

bool ExtendibleHashing::deleteElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto itr = this->dir[bucketIndex]->bucket.begin(); itr!=this->dir[bucketIndex]->bucket.end(); itr++){
		if(*itr==val){
			this->dir[bucketIndex]->bucket.erase(itr);
			return true;
		}
	}
	return false;
}
void ExtendibleHashing::doubleHashTable(){
	int globalDepthPower = twoPower(this->globalDepth);
	this->dir.resize(2*globalDepthPower);
	for(int i=0; i<globalDepthPower; i++){
		this->dir[i+globalDepthPower] = this->dir[i];
	}
	this->globalDepth++;
}

void ExtendibleHashing::splitBucket(int bucketIndex){
	if(this->globalDepth==this->dir[bucketIndex]->localDepth){
		this->doubleHashTable();	
	}
	// printHashTable();
	int localDepthPower = twoPower(this->dir[bucketIndex]->localDepth);
	// printHashTable();
	int firstBucketIndex = bucketIndex&(localDepthPower-1);
	// printHashTable();
	int secondBucketIndex = bucketIndex + localDepthPower;
	// printHashTable();
	// cout<<bucketIndex<<endl;
	// cout<<localDepthPower<<endl;
	// cout<<secondBucketIndex<<endl;
	this->dir[secondBucketIndex] = new Bucket();
	// printHashTable();
	this->dir[firstBucketIndex]->localDepth++;
	// printHashTable();
	this->dir[secondBucketIndex]->localDepth =  this->dir[firstBucketIndex]->localDepth;
	// cout<<"1"<<endl;
	// printHashTable();
	// cout<<"2"<<endl;
	for(auto it = this->dir[firstBucketIndex]->bucket.begin(); it!=(this->dir[firstBucketIndex]->bucket).end();){
		// cout<<"it's in"<<endl;
		// printHashTable();
		if((*it)&localDepthPower){
			this->dir[secondBucketIndex]->bucket.push_back(*it);
			it = this->dir[firstBucketIndex]->bucket.erase(it);
		} else {
			it++;
		}
	}
	// cout<<"it's out"<<endl;
	// printHashTable();
	localDepthPower *= 2;
	for(int i=secondBucketIndex+localDepthPower; i<(this->dir.size()); i+=localDepthPower){
		// printHashTable();
		this->dir[i] = this->dir[localDepthPower];
	}
	// printHashTable();
}

int ExtendibleHashing::twoPower(int p){
	return (1<<p);
}

void ExtendibleHashing::printHashTable(){
	cout<<"\n ***** Hash Table ***** \n"<<endl;
	cout<<"globalDepth :: " << this->globalDepth<<endl;
	unordered_set<Bucket*> uset;
	cout<<"localDepth ::| Elements |"<<endl;
	for(int i=0; i<this->dir.size(); i++){
		if(uset.find(this->dir[i])!=uset.end())
			continue;
		cout<<i<<" :: | ";
		cout<<this->dir[i]->localDepth<<" :: | ";
		uset.insert(this->dir[i]);
		for(auto l:this->dir[i]->bucket){
			cout<<l<<" ";
		}
		cout<<"|" <<endl;
	}
	cout<<endl;
}
