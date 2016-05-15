.SUFFIXES: .c.o

DBLINKED_LIST = DBLinkedList

SERVER_MAIN = server_main

DBLINKED_LIST_SOURCE = ../chapter3/DBLinkedList/DBLinkedList.c

INC = -I../chapter3/DBLinkedList/

DBLINKED_LIST_OBJECT = DBLinkedList.o

SERVER_MAIN_OBJECT = \
					 server_main.o\
					 server.o\
					 file_db.o\

SHARED_LIBRARY = lib$(DBLINKED_LIST).so

CFLAGS += -Wall -g $(INC)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(SERVER_MAIN)

$(SERVER_MAIN) : $(SERVER_MAIN_OBJECT) $(SHARED_LIBRARY)
	$(CC) -o $(SERVER_MAIN) $(SERVER_MAIN_OBJECT) -L. -l$(DBLINKED_LIST)

$(SHARED_LIBRARY):$(DBLINKED_LIST_OBJECT)
	$(CC) -shared -o $@ $^

$(DBLINKED_LIST_OBJECT):
	$(CC) -c -Wall -fpic $(INC) $(DBLINKED_LIST_SOURCE)

clean:
	$(RM) $(SERVER_MAIN) *.o *.so
