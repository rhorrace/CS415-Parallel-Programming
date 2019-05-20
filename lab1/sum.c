// The sum program (Sequential version).
//
//
#include <stdio.h>

#define N 1000 	/* problem domain size */

int sum = 0;

int compute(int i) {
  return i*i;
}

int main(int argc, char **argv) {
  int i; 
  for (i = 0; i < N; i++)
    sum += compute(i);
  printf("The sum of 1,2,...,1000 is %d (should be 332833500)\n", sum);
}  
