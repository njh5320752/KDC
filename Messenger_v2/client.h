#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "looper.h"

typedef struct _Client Client;

Client* new_client(Looper *looper);

#endif
