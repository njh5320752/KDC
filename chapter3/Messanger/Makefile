.SUFFIXES: .c.o

DBLINKED_LIST = DBLinkedList

SERVER = server

CLIENT = client

DBLINKED_LIST_OBJECT = DBLinkedList.o

SERVER_OBJECT = server.o

CLIENT_OBJECT = client.o

SHARED_LIBRARY = lib$(DBLINKED_LIST).so

CFLAGS += -Wall -g

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(SERVER) $(CLIENT)

$(SERVER): $(SHARED_LIBRARY) $(SERVER_OBJECT)
	$(CC) -L. -Wall -o $(SERVER) $(SERVER_OBJECT) -l$(DBLINKED_LIST)

$(CLIENT): $(CLIENT_OBJECT)
	$(CC) -o $(CLIENT) $(CLIENT_OBJECT)

$(SHARED_LIBRARY):$(DBLINKED_LIST_OBJECT)
	$(CC) -shared -o $@ $^

$(DBLINKED_LIST_OBJECT):
	$(CC) -c -Wall -fpic $(DBLINKED_LIST).c

clean:
	$(RM) $(SERVER) $(CLIENT) *.o *.so*
