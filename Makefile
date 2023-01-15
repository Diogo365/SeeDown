#Compiler
CC = gcc

#Compiler flags
#Release
#CFLAGS = -02 -Wall -I .

#Debug
CFLAGS = -g -Wall -Wextra -I .

#Library flags
LDFLAGS = -lpthread -lcurl

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