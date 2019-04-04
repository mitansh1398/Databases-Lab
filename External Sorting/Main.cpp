#include "common.h"
#include "MainMemory.h"
#include "DiskFile.h"
#include "ExtMergeSort.h"
int DISK_PAGE_SIZE;
int MEM_FRAME_SIZE; 
int main()
{
	cout<<"Enter Buffer Size: "<<endl;
	int x;
	//reads size of main memory in terms of number of frames available
	cin >> x;
	cout<<"Enter page and frame size: "<<endl;
	cin>>DISK_PAGE_SIZE;
	MEM_FRAME_SIZE = DISK_PAGE_SIZE;
	
	//create main memory of x frames
	MainMemory mm(x);

	cout<<"Do you want Double buffering?"<<endl;
	int db_flag;
	cin>>db_flag;
	//create a file by taking input from cin
	DiskFile f;
	f.readDiskFile();
	f.writeDiskFile();

	ExtMergeSort e;
	
	//call 2 way externalmerge sort

	if(db_flag){
		e.DBmultiWaySort(f,mm);
	} else {
		e.multiWaySort(f,mm);
	}
	

	//output file by using cout
	f.writeDiskFile(); 

}