#pragma once
#include "Common.h"

class DirectoryEntry {
public:
    int id;
    int length;
    int actLength;
    int start;
    bool valid;
    DirectoryEntry() {
        this->id = 0;
        this->length = 0;
        this->start = 0;
        this->valid = false;
    }
    DirectoryEntry(int id, int length, int start, bool valid) {
        this->id = id;
        this->length = length;
        this->actLength = length;
        this->start = start;
        this->valid = valid;
    }
};

class Page{

    public:
        vector<DirectoryEntry> arr;
        int spaceLeft;
        int dirSlotCount;

        //initializes an empty page with initial record entry
        Page(){
            this->spaceLeft = DISK_PAGE_SIZE - (sizeof(this->spaceLeft)+sizeof(this->dirSlotCount));
            //printf("%d", this->spaceLeft);
            this->dirSlotCount = this->arr.size();
        }
};