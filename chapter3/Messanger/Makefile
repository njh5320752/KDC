CFLAGS := -fPIC -O3 -g -Wall
CC := gcc
NAME := DBLinkedList
SERVER := server
CLIENT := client

all: $(SERVER) $(CLIENT)

$(SERVER): lib$(NAME).so $(SERVER).o
	$(CC) $(SERVER).c -o $@ -L. -l$(NAME)

$(CLIENT): $(CLIENT).o
	$(CC) $(CLIENT).c -o $@

lib$(NAME).so:$(NAME).o
	$(CC) -shared $^ -o $@

clean:
	$(RM) $(SERVER) *.o *.so*
