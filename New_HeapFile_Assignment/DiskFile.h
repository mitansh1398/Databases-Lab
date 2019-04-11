#pragma once
#include "Common.h"
#include "Page.h"

struct Node {
    Page data;
    struct Node* next;
    struct Node* prev;
};

class DiskFile{
    public:
        Node* nodePointer;
        list<Page> data;
        int totalPages;

        DiskFile(){ }

        DiskFile(int n, bool * create_diskFile){
            if(n<=0){
                printf("Number of pages should be grater than 0");
            }
            if(n * DISK_PAGE_SIZE > DISK_FILE_SIZE - (sizeof(nodePointer)+sizeof(totalPages))){
                printf("Number of Pages must not exhaust maximum allowable DiskFile size %d\n", DISK_PAGE_SIZE);
                *create_diskFile = false;
                return;
            }
            for(int i=0; i<n; i++){
                data.push_back(Page());
            }
            printf("Created %d number of pages of size %d bytes each in DiskFile...\n", n, DISK_PAGE_SIZE);
            printf("Printing created initial DiskFile...\n");
            printPages();
            this->totalPages = n;
            *create_diskFile = true;
        }

        void printPages();
        void insertRecord(int rec_id, int rec_length, bool * inserted_records);
        void deleteRecord(int rec_id);

};