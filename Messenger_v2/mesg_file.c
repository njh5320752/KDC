#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mesg_file.h"
#include "socket.h"

struct _Mesg_File {
    int fd;
};

Mesg_File* new_mesg_file() {
    Mesg_File *mesg_file;
    char *homedir;
    char *message_path;
    int home_pathlen;
    int message_strlen;
    int message_pathlen;
    int file_fd;

    homedir = getenv("HOME");

    home_pathlen = strlen(homedir);
    message_strlen = strlen(MESSAGE_STORE);

    message_pathlen = home_pathlen + message_strlen;

    message_path = (char*) malloc(message_pathlen + 1);
    memset(message_path, 0, message_pathlen + 1);

    strncpy(message_path, homedir, home_pathlen);
    strncpy(message_path + home_pathlen, MESSAGE_STORE, message_pathlen);

    printf("message_path:%s\n", message_path);

    file_fd = open(message_path, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (file_fd == -1) {
        printf("Failed to make message store\n");
        return NULL;
    }

    mesg_file = (Mesg_File*) malloc(sizeof(Mesg_File));
    if (!mesg_file) {
        printf("Failed to make Mesg_File\n");
        return NULL;
    }

    mesg_file->fd = file_fd;

    return mesg_file;
}

void destroy_mesg_file(Mesg_File* mesg_file) {
    if (!mesg_file) {
        printf("There is nothing to pointer the Mesg_File\n");
        return;
    }

    free(mesg_file);
}
