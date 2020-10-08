#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define threads_total 8
long long thread_count=threads_total;
int counter=0;
pthread_t hilos[threads_total];
pthread_mutex_t mutex;
pthread_cond_t cond_var;


void* Thread_work(void* rank) {
	long my_rank = (long) rank;
	/* Barrier */
	pthread_mutex_lock(&mutex);
	printf("SE BLOQUEO EL HILO  %li\n",my_rank);
	counter++;

	if (counter == thread_count) {
		counter = 0;
		pthread_cond_broadcast(&cond_var);
	} 
	else {
		while (pthread_cond_wait(&cond_var, &mutex) != 0);
	}
	pthread_mutex_unlock(&mutex);
	printf("BARRERA COMPLETADA, HILO %li\n",my_rank);

}

int main(int argc, char* argv[]){
	long i;
	int x;
	for(i=0;i<thread_count;i++){
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