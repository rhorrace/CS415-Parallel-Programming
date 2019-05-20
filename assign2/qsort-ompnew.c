//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A sequential quicksort program.
//
// Usage: ./qsort N
// 
//

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MINSIZE   10 		// threshold for switching to bubblesort

//for threads
int numThreads = 1;
int *threads = NULL;

// Swap two array elements 
//
void swap(int *array, int i, int j) {
  if (i == j) return;
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

// Initialize array.
// - first generate [1,2,...,N]
// - then perform a random permutation
//
int *init_array(int N) {
  int *array = (int *) malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++) {
    array[i] = i + 1;
  }
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    int j = rand() % (N-1) + 1;
    swap(array, i, j);
  }
  printf("Initialized array to a random permutation of [1..%d]\n", N);
  return array;
}

// Verify that array is sorted
//
void verify_array(int *array, int N) {
  for (int i = 0; i < N-1; i++)
    if (array[i] > array[i+1]) {
      printf("FAILED: array[%d]=%d, array[%d]=%d\n", 
	     i, array[i], i+1, array[i+1]);
      return;
    }
  printf("Result verified!\n");
}

// Bubble sort for the base cases
//
void bubblesort(int *array, int low, int high, int tid) {
  if (low >= high) 
    return;
  for (int i = low; i <= high; i++)
    for (int j = i+1; j <= high; j++) 
      if (array[i] > array[j]){
				swap(array, i, j);
				threads[tid] += 2;
			}
	printf("Thread %d sorted [%d,%d]\n",tid,low,high);
}

// Pick an arbitrary element as pivot. Rearrange array 
// elements into [smaller one, pivot, larger ones].
// Return pivot's index.
//
int partition(int *array, int low, int high, int tid) {
  int pivot = array[high]; 	// use highest element as pivot 
  int middle = low;
  for (int i = low; i < high; i++)
    if (array[i] < pivot) {
      swap(array, i, middle);
      middle++;
    }
  swap(array, high, middle);
	printf("Thread %d found the middle [%d]\n",tid,middle);
  return middle;
}
 
// QuickSort an array range
// 
void quicksort(int *array, int low, int high, int tid) {
		if (high - low < MINSIZE) {
    	bubblesort(array, low, high,tid);
    	return;
  	}
  	int middle = partition(array, low, high, tid);
		if (low < middle)
   		quicksort(array, low, middle-1, tid);
 		if (middle < high)
   		quicksort(array, middle+1, high, tid);
}
 
// Main routine for testing quicksort
// 
int main(int argc, char **argv) {
  int *array, N;

	int i;

	int tid = 0;
  // check command line first 
  if (argc < 2) {
    printf ("Usage: ./qsort N numthreads\n");
    exit(0);
  }
	else if(argc < 3) {
		printf("Setting # of threads to 1\n");
		numThreads = 1;
		sscanf(argv[1],"%d",&N);
		if(N < 2) {
			printf("Size of array needs to be greater than 2\n");
			exit(0);
		}
  }
	else {
		sscanf(argv[1],"%d",&N);
		if(N < 2) {
			printf("Size of array needs to be greater than 2\n");
			exit(0);
		}
		sscanf(argv[2],"%d",&numThreads);
		if(numThreads < 1){
			printf("# of threads needs to be over 0\n");
			printf("Setting # of threads to 1\n");
			numThreads = 1;
		}
	}
	printf("numThreads: %d\n",numThreads);
	//initialize thread array
	threads = (int*)malloc(sizeof(int) * numThreads);
	for(i = 0;i < numThreads;++i)
		threads[i] = 0;

  array = init_array(N);


	#pragma omp parallel shared(array,N,threads) private(tid) num_threads(numThreads)
  {
	tid = omp_get_thread_num();
	//  printf("Sorting started ...\n");
  quicksort(array, 0, N-1,tid);
  //  printf("... completed.\n");
	}
  verify_array(array, N);
	
	int sum = 0;
	for(int i = 0;i < numThreads; ++i) {
		printf("[");
		if(i < 10)
			printf(" ");
		printf("%d]: %d",i,threads[i]);
		if(threads[i] < 10)
			printf("  ");
		else
			printf(" ");
		if((i+1)%4 == 0) 
			printf("\n");
	}
	return 0;
}

