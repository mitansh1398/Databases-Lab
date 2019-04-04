#include "common.h"
#include "ExtMergeSort.h"

void ExtMergeSort::firstPass(DiskFile &inputFile, MainMemory &memory){
	int lastPage = 0;
	for(int i=0; i<inputFile.totalPages; i+=memory.totalFrames){
		vector<int> sortPage;
		for(int j=0; j+i<inputFile.totalPages&&j<memory.totalFrames; j++){
			// cout<<"numEntries :"<<inputFile.data[j+i].validEntries<<endl;
			for(int l=0; l<inputFile.data[j+i].validEntries; l++){
				sortPage.push_back(inputFile.data[j+i].arr[l]);
				// cout<<inputFile.data[j+i].arr[l]<<endl;
			}
		}
		int y = 0;
		sort(sortPage.begin(), sortPage.end());
		for(int j=0; j+i<inputFile.totalPages&&j<memory.totalFrames; j++){
			for(int l=0; l<inputFile.data[j+i].validEntries; l++){
				inputFile.data[j+i].arr[l] = sortPage[y];
				// cout<<inputFile.data[j+i].arr[l]<<endl;
				y++;
			}
		}
	}
	this->runSize = memory.totalFrames;
	this->totalPass = 1;
	if(inputFile.totalPages%this->runSize){
		this->totalRuns = inputFile.totalPages/this->runSize+1;
	} else {
		this->totalRuns = inputFile.totalPages/this->runSize;
	}
	cout << "==========Pass 1 Performed==========" << endl;
	cout<<"Current runSize: "<<runSize<<endl;
	cout<<"Number of Runs: "<<this->totalRuns<<endl;
	cout<<"Current Disk File: "<<endl;
	inputFile.writeDiskFile();
}

void ExtMergeSort::multiWayMerge(DiskFile &inputFile, MainMemory &memory){
	
	int sortedRuns = 0;
    int numRuns=0;
	while(sortedRuns < this->totalRuns){
		int runsToMerge = 0;
		priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pageHeap;
		vector<int> pageNumber(memory.totalFrames, 1);
		vector<int> frameNumber(memory.totalFrames, 0);
		vector<int> recordNumber(memory.totalFrames, 1);
		int sortedFrame = memory.getEmptyFrame();
        memory.valid[sortedFrame] = 1;
		for(int i=sortedRuns*runSize; i<inputFile.totalPages; i+=this->runSize){
			int z = memory.loadPage(inputFile, i);
			if(z==-1){
				break;
			}
			pageHeap.push({memory.getVal(z, 0), runsToMerge});
			frameNumber[runsToMerge] = z;
			runsToMerge++;
		}
		DiskFile tempFile(runsToMerge*this->runSize);
        int x = 0;
		while(!pageHeap.empty()){
			pair<int, int> temp = pageHeap.top();
			pageHeap.pop();
			memory.setVal(sortedFrame, memory.data[sortedFrame].validEntries, temp.first);
			// cout<<temp.first<<endl;
			// memory.data[sortedFrame].validEntries++;
			if(memory.data[sortedFrame].validEntries == MEM_FRAME_SIZE){
				// cout<<"Out frame full"<<endl;
				memory.writeFrame(tempFile, sortedFrame, x);
				memory.data[sortedFrame].validEntries = 0;
                x++;
			}
			if(recordNumber[temp.second]==memory.data[frameNumber[temp.second]].validEntries){
                int newPage = (sortedRuns+temp.second)*runSize+pageNumber[temp.second];
				memory.freeFrame(frameNumber[temp.second]);
				if( newPage < inputFile.totalPages && pageNumber[temp.second] < this->runSize){
					frameNumber[temp.second] = memory.loadPage(inputFile, newPage);
					pageNumber[temp.second]++;
                    recordNumber[temp.second] = 0;
				}
			}

            if(memory.valid[frameNumber[temp.second]]){
                pageHeap.push({memory.getVal(frameNumber[temp.second], recordNumber[temp.second]), temp.second});
                recordNumber[temp.second]++;
            }

		}
		if(memory.getValidEntries(sortedFrame)>0){
			memory.writeFrame(tempFile, sortedFrame, x);
		}
		for(int i=0; i<memory.totalFrames; i++){
			memory.freeFrame(i);
		}
		inputFile.DiskFileCopy(tempFile, min(sortedRuns*runSize, inputFile.totalPages-1), min((sortedRuns+runsToMerge)*runSize, inputFile.totalPages-1));
        sortedRuns += runsToMerge;
        numRuns++;
	}
    this->totalRuns = numRuns;
}

