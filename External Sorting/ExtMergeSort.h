#pragma once
#include "common.h"
#include "DiskFile.h"
#include "Page.h"
#include "Frame.h"
#include "MainMemory.h"

class ExtMergeSort{
public:
	int runSize; // size of run in terms of number of pages
	int totalPass; // number of passes performed
	int totalRuns;

	ExtMergeSort(){
		runSize = 0;
		totalPass = 0;
		totalRuns = -1;
	}

	void firstPass(DiskFile &inputFile, MainMemory &memory);
	void multiWaySort(DiskFile &inputFile, MainMemory &memory);
	void multiWayMerge(DiskFile &inputFile, MainMemory &memory);
	void DBmultiWayMerge(DiskFile &inputFile, MainMemory &memory);
	void DBmultiWaySort(DiskFile &inputFile, MainMemory &memory);
};