.SUFFIXES: .c.o

STACK = stack

DBLINKED = dblinked

STACK_OBJECTS = \
		StackMain.o Stack.o \
		DBLinkedList.o
DBLINKED_OBJECTS = \
		   DBLinkedListMain.o \
		   DBLinkedList.o

CFLAGS += -Wall -g

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(STACK) $(DBLINKED)

$(STACK): $(STACK_OBJECTS)
	$(CC) -o $(STACK) $(STACK_OBJECTS)

$(DBLINKED): $(DBLINKED_OBJECTS)
	$(CC) -o $(DBLINKED) $(DBLINKED_OBJECTS)

clean:
	rm -f *.o $(STACK) $(DBLINKED)
