#include <stdlib.h>
#include <stdio.h>

#include "mesg_file_db.h"

struct _Mesg_File_DB 
{
    int mesg_num;
};

Mesg_File_DB* new_mesg_file_db() {
    Mesg_File_DB *mesg_file_db;

    mesg_file_db = (Mesg_File_DB*) malloc(sizeof(Mesg_File_DB));
    
    if (!mesg_file_db) {
        printf("Failed to make File_DB\n");
        return NULL;
    }
    
    return mesg_file_db;
}

void destroy_mesg_file_db(Mesg_File_DB *mesg_file_db) {
    if (!mesg_file_db) {
        printf("There is no a pointer to file_db\n");
        return;
    }

    free(mesg_file_db);
}
