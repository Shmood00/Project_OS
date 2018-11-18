#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

pthread_mutex_t mutex;						
sem_t full, empty;								
buffer_item buffer[BUFFER_SIZE];				
int counter = 0;								
long long di = 10000000;						
pthread_t tid;       							
pthread_attr_t attr; 							


void initialize()
{
	pthread_mutex_init(&mutex, NULL);
   	sem_init(&full, 0, 0);
   	sem_init(&empty, 0, BUFFER_SIZE);
   	pthread_attr_init(&attr);
}

int insert_item(buffer_item item) {

	if(counter < BUFFER_SIZE) {
			buffer[counter] = item;
			counter++;
			return 0;
	} else  
		return -1;	
} 


void *producer(void *param) {
	buffer_item item;

	while(1 != 0) {
		sleep(rand()/di);
		item = rand();

		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if(insert_item(item)) 
			fprintf(stderr, " Producer report error condition\n");
		else 
			printf("producer produced %d\n", item);		
		
		pthread_mutex_unlock(&mutex);    
		sem_post(&full);
	  
   	}
}

int main(int argc, char *argv[])
{

	int sleepTime = atoi(argv[1]);
	int numProducers = atoi(argv[2]);	

	initialize();
	
	int i;
	for(i = 0; i < numProducers; i++) {
      		pthread_create(&tid, &attr, producer, NULL);
   	}

	sleep(sleepTime);
	return 0;
}
