CFLAGS := -fPIC -O3 -g -Wall -Werror
CC := gcc
NAME := DBLinkedList
SERVER := server


server: lib$(NAME).so
	$(CC) $(SERVER).c -o $@ -L. -l$(NAME)

lib$(NAME).so:$(NAME).o
	$(CC) -shared $^ -o $@

clean:
	$(RM) $(SERVER) *.o *.so*
