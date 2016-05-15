#ifndef __SERVER_H__
#define __SERVER_H__

#include "looper.h"

typedef struct _Server Server;
typedef struct _Client Client;

Server* new_server(Looper *looper);
void destroy_server(Server *server);

#endif
