/*
 *
 * Authors: John Hebb, Ian Gerics, Adam Iannaci
 * Date: November 18th, 2018
 * Github: https://github.com/Shmood00/Project_OS/
 * Professor: Cristina Ribeiro
 * Course: Operating Systems Design & UNIX Programming
 * School: Sheridan College
 * 
 * Description:
 * 
 * 		This program uses semaphores to solve the bounded-buffer
 * 		producer-consumer problem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

pthread_mutex_t mutex;							// mutex
sem_t full, empty;								// semaphores
buffer_item buffer[BUFFER_SIZE];				// buffer
int counter = 0;								// counter
long long di = 1000000000;						// rand divisor
pthread_t tid;       							// thread id
pthread_attr_t attr; 							// thread attributes

// Initialize data
void initialize()
{
	pthread_mutex_init(&mutex, NULL);
   	sem_init(&full, 0, 0);
   	sem_init(&empty, 0, BUFFER_SIZE);
   	pthread_attr_init(&attr);
}
// Add item to the buffer
int insert_item(buffer_item item) {

	if(counter < BUFFER_SIZE) {
			buffer[counter] = item;
			counter++;
			return 0;
	} else  
		return -1;	
} // end of insert_item()

// Remove item from the buffer
int remove_item(buffer_item *item) {
	
	if(counter > 0) {
		*item = buffer[(counter-1)];
		counter--;
		return 0;
	} else
		return -1;	
} // end of remove_item()

// producer thread
void *producer(void *param) {
	buffer_item item;

	while(1 != 0) {
		sleep(rand()/di);
		item = rand();

		// acquire semaphore and lock
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if(insert_item(item)) 
			fprintf(stderr, " Producer report error condition\n");
		else 
			printf("producer produced %d\n", item);		
		
		// release semaphore and lock
		pthread_mutex_unlock(&mutex);    
		sem_post(&full);
	  
   	}
} // end of producer()

// consumer thread
void *consumer(void *param) {
   	buffer_item item;

   	while(1 != 0) {
	    sleep(rand()/di);

		// acquire semaphore and lock
      	sem_wait(&full);
      	pthread_mutex_lock(&mutex);

		if(remove_item(&item)) 
			fprintf(stderr, "Consumer report error condition\n");
		else 
         	printf("consumer consumed %d\n", item);

		// release semaphore and lock	
		pthread_mutex_unlock(&mutex);		
		sem_post(&empty);		
   }
} // end of consumer()

int main(int argc, char *argv[])
{
	if (argc != 4) 
		fprintf(stderr, "USAGE: ./produce_consume <sleep time> <producers> <consumers>\n");

	int sleepTime = atoi(argv[1]);
	int numProducers = atoi(argv[2]);
	int numConsumers = atoi(argv[3]);	

	initialize();
	// create threads
	int i;
	for(i = 0; i < numProducers; i++) {
      pthread_create(&tid, &attr, consumer, NULL);
   	}

	for (i = 0; i < numConsumers; i++) {
		pthread_create(&tid, &attr, producer, NULL);
	}

	sleep(sleepTime);
	return 0;
} // end of main()