//Horrace, Robert
/*
	This is the fourth program 
	for CS415. It will utilize a
	MPI version of bucket sort, which
	places elements are an unsorted array
	into buckets based on their bit values
	and places them in a result array sorted,
	after being sorted in their respected buckets.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define MAX(X,Y) (((X) > (Y)) ? (X):(Y))

#define DATABITS 13
 
void swap(int *array, int i, int j) {
  if (i == j) return;
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

void bubblesort(int *array, int low, int high) {
 	int i,j;
	if (low >= high) return;
  for (i = low; i <= high; i++)
    for (j = i+1; j <= high; j++) 
      if (array[i] > array[j])
				swap(array, i, j);
}

void print_array(int *array, int N) {
	int i;
  for (i = 0; i < N; i++) {
    printf("%4d ", array[i]);
    if (i % N == N-1)
      printf("\n");
  }
  printf("\n");
}

int isPower2(int x) {
  return !(x & (x - 1));
}

int bktidx(int b, int x, int B) {
	return x >> (b - (int)log2(B));
}

int main(int argc, char ** argv) {
	int i,j,k,N,P,rank,size,buf,bsize,psize,ierr, namesize;
	char *in, *out;
	MPI_File fin,fout;
	MPI_Status st;
	MPI_Offset fsize,g_start,offset;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	namesize = 100;
	char pname[namesize];
	MPI_Get_processor_name(pname, &namesize);
	printf("Process %d starting on %s\n", rank, pname);
	//Checking arguments
	if(argc < 3){
		if(rank == 0)
			printf("Error: takes format  ./bsort-mpi <infile> <outfile>\n");
		MPI_Finalize();
		exit(1);
	}
	in = argv[1];
	out = argv[2];
	//check if file exists
	ierr = MPI_File_open(MPI_COMM_WORLD, in, MPI_MODE_RDONLY, MPI_INFO_NULL, &fin);
	if(ierr){
		if(rank == 0)
			printf("Error: Could not open file %s\n", in);
		MPI_Finalize();
		exit(2);
	}
	//Get filesize
	MPI_File_get_size(fin, &fsize);
	if(fsize == 0 || fsize-1 == 0){
		if(rank == 0)
			printf("%s is empty\n",argv[1]);
		MPI_Finalize();
		exit(3);
	}
	N = fsize/sizeof(int);

	//If file is empty
	if(N == 0){
		if(rank == 0)
			printf("N can't be zero\n");
		MPI_Finalize();
		exit(4);
	}
	if(N%P != 0){
		if(rank == 0)
			printf("Error: # of elements must be divisible by # of processes\n");
		MPI_Finalize();
		exit(5);
	}
	offset = fsize/P;
	g_start = rank*offset;
	buf = N/P;
	int section[buf];
	MPI_File_read_at_all(fin, g_start, section, offset, MPI_INT, &st);
	
	//Make buckets for each process
	bsize = MAX(32,2*N/pow(P,2));
	int bcnt[P];
	for(i = 0;i < P;++i)
		bcnt[i] = 0;
	int bucket[P][bsize];
	for(i = 0;i < buf;++i){
		k = bktidx(DATABITS, section[i], P);
		bucket[k][bcnt[k]++] = section[i];
	}
	
	//Make a data for making process bucket for bucket collection and make gather displacement
	psize = 0;
	int disp[P];
	int recvcounts[P];
	int hold;
	disp[0] = 0;
	for(i = 0;i < P;++i){
		MPI_Scatter(bcnt, 1, MPI_INT, &hold, 1, MPI_INT, i, MPI_COMM_WORLD);
		psize += hold;
		recvcounts[i]= hold;
		MPI_Barrier(MPI_COMM_WORLD);
	}
	for(i = 1;i < P;++i){
		disp[i] = disp[i-1] + recvcounts[i-1];
	}

	//Initialize process bucket
	int pbucket[psize];
	int arr[psize];
	for(i = 0;i < psize;++i)
		pbucket[i] = 0;

	//Send/receive and place into process's big bucket
	for(i = 0;i < P;++i){
		MPI_Gatherv(bucket[i], bcnt[i], MPI_INT, pbucket, recvcounts, disp, MPI_INT, i, MPI_COMM_WORLD);
	}

	//Sort big bucket
	bubblesort(pbucket, 0, psize);

	//Open file for writing and write it
	ierr = MPI_File_open(MPI_COMM_WORLD, out, MPI_MODE_CREATE|MPI_MODE_RDWR, MPI_INFO_NULL, &fout);
	if(ierr){
		if(rank == 0)
			printf("Error opening file %s\n",out);
		MPI_Finalize();
		exit(6);
	}
	if(rank == 0){
		printf("Process %d big bucket: ", rank);
		print_array(pbucket, psize);
		MPI_File_get_size(fout, &fsize);
		printf("File size: %d\n", fsize);
		MPI_File_write_at(fout, fsize, pbucket, psize, MPI_INT, &st);
		MPI_File_get_size(fout, &fsize);
		printf("File size: %d\n", fsize);
		if(P > 1)
			MPI_Send(&fsize, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else{
		MPI_Recv(&fsize, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &st);
		printf("Process %d big bucket: ", rank);
		print_array(pbucket, psize);
		MPI_File_write_at(fout, fsize, pbucket, psize, MPI_INT, &st);
		MPI_File_get_size(fout, &fsize);
		printf("File size: %d\n", fsize);
		if(rank != P-1)
			MPI_Send(&fsize, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_close(&fout);
	printf("Process %d on %s ending\n", rank, pname);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