void ExtMergeSort::multiWaySort(DiskFile &inputFile, MainMemory &memory){
	this->firstPass(inputFile, memory);
	while(this->totalRuns > 1){
		this->multiWayMerge(inputFile, memory);
        this->totalPass++;
        this->runSize = this->runSize * (memory.totalFrames-1);
		cout<<"==========Pass "<<totalPass<<" performed=========="<<endl;
		cout<<"Current runSize: "<<runSize<<endl;
		cout<<"Number of Runs: "<<this->totalRuns<<endl;
		cout<<"Current Disk File: "<<endl;
		inputFile.writeDiskFile();
	}
}

void ExtMergeSort::DBmultiWayMerge(DiskFile &inputFile, MainMemory &memory){
	
	int sortedRuns = 0;
    int numRuns=0;
	while(sortedRuns < this->totalRuns){
		int runsToMerge = 0;
		priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pageHeap;
		vector<int> pageNumber(memory.totalFrames, 1);
		vector<int> frameNumber(memory.totalFrames, 0);
		vector<int> bufferFrame(memory.totalFrames, -1);
		vector<int> recordNumber(memory.totalFrames, 1);
		int sortedFrame = memory.getEmptyFrame();
        memory.valid[sortedFrame] = 1;
		int sortedFrame2 = memory.getEmptyFrame();
        memory.valid[sortedFrame2] = 1;
		for(int i=sortedRuns*runSize; i<inputFile.totalPages; i+=this->runSize){
			int z = memory.loadPage(inputFile, i);
			if(z==-1){
				break;
			}
			pageHeap.push({memory.getVal(z, 0), runsToMerge});
			frameNumber[runsToMerge] = z;
			if(pageNumber[runsToMerge]<this->runSize&&(i+1)<inputFile.totalPages){
				bufferFrame[runsToMerge] = memory.loadPage(inputFile, i+1);
				pageNumber[runsToMerge]++;
			}
			runsToMerge++;
		}
		DiskFile tempFile(runsToMerge*this->runSize);
        int x = 0;
		while(!pageHeap.empty()){
			pair<int, int> temp = pageHeap.top();
			pageHeap.pop();
			memory.setVal(sortedFrame, memory.data[sortedFrame].validEntries, temp.first);
			if(memory.data[sortedFrame].validEntries == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, sortedFrame, x);
				memory.data[sortedFrame].validEntries = 0;
				memory.data[sortedFrame2].validEntries = 0;
				swap(sortedFrame, sortedFrame2);
                x++;
			}
			if(recordNumber[temp.second]==memory.getValidEntries(frameNumber[temp.second])){
                int newPage = (sortedRuns+temp.second)*runSize+pageNumber[temp.second];
				memory.freeFrame(frameNumber[temp.second]);
				if(bufferFrame[temp.second]!=-1){
					frameNumber[temp.second] = bufferFrame[temp.second];
					recordNumber[temp.second] = 0;
				}
				if( newPage < inputFile.totalPages && pageNumber[temp.second] < this->runSize){
					bufferFrame[temp.second] = memory.loadPage(inputFile, newPage);
					pageNumber[temp.second]++;
				} else {
					bufferFrame[temp.second] = -1;
				}
			}

            if(memory.valid[frameNumber[temp.second]]){
                pageHeap.push({memory.getVal(frameNumber[temp.second], recordNumber[temp.second]), temp.second});
                recordNumber[temp.second]++;
            }

		}
		if(memory.getValidEntries(sortedFrame)>0){
			memory.writeFrame(tempFile, sortedFrame, x);
		}
		for(int i=0; i<memory.totalFrames; i++){
			memory.freeFrame(i);
		}
		inputFile.DiskFileCopy(tempFile, min(sortedRuns*runSize, inputFile.totalPages-1), min((sortedRuns+runsToMerge)*runSize, inputFile.totalPages-1));
        sortedRuns += runsToMerge;
        numRuns++;
		// delete &tempFile;
	}
    this->totalRuns = numRuns;
}

void ExtMergeSort::DBmultiWaySort(DiskFile &inputFile, MainMemory &memory){
	cout<<"######### Double Buffering in action #########"<<endl;
	this->firstPass(inputFile, memory);
	while(this->totalRuns > 1){
		this->DBmultiWayMerge(inputFile, memory);
        this->totalPass++;
		if(this->totalPass==1){
			this->runSize = this->runSize * (memory.totalFrames-1);
		} else {
        	this->runSize = this->runSize * ((memory.totalFrames-1)/2);
		}
		cout<<"==========Pass "<<totalPass<<" performed=========="<<endl;
		cout<<"Current runSize: "<<runSize<<endl;
		cout<<"Number of Runs: "<<this->totalRuns<<endl;
		cout<<"Current Disk File: "<<endl;
		inputFile.writeDiskFile();
	}
}