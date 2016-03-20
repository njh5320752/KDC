main: StackMain.o Stack.o DBLinkedList.o
	gcc -o main StackMain.o Stack.o DBLinkedList.o

StackMain.o: Stack.h DBLinkedList.h DBLinkedList.c Stack.c StackMain.c
	gcc -c StackMain.c Stack.c DBLinkedList.c

Stack.o: Stack.h DBLinkedList.h Stack.c DBLinkedList.c
	gcc -c Stack.c DBLinkedList.c

DBLinkedList.o: DBLinkedList.h DBLinkedList.c
	gcc -c DBLinkedList.c

clean:
	rm -f main StackMain.o Stack.o DBLinkedList.o
