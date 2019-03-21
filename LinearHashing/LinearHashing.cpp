#include <bits/stdc++.h>
using namespace std;

class Bucket{
    public:
    list<int> bucket;
};

class LinearHashing{
    vector<Bucket*> dir;
    int next;
    int level;
    int n;
    int bucketSize;

    public:
    void createHashTable(int n, int bucketSize){
        this->level = 0;
        this->n = n;
        this->dir.resize(n);
        this->bucketSize = bucketSize;
        for(int i=0; i<n; i++){
            this->dir[i] = new Bucket();
        }
        next = 0;
    }

    int getBucketIndex(int val){
        int h0 = (n<<(this->level)) - 1;
        int indexVal = h0&val;
        if(indexVal < next){
            int h1 = 2*h0+1;
            int indexVal = h1&val;
        }
        return indexVal;
    }

    bool isBucketOverflow(int bucketIndex){
        if(this->dir[bucketIndex]->bucket.size()%this->bucketSize || this->dir[bucketIndex]->bucket.size()==0){
            return 0;
        }
        return 1; 
    }

    void splitBucket(){
        this->dir.push_back(new Bucket());
        list<int> &firstBucket = this->dir[next]->bucket;
        list<int> &secondBucket = this->dir[this->dir.size()-1]->bucket;
        for(auto it = firstBucket.begin(); it!=firstBucket.end();){
            if((n<<(this->level))&*it){
                secondBucket.push_back(*it);
                it = firstBucket.erase(it);
            } else {
                it++;
            }
        }
    }

    void insertElement(int val){
        int bucketIndex = this->getBucketIndex(val);
        if(this->isBucketOverflow(bucketIndex)){
            this->splitBucket();
            if(bucketIndex <= next){
                this->dir[(((n<<(this->level+1))-1)&val)]->bucket.push_back(val);
            } else {
                this->dir[bucketIndex]->bucket.push_back(val);
            }
            if(this->next == ((n<<this->level)-1)){
                this->next = 0;
                this->level++;
            } else {
                this->next++;
            }
            
        } else {
            this->dir[bucketIndex]->bucket.push_back(val);
        }
        cout<<"Insered into "<<bucketIndex<<endl;
    }

    void printHashTable(){
        cout<<"level = "<<this->level<<endl;
        cout<<"INDEX | ELEMENTS"<<endl;
        int index=0;
        for(auto it = this->dir.begin(); it!=this->dir.end(); it++){
            if(next==index){
                cout<<"next => ";
            }
            cout<<index<<" >> ";
            for(auto it2:(*it)->bucket){
                cout<<it2<<" ";
                cout<<"|";
            }
            cout<<endl;
            index++;
        }
    }

    bool searchElement(int val){
        int bucketIndex = getBucketIndex(val);
        for(auto it:(this->dir[bucketIndex])->bucket){
            if(it==val){
                return true;
            }
        }
        return false;
    }
};