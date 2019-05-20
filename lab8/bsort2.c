//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Bucket Sort (Sequential version 1)
//
// Usage: bsort1 B [N]   -- use B buckets to sort N integers (N defaults to 16)
//
// Assumption: B is a power of 2 (so that bucket distribution can be based 
//             on leading bits)
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))

// Specify data value range by bit-width 
//
#define DATABITS 13     // assume data are 13-bit integers: [0,8191] 

// Swap two array elements 
//
void swap(int *array, int i, int j) {
  if (i == j) return;
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

// Bubble sort for the base cases
//
void bubblesort(int *array, int low, int high) {
  int i,j;
	if (low >= high) return;
  for (i = low; i <= high; i++)
    for (j = i+1; j <= high; j++) 
      if (array[i] > array[j])
				swap(array, i, j);
}

// Initialize array
//
void init_array(int *a, int n) {
 	int i;
	srand(time(NULL));
  for (i = 0; i < n; i++)
    a[i] = rand() % 8192;
}

// Verify that array elements are sorted
//
void verify_array(int *array, int N) {
  int i;
	for (i = 0; i < N-1; i++) {
    if (array[i]>array[i+1]) {
      printf("FAILED: array[%d]=%d, array[%d]=%d\n", 
	     i, array[i], i+1, array[i+1]);
      return;
    }
  }
  printf("Result verified!\n");
}

// Print array
//
void print_array(int *array, int N) 
{
	int i;
  for (i = 0; i < N; i++) {
    printf("%4d ", array[i]);
    if (i % 12 == 11)
      printf("\n");
  }
  printf("\n");
}

// Return 1 if x is a power of 2
//
int isPower2(int x) {
  return !(x & (x - 1));
}

// Find bucket idx for an int value x of b bits
//
int bktidx(int b, int x, int B) {
  return x >> (b - (int)log2(B));
}

// Bucket sort -- main routine
//
int main(int argc, char **argv) {
  FILE* fin = NULL,* fout = NULL;
	int i,k,size;
	if (argc < 2) {
    printf("Usage: bsort1 B [N]\n");
    exit(1);
  }

  // Get param B (numBkts) from command-line, verify it's a power of 2
  int B = atoi(argv[1]);
  if (!isPower2(B)) {
    printf("B (%d) must be a power of 2.\n", B);
    exit(1);
  }

  int N = 16;   // default
  // Get param N (array size) from command-line
  if (argc >= 3){
  	fin = fopen(argv[2],"r");
		fseek(fin, 0, SEEK_END);
		size = ftell(fin);
		rewind(fin);
		N = (int) size/sizeof(int);
	}
	printf("Size of N: %d\n",N);
  // Allocate array and read data into it
  int array[N];
	if(fin)
  	fread(&array,sizeof(size),N,(FILE*)fin);
	else
		init_array(array,N);
  printf("Init[%d] ", N);
  print_array(array, N);

  // Allocate buckets; with a safe bucket size
  int bsize = MAX(64, 2*N/B);
  int bucket[B][bsize];

  // Initialize all bucket counters to 0
  int bcnt[B];
  for (k = 0; k < B; k++)
    bcnt[k] = 0;

  // Distributing each integer to a bucket
  for (i = 0; i < N; i++) {
    k = bktidx(DATABITS, array[i], B);
    bucket[k][bcnt[k]++] = array[i];
  }
  for (k = 0; k < B; k++) {
    printf("Bucket%d[%d] ", k, bcnt[k]);
    print_array(bucket[k], bcnt[k]);
  }

  // Sort each bucket; copy result to result array
  int result[N];
  int rcnt = 0;
  for (k = 0; k < B; k++) {
    int *data = bucket[k]; 
    int cnt = bcnt[k];
    bubblesort(data, 0, cnt-1);
    for (i = 0; i < cnt; i++)
      result[rcnt++] = data[i];
  }
  printf("Result[%d] ", rcnt);
  print_array(result, rcnt);

  // Verify the result array
  if (rcnt != N) {
    printf("Result size (%d) differs from input size (%d)\n", rcnt, N);
    exit(1);
  }
  verify_array(result, rcnt);
	//fwrite(result,sizeof(int),rcnt,stdout);
	return 0;
}
