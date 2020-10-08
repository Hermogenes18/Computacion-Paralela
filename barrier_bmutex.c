#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#define threads_total 4
long long thread_count=threads_total;
int counter=0;
pthread_t hilos[threads_total];
pthread_mutex_t mutex;


void* Thread_work(void* rank) {
	long my_rank = (long) rank;

	printf("SE BLOQUEO EL HILO  %li\n",my_rank);
	
	pthread_mutex_lock(&mutex);
	counter++;
	pthread_mutex_unlock(&mutex);
	while (counter < thread_count);
	printf("BARRERA COMPLETADA, HILO %li\n",my_rank);
	
	return NULL;
} /* Thread sum */

int main(int argc, char* argv[]){

	int x;
	for(long i=0;i<thread_count;i++){
		x = pthread_create(&hilos[i], NULL, Thread_work, (void*) i);	
		if (x != 0){
			printf("Error en la creacion del hilo %li.\n",i);
		}
	}
	//JOIN
	for(int j=0;j<thread_count;j++){
		pthread_join(hilos[j], NULL);
	}
	printf("Programa ejecutado con %i hilos.\n",threads_total);
    return 0;
}