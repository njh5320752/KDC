.SUFFIXES: .c.o

STACK = stack

QUEUE = queue

DBLINKED = dblinked

STACK_OBJECTS = \
		StackMain.o\
		Stack.o \
		DBLinkedList.o
DBLINKED_OBJECTS = \
		   DBLinkedListMain.o \
		   DBLinkedList.o
QUEUE_OBJECTS = \
		QueueMain.o\
		Queue.o\
		DBLinkedList.o

CFLAGS += -Wall -g

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(STACK) $(DBLINKED) $(QUEUE)

$(STACK): $(STACK_OBJECTS)
	$(CC) -o $(STACK) $(STACK_OBJECTS)

$(QUEUE): $(QUEUE_OBJECTS)
	$(CC) -o $(QUEUE) $(QUEUE_OBJECTS)

$(DBLINKED): $(DBLINKED_OBJECTS)
	$(CC) -o $(DBLINKED) $(DBLINKED_OBJECTS)

clean:
	rm -f *.o $(STACK) $(DBLINKED) $(QUEUE)
