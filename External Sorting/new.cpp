#include "common.h"
#include "ExtMergeSort.h"

void ExtMergeSort::firstPass(DiskFile &inputFile, MainMemory &memory){
	for(int i=0; i<inputFile.totalPages; i++){
		int frame = memory.loadPage(inputFile, i);
		sort(memory.data[frame].arr.begin(), memory.data[frame].arr.end());
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
	vector<int> pageNumber(this->totalRuns, 0);
	this->totalPass++;
	int sortedRuns = 0;
	while(sortedRuns < this->totalRuns){
		int runsToMerge = 0;
		priority_queue<pair<int, int> > pageHeap;
		vector<int> frameNumber(memory.totalFrames-1, 0);
		vector<int> recordNumber(memory.totalFrames-1, 0);
		for(int i=0; i<inputFile.totalPages; i+=this->runSize){
			int z = memory.loadPage(inputFile, i);
			if(z==-1){
				break;
			}
			pageHeap.push({memory.getVal(z, 0), runsToMerge});
			frameNumber[runsToMerge] = z;
			runsToMerge++;
		}
		DiskFile tempFile(runsToMerge*this->runSize);
		while(!pageHeap.empty()){
			pair<int, int> temp = pageHeap.top();
			pageHeap.pop();
			memory.setVal(memory.totalFrames-1, memory.data[memory.totalFrames-1].validEntries, frameNumber[temp.first]);
			if(memory.data[memory.totalFrames-1].validEntries == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, memory.totalFrames-1, tempFile.totalPages);
				memory.freeFrame(memory.totalFrames-1);
			}
			if(recordNumber[temp.first]==memory.data[frameNumber[temp.first]].validEntries){
				if(sortedRuns*runSize+pageNumber[temp.first] < inputFile.totalPages){
					memory.data[frameNumber[temp.first]].fillFrame(inputFile.data[sortedRuns*runSize+pageNumber[temp.first]].arr);
					pageNumber[temp.first]++;
				}
			}
		}
	}
	
}

void ExtMergeSort::multiWaySort(DiskFile &inputFile, MainMemory &memory){
	this->firstPass(inputFile, memory);
	while(this->totalRuns > 1){
		this->multiWayMerge(inputFile, memory);
	}
}