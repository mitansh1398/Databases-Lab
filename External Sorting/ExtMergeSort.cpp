#include "common.h"
#include "ExtMergeSort.h"

void ExtMergeSort::firstPass(DiskFile &inputFile, MainMemory &memory){
	for(int i=0; i<inputFile.totalPages; i++){
		int frame = memory.loadPage(inputFile, i);
		sort(memory.data[frame].arr.begin(), memory.data[frame].arr.begin()+memory.data[frame].validEntries);
		memory.writeFrame(inputFile, frame, i);
		memory.freeFrame(frame);
	}
	this->runSize = 1;
	this->totalPass = 1;
	this->totalRuns = inputFile.totalPages;
	cout << "First Pass Performed" << endl;
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
			if(memory.data[sortedFrame].validEntries == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, sortedFrame, x);
				memory.data[sortedFrame].validEntries = 0;
                x++;
			}
			// cout<<"here6"<<endl;
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
        sortedRuns += runsToMerge;+
        numRuns++;
	}
    this->totalRuns = numRuns;
}

void ExtMergeSort::multiWaySort(DiskFile &inputFile, MainMemory &memory){
	this->firstPass(inputFile, memory);
	while(this->totalRuns > 1){
        // cout<<"asdf"<<endl;
		this->multiWayMerge(inputFile, memory);
		// cout<<"asd"<<endl;
        this->totalPass++;
        this->runSize = this->runSize * (memory.totalFrames-1);
		cout<<"Current runSize: "<<runSize<<endl;
		cout<<"Number of Runs: "<<this->totalRuns<<endl;
		cout<<"Pass "<<totalPass<<" performed"<<endl<<"Current Disk File: "<<endl;
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
		// cout<<"here1"<<endl;
		int sortedFrame = memory.getEmptyFrame();
        memory.valid[sortedFrame] = 1;
		// cout<<"here3"<<endl;
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
		// cout<<"here2"<<endl;
		DiskFile tempFile(runsToMerge*this->runSize);
        int x = 0;
		while(!pageHeap.empty()){
			// cout<<"here4"<<endl;
			pair<int, int> temp = pageHeap.top();
			// cout<<"value: "<<temp.first<<endl;
			pageHeap.pop();
			// cout<<"here4"<<endl;
			memory.setVal(sortedFrame, memory.data[sortedFrame].validEntries, temp.first);
			// cout<<"here5"<<endl;
			if(memory.data[sortedFrame].validEntries == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, sortedFrame, x);
				memory.data[sortedFrame].validEntries = 0;
                x++;
			}
			// cout<<"here6"<<endl;
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
			// cout<<"here7"<<endl;	

            if(memory.valid[frameNumber[temp.second]]){
				// cout<<"newVal: "<<memory.getVal(frameNumber[temp.second], recordNumber[temp.second])<<endl;
                pageHeap.push({memory.getVal(frameNumber[temp.second], recordNumber[temp.second]), temp.second});
                recordNumber[temp.second]++;
            }
			// cout<<"here8"<<endl;
			// cout<<min(sortedRuns*runSize, inputFile.totalPages-1)<<" "<<min((sortedRuns+runsToMerge)*runSize, inputFile.totalPages-1)<<endl;
			// cout<<"here9"<<endl;

		}
		if(memory.getValidEntries(sortedFrame)>0){
			memory.writeFrame(tempFile, sortedFrame, x);
		}
		for(int i=0; i<memory.totalFrames; i++){
			memory.freeFrame(i);
		}
		// cout<<"Writing temporary DiskFile:"<<endl;
        // tempFile.writeDiskFile();
		inputFile.DiskFileCopy(tempFile, min(sortedRuns*runSize, inputFile.totalPages-1), min((sortedRuns+runsToMerge)*runSize, inputFile.totalPages-1));
        sortedRuns += runsToMerge;
        numRuns++;
		// tempFile.~DiskFile();
	}
    this->totalRuns = numRuns;
}

void ExtMergeSort::DBmultiWaySort(DiskFile &inputFile, MainMemory &memory){
	this->firstPass(inputFile, memory);
	while(this->totalRuns > 1){
        // cout<<"asdf"<<endl;
		this->DBmultiWayMerge(inputFile, memory);
		// cout<<"asd"<<endl;
        this->totalPass++;
		if(this->totalPass==2){
			this->runSize = this->runSize * (memory.totalFrames-1);
		} else {
        	this->runSize = this->runSize * ((memory.totalFrames-1)/2);
		}
		cout<<"Current runSize: "<<runSize<<endl;
		cout<<"Number of Runs: "<<this->totalRuns<<endl;
		cout<<"Pass "<<totalPass<<" performed"<<endl<<"Current Disk File: "<<endl;
		inputFile.writeDiskFile();
	}
}