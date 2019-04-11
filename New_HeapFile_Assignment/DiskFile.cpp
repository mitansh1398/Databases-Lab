#include "DiskFile.h"
#include "Page.h"

/* Given a reference (pointer to pointer) to the head
of a list and an int, appends a new node at the end  */

/* Given a reference (pointer to pointer) to the head
of a list and a record details, it inserts the record in the DiskFIle  */
void DiskFile :: insertRecord(int rec_id, int rec_length, bool * inserted_records)
{
    if(rec_length > (DISK_PAGE_SIZE - (2*sizeof(int)))){
        printf("Record length should be less than effective allowable Page size %d, where the maximum Page size is %d\n", (int)(DISK_PAGE_SIZE - (2*sizeof(int)+DIR_ENTRY_LENGTH)), DISK_PAGE_SIZE);
        *inserted_records = false;
        return;
    }

    int count = 0;

    bool insert_success = false;

    for(auto last = this->data.begin(); last!=this->data.end(); last++){
        count = count+1;
        /*If Page is totally empty*/
        if(last->arr.empty()){
            last->arr.push_back(DirectoryEntry(rec_id, rec_length, 0, true));
            last->spaceLeft -= (rec_length + DIR_ENTRY_LENGTH);
            insert_success = true;
            printf("Record inserted in Page : %d \n", count);
            return;
        } else {
            for(auto it = last->arr.begin(); it!=last->arr.end(); it++){
                if(it->valid == false){
                    if(it->actLength >= rec_length){
                        it->id = rec_id;
                        it->length = rec_length;
                        it->valid = true;
                        insert_success = true;
                        printf("Record inserted in an empty slot of Page : %d \n", count);
                        return;
                    }
                }
            }
            if(!insert_success){
                if(last->spaceLeft >= (DIR_ENTRY_LENGTH+rec_length)){
                    int next_start = (last->arr.end()-1)->start + (last->arr.end()-1)->actLength;
                    last->arr.push_back(DirectoryEntry(rec_id, rec_length, next_start, true));
                    last->spaceLeft -= (rec_length + DIR_ENTRY_LENGTH);
                    printf("Record inserted in Page : %d \n", count);
                    insert_success = true;
                    return;
                } else if(this->totalPages == count){
                    if((this->totalPages + 1) * DISK_PAGE_SIZE <=  (DISK_FILE_SIZE  - (sizeof(this->nodePointer)+sizeof(this->totalPages)))){
                        printf("Exhausted space in Page %d. Page %d is created to insert this new Record!\n", count, count+1);
                        this->data.push_back(Page());
                        this->totalPages = this->totalPages + 1;
                        continue;
                    }
                    else{
                        printf("Exhausted effective allowable DiskFile size %d, where the maximum DiskFile size is %d\n", (DISK_FILE_SIZE  - (int)(sizeof(this->nodePointer)+sizeof(this->totalPages))), DISK_FILE_SIZE);
                        return;
                    }
                    
                }
            }
        }
        printf("Exhausted space in Page %d. Moving on to Page %d to insert Record...\n", count, count+1);
    }
    return;
}

/* Given a reference pointer to the head
of a list and an Record ID, it deletes the corresponding record in a Page  */
void DiskFile :: deleteRecord(int rec_id)
{
    bool flag = false; /* To check for duplicate entries. Currently deletes all
    duplicate entries by traversing all the pages. */
    int count = 0;
    for(auto last = this->data.begin(); last!=this->data.end(); last++){
        count = count + 1;
        for(int i=0; i<last->arr.size(); i++){
            if(last->arr[i].id == rec_id && last->arr[i].valid == true){
                last->arr[i].id = 0;
                last->arr[i].valid = false;
                last->arr[i].actLength = last->arr[i].length;
                printf("Record deleted from Page : %d \n", count);
                flag = true;
                /* No return after one deletion. After one entry deletion it checks
                for all the duplicate entries matching IDs and deletes them. */
            }
        }
    }

    if(flag==false){ // If not a single deletion has happened
        printf("Sorry the entered Record ID does not exist in any of the Pages...\n");
    }
    return;
}

/* This function prints contents of DiskFIle starting in terms of Pages & Records */
void DiskFile :: printPages()
{
    printf("DiskFile traversal, displaying Pages with Records <ID,Start,Length-Gap,Valid> :\n");
    int count = 0;
    for(auto node = this->data.begin(); node!=this->data.end(); node++){
        count = count+1;
        printf("Page:%d  Records{", count);
        for(int i=0; i<node->arr.size(); i++){
            int gap = node->arr[i].actLength - node->arr[i].length;
            if(i!=node->arr.size()-1)
                printf("<%d,%d,%d-%d,%s> ", node->arr[i].id, node->arr[i].start, node->arr[i].length, gap, node->arr[i].valid ? "true" : "false");
            else
                printf("<%d,%d,%d-%d,%s>", node->arr[i].id, node->arr[i].start, node->arr[i].length, gap, node->arr[i].valid ? "true" : "false");
        }
        printf("}\n");
    }
}
