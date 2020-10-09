
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include "timer.h"

#define threads_total 2

long long thread_count=threads_total;
pthread_t hilos[threads_total];
double inicio, finish;


pthread_mutex_t head_p_mutex;
pthread_mutex_t list_mutex;



struct list_node_s {
	int data;
	struct list_node_s* next;
	pthread_mutex_t mutex;
};

struct list_node_s* head_p=NULL;

int Insert(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        pthread_mutex_init(&(temp_p->mutex), NULL);
        temp_p->data = value;
        temp_p->next = curr_p;
        if (curr_p != NULL) 
            pthread_mutex_unlock(&(curr_p->mutex));
        if (pred_p == NULL) 
            *head_p = temp_p;
        else{
            pred_p->next = temp_p;
            pthread_mutex_unlock(&(pred_p->mutex));
        }
        return 1;
    } else {
        return 0;
    }
}



int Member(int value){
	struct list_node_s* temp_p;

	pthread_mutex_lock(&head_p_mutex);
	temp_p = head_p;
	while (temp_p != NULL && temp_p->data < value) {
		if (temp_p->next != NULL)
			pthread_mutex_lock(&(temp_p->next->mutex));
		if (temp_p == head_p)
			pthread_mutex_unlock(&head_p_mutex);
		pthread_mutex_unlock(&(temp_p->mutex));
		temp_p = temp_p->next;
	}
	if (temp_p == NULL || temp_p->data > value) {
		if (temp_p == head_p)
			pthread_mutex_unlock(&head_p_mutex);
		if (temp_p != NULL)
			pthread_mutex_unlock(&(temp_p->mutex));
		return 0;
	} else {
		if (temp_p == head_p)
			pthread_mutex_unlock(&head_p_mutex);
		pthread_mutex_unlock(&(temp_p->mutex));
		return 1;
	}
} /* Member */




int Delete(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;  

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) { 
            *head_p = curr_p->next;
            pthread_mutex_unlock(&head_p_mutex);
            pthread_mutex_unlock(&(curr_p->mutex));
            pthread_mutex_destroy(&(curr_p->mutex));
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            pthread_mutex_unlock(&(pred_p->mutex));
            pthread_mutex_destroy(&(curr_p->mutex));
            free(curr_p);
        }
        return 1;
    } else { 
        if (pred_p != NULL)
            pthread_mutex_unlock(&(pred_p->mutex));
        if (curr_p != NULL)
            pthread_mutex_unlock(&(curr_p->mutex));
        if (curr_p == *head_p)
            pthread_mutex_unlock(&head_p_mutex);
        return 0;
    }
}

void* Thread_work(void* rank) {
    long my_rank = (long) rank;
    int Operaciones = 100000/threads_total;
    int menbers = 99.9*Operaciones/100;
    int inserts = 0.05*Operaciones/100;
    int deletes = 0.05*Operaciones/100;
    for (int i = 0; i < menbers; ++i){
        int random = rand() % 1000;
        Member(random);
    }
    for (int i = 0; i < inserts; ++i){
        Insert(i,&head_p);
    }
    for (int i = 0; i < deletes; ++i){
        Delete(i,&head_p);
    }
    printf("Corrio el hilo %ld\n",my_rank);
    return NULL;
    printf("EJECUTADO, HILO %ld\n",my_rank);
    return NULL;
}  


int main(int argc, char* argv[]){
	int x;
	for(int i=0;i<1000;i++)
	{
		Insert(i,&head_p);
	}
	GET_TIME(inicio);
	for(int i=0;i<thread_count;i++){
		x = pthread_create(&hilos[i], NULL, &Thread_work, (void*)i);	
		if (x != 0){
			printf("Error en la creacion del hilo %i.\n",i);
		}
	}
	for(int j=0;j<thread_count;j++){
		pthread_join(hilos[j], NULL);
	}
	GET_TIME(finish);
	printf("Programa ejecutado con %i hilos.\n",threads_total);
	printf("Tiempo: %lf segundos\n", finish - inicio);
    return 0;
}