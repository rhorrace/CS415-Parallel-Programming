/*
	Horrace, Robert
	CS 415
	Due: 1/30/2018

	This program is the producer/consumer
	program that uses pthreads. There will
	be one producer and a specific amount
	of consumer threads determined by the
	user or the program, depending if there
	is a number or not. If the user does not
	clarify a number of threads, the value one
	will be the default.
*/

#define _GNU_SOURCE
#include "task-queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <unistd.h>

#define QLIMIT 20//for limiting length of queue

pthread_mutex_t lock;
pthread_cond_t not_full,not_empty;//conditions for wait/signal/broadcast

queue_t * queue = NULL;//a global task queue

int numCons = 1;//number of consumers
int tasks_p = 0;//tasks produced
int tasks_d = 0;//tasks done
int last_task = 0;//flag to break consumer thread loop
int * tasks_bc;//number of tasks done by consumer(s)

//for producer thread
void produce(void * ignore)
{
	task_t * task_c = NULL;
	int cpu_id = sched_getcpu();
	/*Display what core thread is running on*/
	printf("Producer starting on core %d\n",cpu_id);
	while(1)
	{
		//If 100 tasks produced
		if(tasks_p == 100) break;
		//lock
		pthread_mutex_lock(&lock);
		//try to add task, wait if empty
		task_c = create_task(tasks_p+1);
		while(add_task(queue,task_c) == 0)
			pthread_cond_wait(&not_full,&lock);
		//increment tasks produced
		++tasks_p;
		/*Display task # added*/
		printf("Task %d added to queue\n",tasks_p);
		//unlock
		pthread_mutex_unlock(&lock);
		//broadcast queue is not empty
		pthread_cond_broadcast(&not_empty);
	}
	/*display producer thread is ending*/
	printf("Producer ending\n");
}

//for consumer threads
void consume(long tid)
{
	//for removing task from queue
	task_t * task_r = NULL;
	int cpu_id = sched_getcpu();
	/*Display core consumer thread is running on*/
	printf("Consumer thread [%ld] starting on core %d\n",tid,cpu_id);
	while(1)
	{
		//lock
		pthread_mutex_lock(&lock);
		//check if last task has been reached
		if(last_task == 1) break;
		//try to remove task
		task_r = remove_task(queue);
		//wait if removal failed until queue is not empty
		if(!task_r)
				pthread_cond_wait(&not_empty,&lock);
		//otherwise continue
		else
		{
			//increment tasks done
			tasks_d++;
			//increment task done by specific consumer thread
			++*(tasks_bc + tid);
			/*Display task removed by consumer thread*/
			printf("Consumer thread [%ld] removed task %d\n",tid,task_r -> val);
			//check if final task has been removed
			if(task_r -> val == 100) last_task = 1;
			free(task_r);
			task_r = NULL;
		}
		//unlock
		pthread_mutex_unlock(&lock);
		//signal queue is not full
		pthread_cond_signal(&not_full);
	}
	//unlock when out of loop
	pthread_mutex_unlock(&lock);
	/*Display consumer thread is ending*/
	printf("Consumer thread[%ld] ending\n",tid);
	//free task_r if not NULL
	if(task_r)
	{
		free(task_r);
		task_r = NULL;
	}
}
//The main function
int main(int argc,char *argv[])
{
	//producer thread
	pthread_t producer;
	//for setting a specific CPU core
	cpu_set_t cpuset;
	int cid = 0,nprocs = sysconf(_SC_NPROCESSORS_ONLN);
	//set number of consumers
	if(argc == 1) numCons = 1;
	else
	{
		if((numCons = atoi(argv[1])) < 1) numCons = 1;
	}
	//set array for capturing the task done by each thread
	tasks_bc = (int*) malloc(sizeof(int)*numCons);
	for(int i = 0;i < numCons;++i)
		tasks_bc[i] = 0;
	//initialize consumer threads to size numCons
	pthread_t consumer[numCons];
	/*printf statement to test numCons*/
	//printf("Number of consumers: %d\n",numCons);
	//initialize task queue
	queue = init_queue(QLIMIT);
	//initialize mutex and cond(s)
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&not_full,NULL);
	pthread_cond_init(&not_empty,NULL);
	//create producer and set it to a core
	pthread_create(&producer,NULL,(void*)&produce,(void*)NULL);
	CPU_ZERO(&cpuset);
	CPU_SET(cid++ % nprocs,&cpuset);
	pthread_setaffinity_np(producer,sizeof(cpu_set_t),&cpuset);	
	/*Display that producer thread is created*/
	printf("Producer thread created\n");
	//Create numCons consumer threads
	for(long k = 0;k < numCons;++k)
	{
		pthread_create(&consumer[k],NULL,(void*)&consume, (void*)k);
		CPU_ZERO(&cpuset);
		CPU_SET(cid++ % nprocs,&cpuset);
		pthread_setaffinity_np(consumer[k],sizeof(cpu_set_t),&cpuset);
		/*Display each consumer thread is created*/
		printf("consumer thread[%ld] created\n",k);
	}
	/*Display all consumer threads are created*/
	printf("%d consumer thread(s) created\n",numCons);
	//Join producer thread
	pthread_join(producer,(void*)NULL);
	//Join consumer threads
	for(long k = 0;k < numCons;++k)
		pthread_join(consumer[k],(void*)NULL);
	/*Display that all threads are finished*/
	printf("All threads finished\n");
	/*Display the tasks produced/done (Should be 100 each)*/
	printf("Tasks produced: %d\n",tasks_p);
	printf("Tasks done: %d\n",tasks_d);
	/*display tasks done by each consumer thread*/
	printf("Task done by each thread:\n");
	for(int i = 0;i < numCons;++i)
	{
		printf("C[%d]: %d\t",i,*(tasks_bc + i));
		if(i > 0 && i%4 == 0) printf("\n");
	}
	//a key for consumer
	printf("\nKey: C = Consumer thread\n");
	/*Check if queue is empty, display success/failure*/
	if(queue -> length == 0) printf("Program successful\n");
	else printf("Program failed\n");
	//Free any allocated memory
	free(tasks_bc);
	tasks_bc = NULL;	
	free(queue);
	queue = NULL;	
	return 0;
} 
