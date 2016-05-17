#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "looper.h"

#define BUF_SIZE 3
#define REQUEST "command "
#define REQUEST_MIN_LEN 10

typedef struct _Client Client;

Client* new_client(Looper *looper);

#endif
