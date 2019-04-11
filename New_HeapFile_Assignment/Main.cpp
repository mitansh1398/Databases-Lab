#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "Common.h"
#include "Page.h"
#include "DiskFile.h"

int main()
{
    DiskFile d;
    bool create_diskFile = false;
    bool inserted_records = false;
    while(true){
        int x;
        printf("\nEnter :- 1:Create New DiskFile, 2:Insert Record, 3:Delete Record, 4:Display DiskFile Structure, -1:Exit\n");
        printf("Choice : ");
        cin >> x;
        switch (x)
        {
            case 1: 
                if(create_diskFile==false){
                    printf("Enter number of DataPages to create : ");
                    int n;
                    cin >> n;
                    
                    d = DiskFile(n, &create_diskFile);
                //    create_diskFile = true;
                }
                else{
                    printf("DiskFile has already been created.\n");
                }
                break;

            case 2: 
                if(create_diskFile){
                    printf("Enter Record identifier in int to insert into Pages : \n");
                    int id;
                    cin >> id;
                    printf("Enter Record length in bytes : \n");
                    int l;
                    cin >> l;
                    d.insertRecord(id,l,&inserted_records);
                }
                else{
                    printf("You can not enter Records into Pages without creating the initial DiskFile.\n");
                }
                break;

            case 3: 
                if(create_diskFile){
                    printf("Enter Record identifier in int to delete from Pages : \n");
                    int id;
                    cin >> id;
                    d.deleteRecord(id);
                }
                else{
                    printf("You can not delete Records without creating the initial DiskFile.\n");
                }   
                break;

            case 4: 
                if(create_diskFile){
                    d.printPages();
                }
                else{
                    printf("You can not view Pages & Records without creating the initial DiskFile.\n");
                }
                break;

            case -1: exit(0);

            default: printf("Choice other than 1, 2, 3, 4 and -1\n");
                continue;
        }
    }
}