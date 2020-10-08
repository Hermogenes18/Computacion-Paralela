#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
//#include <time.h>
#define threads_total 64
volatile int flag=0;
volatile double sum=0;

long long thread_count=threads_total;
long long n=pow(10,8);
pthread_t hilos[threads_total];
double tmp1,tmp2;
float tiempo_total = 0;


void* Thread_sum(void* rank) {
	long my_rank = (long) rank;
	double factor,my_sum =0.0;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	
	
	if (my_first_i % 2 == 0)
		factor = 1.0;
	else
		factor = -1.0;
	
	for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
		my_sum += factor/(2*i+1);
		//my_sum++;
	}
	
	while (flag != my_rank);
	//printf("The flag is %i and my rank is %li \n",flag,my_rank);
	
	sum+=my_sum;
	flag = (flag+1) % thread_count;

	return NULL;
}

int main(int argc, char* argv[])
{

	int x;
	pthread_t t1,t2;
	//CREATE

	for(long i=0;i<thread_count;i++){
		x = pthread_create(&hilos[i], NULL, Thread_sum, (void*)i);	
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