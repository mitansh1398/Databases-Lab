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
	int bucketIndex = getBucketIndex(val);
	while(this->dir[bucketIndex]->bucket.size()==this->bucketSize){
		this->splitBucket(bucketIndex);
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

	int localDepthPower = twoPower(this->dir[bucketIndex]->localDepth);
	int firstBucketIndex = bucketIndex&(localDepthPower-1);
	int secondBucketindex = bucketIndex + localDepthPower;
	this->dir[secondBucketindex] = new Bucket;
	this->dir[firstBucketIndex]->localDepth++;
	this->dir[secondBucketindex]->localDepth =  this->dir[firstBucketIndex]->localDepth;
	for(auto it = this->dir[firstBucketIndex]->bucket.begin(); this->dir[firstBucketIndex]->bucket.end()!=it; it++){
		if((*it)&localDepthPower){
			this->dir[firstBucketIndex]->bucket.push_back(*it);
			this->dir[firstBucketIndex]->bucket.erase(it);
		}
	}
	localDepthPower <<= 1;
	for(int i=secondBucketindex+localDepthPower; i<(this->dir.size()); i+=localDepthPower){
		this->dir[i] = this->dir[localDepthPower];
	}

}

int ExtendibleHashing::twoPower(int p){
	return (1<<p);
}

void ExtendibleHashing::printHashTable(){
	
}
