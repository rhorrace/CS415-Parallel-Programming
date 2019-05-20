//Horrace, Robert 
/*
	This program is the second openMP
	program for the assignment.
	Some of the provided code will be modified
	in order for the openMP addition to 
  function properly. OpenMP will be 
	applied to the inner for loop of
	the computation.
*/ 

#include <stdio.h>
#include <omp.h>

#define N 64

int main() {
  int a[N][N], b[N][N], c[N][N]; //matrices
  int i, j, k;//  for for loops

	//added
	//for the threads
	int tid, nthreads = 12;
	int threads[nthreads];
	//ended

  // initialization

	//added	
	for(i =0;i < nthreads;++i)
		threads[i] = 0;
	//end added

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = 1;
    }
  }

  // multiplication (modified for openMP)
  for (i = 0; i < N; i++) {
		#pragma omp parallel shared(threads,a,b,c,i) private(tid,j,k) num_threads(nthreads)
		{
		tid = omp_get_thread_num();
		#pragma omp for schedule(static)
    for (j = 0; j < N; j++) {
			c[i][j] = 0;
			printf("[%d]: [%d,%d]\n",tid,i,j);
  		for (k = 0; k < N; k++) {
  	  	c[i][j] += a[i][k] * b[k][j];
  		}
			++threads[tid];
    }
  	}
	}

  // validation
  int total = 0;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      total += c[i][j];
    }
  }
	printf("\n");


	printf("Number of threads: %d\n",nthreads);
	for(i = 0;i < nthreads;++i){
		printf("[");
		if(i < 10)
			printf(" ");
		printf("%d]: %d",i,threads[i]);
		if(threads[i] < 10) 
			printf("     ");
		else if(threads[i] < 100)
			printf("    ");
		else if(threads[i] < 1000)
			printf("   ");
		else if(threads[i] < 10000)
			printf("  ");
		else
			printf(" ");
		if((i+1)%4 == 0) 
			printf("\n");
	}
	printf("\n");

  printf("total = %d (should be 16515072)\n", total);
}
