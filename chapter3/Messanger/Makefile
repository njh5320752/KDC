.SUFFIXES: .c.o

DBLINKED_LIST = DBLinkedList

SERVER_LOOPER = server_looper

CLIENT = client

DBLINKED_LIST_SOURCE = ../DBLinkedList/DBLinkedList.c

SERVER_SOURCE = server.c

INC = -I../DBLinkedList/


CM_MANAGER_OBJECT = cm_manager.o

SERVER_OBJECT = server.o

PACKET_OBJECT = packet.o

MESSAGE_OBJECT = message.o

DBLINKED_LIST_OBJECT = DBLinkedList.o

SERVER_LOOPER_OBJECT = \
			server.o\
			packet.o\
			fd_controler.o\
			server_looper.o\

CLIENT_OBJECT = client.o

SHARED_LIBRARY = lib$(DBLINKED_LIST).so

CFLAGS += -Wall -g

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(SERVER_LOOPER) $(CLIENT)

$(SERVER_LOOPER): $(SERVER_LOOPER_OBJECT)
	$(CC) -o $(SERVER_LOOPER) $(SERVER_LOOPER_OBJECT)

$(SERVER_OBJECT): $(DBLINKED_LIST_OBJECT)
	$(CC) -c $(CFLAGS) -o $(SERVER_OBJECT) $(SERVER_SOURCE)

$(CLIENT): $(CLIENT_OBJECT) $(CM_MANAGER_OBJECT) $(MSG_MANAGER)
	$(CC) -L. -Wall -o $(CLIENT) $(CLIENT_OBJECT) $(MSG_MANAGER) $(CM_MANAGER_OBJECT) -l$(DBLINKED_LIST)

$(SHARED_LIBRARY):$(DBLINKED_LIST_OBJECT)
	$(CC) -shared -o $@ $^

$(DBLINKED_LIST_OBJECT):
	$(CC) -c -Wall -fpic $(INC) $(DBLINKED_LIST_SOURCE)

clean:
	$(RM) $(SERVER) $(CLIENT) *.o *.so*
