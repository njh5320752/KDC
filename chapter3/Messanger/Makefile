.SUFFIXES: .c.o

DBLINKED_LIST = DBLinkedList

SERVER_MAIN = server_main

CLIENT_MAIN = client_main

DBLINKED_LIST_SOURCE = ../DBLinkedList/DBLinkedList.c

INC = -I../DBLinkedList/


CM_MANAGER_OBJECT = cm_manager.o


MESSAGE_OBJECT = message.o

DBLINKED_LIST_OBJECT = DBLinkedList.o

SERVER_MAIN_OBJECT = \
			server_controler.o\
			packet.o\
			fd_controler.o\
			server_main.o\
			message.o\

CLIENT_MAIN_OBJECT = \
		  	client_main.o\
		  	client_looper.o\
		  	client_controler.o\

SHARED_LIBRARY = lib$(DBLINKED_LIST).so

CFLAGS += -Wall -g $(INC)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(SERVER_MAIN) $(CLIENT)

$(SERVER_MAIN): $(SERVER_MAIN_OBJECT) $(SHARED_LIBRARY)
	$(CC) -o $(SERVER_MAIN) $(SERVER_MAIN_OBJECT) -L. -l$(DBLINKED_LIST)

$(CLIENT): $(CLIENT_OBJECT) $(CM_MANAGER_OBJECT) $(MSG_MANAGER)
	$(CC) -L. -Wall -o $(CLIENT) $(CLIENT_OBJECT) $(MSG_MANAGER) $(CM_MANAGER_OBJECT) -l$(DBLINKED_LIST)

$(SHARED_LIBRARY):$(DBLINKED_LIST_OBJECT)
	$(CC) -shared -o $@ $^

$(DBLINKED_LIST_OBJECT):
	$(CC) -c -Wall -fpic $(INC) $(DBLINKED_LIST_SOURCE)

clean:
	$(RM) $(SERVER_MAIN) $(CLIENT) *.o *.so*
