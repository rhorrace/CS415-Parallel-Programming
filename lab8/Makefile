#----------------------------------------------------------------------
# Makefile for CS415/515 Lab
#----------------------------------------------------------------------

%: %.c
	gcc -o $@ $< -g -lm

%-mpi: %-mpi.c
	mpicc -o $@ $< -g -lm

%: %.chpl
	chpl -o $@ $<

