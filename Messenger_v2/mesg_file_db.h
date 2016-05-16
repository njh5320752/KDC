#ifndef __MESG_FILE_DB_H__
#define __MESG_FILE_DB_H__

typedef struct _Mesg_File_DB Mesg_File_DB;

Mesg_File_DB* new_mesg_file_db();
void destroy_mesg_file_db(Mesg_File_DB *mesg_file_db);

#endif
