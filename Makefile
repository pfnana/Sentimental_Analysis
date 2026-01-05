CC = gcc
FLAGS = -W -Wall -Wextra -g
#SRC = SAfunctions.c  SA.c

all: mySA

mySA: SA.o 
	$(CC) $(FLAGS) -o $@ $^

SA.o: SA.c SAfunctions.h
	$(CC) $(FLAGS) -c $<


clean:
	rm -f mySA *.o

