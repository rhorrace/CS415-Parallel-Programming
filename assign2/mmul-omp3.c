//Horrace, Robert 
/*
	This program is the third openMP
	program for this assignment.
	Some of the provided code will be modified
	in order for the openMP addition to 
  function properly. OpenMP will be 
	applied to the inner and outer for loop 
	of the computation.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 64

int main() {
  int a[N][N], b[N][N], c[N][N]; //matrices
  int i, j, k;//  for for loops

	//added
	//for the threads
	int tid1,tid2, nthreads = 12;
	int threads[nthreads][nthreads];
	int sum[nthreads];
	//ended

  // initialization

	//added	
	for(i =0;i < nthreads;++i) {
		sum[i] = 0;
		for(j = 0;j < nthreads;++j)
			threads[i][j] = 0;
	}
	//end added

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = 1;
    }
  }

  // multiplication (modified for openMP)
	#pragma omp parallel shared(threads,a,b,c) private(tid1,tid2,i,j,k) num_threads(nthreads)
	{
	tid1 = omp_get_thread_num();
	#pragma omp for schedule(static)
  for (i = 0; i < N; i++) {
		#pragma omp	parallel shared(threads,a,b,c,i,tid1) private(tid2,j,k) num_threads(nthreads)
		{
		tid2 = omp_get_thread_num();
		#pragma omp	for schedule(runtime)
    for (j = 0; j < N; j++) {
			c[i][j] = 0;
			printf("[%d][%d]: [%d,%d]\n",tid1,tid2,i,j);
  		for (k = 0; k < N; k++) {
  	  	c[i][j] += a[i][k] * b[k][j];
  		}
			++threads[tid1][tid2];
    }
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

	for(i = 0;i < nthreads;++i)
		for(j = 0;j < nthreads;++j)
			sum[i] += threads[i][j];

	printf("Number of threads: %d\n",nthreads);
	printf("Number of subthreads: %d\n",nthreads);
	for(i = 0;i < nthreads;++i){
		printf("[%d]: %d\n",i,sum[i]);
		/*for(j = 0;j < nthreads;++j){
			printf("[");
			if(j < 10)
				printf(" ");
			printf("%d]: %d",j,threads[i][j]);
			if(threads[i][j] < 10) 
				printf("     ");
			else if(threads[i][j] < 100)
				printf("    ");
			else if(threads[i][j] < 1000)
				printf("   ");
			else if(threads[i][j] < 10000)
				printf("  ");
			else
				printf(" ");
			if((j+1)%4 == 0) 
				printf("\n");
		}*/
	}
	printf("\n");

  printf("total = %d (should be 16515072)\n", total);
}
