//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple data generation engine.
// 
// - Generate N random int values in range [0,8191] (default: N=64)
//
// Usage: 
//   linux> ./datagen N > <outfile>	-- pipe the output to a file
//
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) 
{
  int i,N = 64;	// default value
  if (argc > 1)
    N = atol(argv[1]);

  srand(time(0));
  int array[N];
  for (i = 0; i < N; i++)
    array[i] = rand() % 8192;

  fwrite(array, sizeof(int), N, stdout);
}

