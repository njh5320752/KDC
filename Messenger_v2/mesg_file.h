#ifndef __MESG_FILE_H__
#define __MESG_FILE_H__

typedef struct _Mesg_File Mesg_File;

Mesg_File* new_mesg_file();
void destroy_mesg_file(Mesg_File *mesg_file);

#endif

