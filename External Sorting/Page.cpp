#include "Page.h"
#include "common.h"


//writes page to cout
void Page :: writePage(){

	for(int i = 0; i < this->validEntries; i++){
		if(this->arr[i] == -1){
			break;
		}
		cout << this->arr[i] << " ";
	}
	cout << endl;
}

//fills page with contents from vector v
void Page :: fillPage(vector<int> &v){
	
	this->validEntries = 0;
	for(int i = 0; i < v.size() && i < DISK_PAGE_SIZE; i++){
		this->validEntries = i;
		if(v[i] == -1){
			for(int j=i; j<DISK_PAGE_SIZE; j++){
				this->arr[i] = -1;
			}
			break;
		}
		this->validEntries++;
		this->arr[i] = v[i];
	}
}
