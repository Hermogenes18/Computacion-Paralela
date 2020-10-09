#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


#define m 8
#define n 8000000
#define threads_total 4

int     thread_count=threads_total;
pthread_t hilos[threads_total];
double* A;
double* x;
double* y;



void *Pth_mat_vect(void* rank) {
	long my_rank = (long) rank;
	int i, j;
	int local_m = m/thread_count;
	int my_first_row = my_rank*local_m;
	int my_last_row = (my_rank+1)*local_m - 1;

	for (i = my_first_row; i <= my_last_row; i++) {
		y[i] = 0.0;
		for (j = 0; j < n; j++)
			y[i] += A[i*n+j]*x[j];
		}

	return NULL;
} /* Pth mat vect */

int main(int argc, char* argv[])
{
	double start, finish;
	int t;

	A = malloc(m*n*sizeof(double));
	x = malloc(n*sizeof(double));
	y = malloc(m*sizeof(double));

	for (int i = 0; i < n; i++)
      x[i] = i;
  	for (int i = 0; i < m; i++)
	  for (int j = 0; j < n; j++)
	     A[i*n+j]=i;

	GET_TIME(start);
	for(int i=0;i<thread_count;i++){
		t = pthread_create(&hilos[i], NULL, &Pth_mat_vect, (void*)i);	
		if (t != 0){
			printf("Error en la creacion del hilo %i.\n",i);
		}
	}
	for(int j=0;j<thread_count;j++){
		pthread_join(hilos[j], NULL);
	}
	GET_TIME(finish);
	printf("Tiempo : %lf  segundos \n", finish - start);
	printf("Programa ejecutado con %i hilos.\n",threads_total);
}