#include "file_db.h"

struct _File_DB 
{
    int mesg_num;
};

File_DB* new_File_DB() {
    File_DB *file_db;

    file_db = (File_DB*) malloc(sizeof(File_DB));
    
    if (!file_db) {
        printf("Failed to make File_DB\n");
        return NULL;
    }
    
    return file_db;
}
