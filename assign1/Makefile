#----------------------------------------------------------------------
# Makefile for CS415/515 Assignment 1
#----------------------------------------------------------------------

FLAGS = -std=c99 -pthread -g
CC = gcc

%: task-queue.h task-queue.c %.c 
	$(CC) $(FLAGS) -o $@ task-queue.c $@.c -lm

prodcons-pthd: prodcons-pthd.c

prime-pthd: prime-pthd.c

prime: prime.c
	$(CC) -o prime prime.c -lm

clean:
	rm prodcons-pthd prime-pthd
