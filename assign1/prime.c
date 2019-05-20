//------------------------------------------------------------------------- 
// This supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A sequetial prime-finding program.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Return 1 if k is a prime 
// 
int isPrime(int k) {
  int limit = (int) sqrt((double) k);
  if (k == 1) 
    return 0;
  for (int i = 2; i <= limit; i++)
    if ((k % i) == 0)
      return 0;
  return 1;
}

// Swap two array elements 
// 
void swap(int *array, int i, int j) {
  if (i == j) return;
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

// Initialize array.
// - first generate [1,2,...,ARRAYSIZE]
// - then perform a random permutation
//
int *init_array(int N) {
  int *array = (int *) malloc(sizeof(int) * N);
  int i, j;
  for (i = 0; i < N; i++) {
    array[i] = i + 1;
  }
  srand(time(NULL));
  for (i = 0; i < N; i++) {
    j = (rand() * 1. / RAND_MAX) * (N - 1);
    swap(array, i, j);
  }
  return array;
}

// The main routine
// 
int main(int argc, char **argv) {
  int *array, *result;
  int N, i, cnt;

  if (argc < 1) {
    printf ("Usage : ./prime N\n");
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    printf ("N must be greater than 0\n");
    exit(0);
  }

  array = init_array(N);
  result = (int *) malloc(sizeof(int) * N);
  cnt = 0;
  for (i = 0; i < N; i++) {
    if (isPrime(array[i]))
      result[cnt++] = array[i];
  }
  printf("Found %d primes in array[%d]:\n", cnt, N); 
  for (i = 0; i < cnt; i++)
    printf("%d, ", result[i]);
  printf("\n");
}
