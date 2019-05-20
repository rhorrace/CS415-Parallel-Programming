//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// MPI scatter routines.
//
//
#include <stdio.h>
#include <mpi.h>

#define N 64

void print_array(char *msg, int *array, int n) {
  printf("%s", msg);
  for (int i = 0; i < n; i++) {
    printf("%4d ", array[i]);
    if (i % 10 == 9)
      printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv) {
  int rank, size; 
  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  
  
  int data[N];          // source data
  int result[N];        // dest result
  int scnt[size];       // send counts
  int disp[size];       // disp indices

  // process 0 is the source
  if (rank == 0) {
    // initialize data
    for (int i = 0; i < N; i++)
      data[i] = i;
    print_array("P0 data: ", data, N);

    // set up send count -- send k+1 numbers to process k
    for (int k = 0; k < size; k++)
      scnt[k] = k + 1;
    print_array("P0 scnt: ", scnt, size);

    // set up disp indices into data array
    disp[0] = 0;
    for (int k = 1; k < size; k++)
      disp[k] = scnt[k-1] + disp[k-1];
    print_array("P0 disp: ", disp, size);
  }

  // now for all processes
  int mycnt;
  // scatter the send counts to all processes
  MPI_Scatter(scnt, 1, MPI_INT, &mycnt, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("P%d: mycnt = %d\n", rank, mycnt);

  // scatter variable-sized data sections to all processes
  MPI_Scatterv(data, scnt, disp, MPI_INT, result, N, MPI_INT, 0, MPI_COMM_WORLD);
  char msg[32];
  sprintf(msg, "P%d got %d numbers: ", rank, mycnt);
  print_array(msg, result, mycnt);

  MPI_Finalize();
}  
