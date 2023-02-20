#Compiler
CC = gcc

#Compiler flags
#Release
#CFLAGS = -O3 -Wall -I .

#Debug
CFLAGS = -g -Wall -Wextra -I .

#Library flags
LDFLAGS = -lpthread -lcurl -lncurses

all: clean main.out run

main.out: main.c
	$(CC) $(CFLAGS) -o main.out main.c $(LDFLAGS)

clean:
	clear
	rm -f *.o main.out *~

run:
	./main.out

valgrind:
	valgrind --leak-check=full ./main.out

memory:
	valgrind --tool=massif --massif-out-file=massif.out.12345 ./main.out 
	ms_print massif.out.12345 > memory.txt
	rm -f massif.out.12345
	