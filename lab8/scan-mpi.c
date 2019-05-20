//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// MPI_Scan routine.
//
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int rank, size; 
  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  
  
  int x = rank + 1;
  int y;
  MPI_Scan(&x, &y, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("P%d: x = %d, y=%d\n", rank, x, y);

  MPI_Finalize();
}  
