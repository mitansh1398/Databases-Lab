#include "LinearHashing.cpp"

int main(){
	
	int choice;
	LinearHashing *d=NULL;
	do{
		cout<<"1. Create new LinearHashTable"<<endl;
		cout<<"2. Add an element to the HashTable"<<endl;
		cout<<"3. Search an element to the HashTable"<<endl;
		cout<<"4. Delete an element from the HashTable"<<endl;
		cout<<"5. Print HashTable"<<endl;

		cout<<"Any other choice to exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		cout<<"You entered "<<choice<<endl;
		switch(choice){
			case 1:
				if(d!=NULL)
					delete d;
				d= new LinearHashing();
				int n;
				cout<<"Enter number of buckets: ";
				cin>>n;
				int bucketSize;
				cout<<"Enter number of entries in bucket: ";
				cin>>bucketSize;
				d->createHashTable(n, bucketSize);
				break;
			case 2:
				int add;
				cout<<"Enter element to add: ";
				cin>>add;
				d->insertElement(add);
				break;
			case 3:
				int search;
				cout<<"Enter element to search: ";
				cin>>search;
				if(d->searchElement(search))
					cout<<"Element is found"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 4:
				int del;
				cout<<"Enter element to delete: ";
				cin>>del;
				cout<<"Functionality not supported"<<endl;
			case 5:
				d->printHashTable();
				break;
			case -1:
				return 0;
			default:
				cout<<"Invalid Choice"<<endl;
				continue;
		}
	}while(true);
}