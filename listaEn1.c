#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

#define threads_total 1

long long thread_count=threads_total;
pthread_t hilos[threads_total];
double inicio, finish;

pthread_rwlock_t rwlock;
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
		temp_p->data = value;
		temp_p->next = curr_p;
		if (pred_p == NULL) /* New first node */
			*head_p = temp_p;
		else
			pred_p->next = temp_p;
		return 1;
	} else { /* Value already in list */
	return 0;
	}
} /* Insert */





int Member(int value,struct list_node_s** head_p) {
    struct list_node_s* curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int Delete(int value, struct list_node_s** head_p) {
	struct list_node_s* curr_p = *head_p;
	struct list_node_s* pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p != NULL && curr_p->data == value) {
		if (pred_p == NULL) { /* Deleting first node in list */
			*head_p = curr_p->next;
			free(curr_p);
		} else {
			pred_p->next = curr_p->next;
			free(curr_p);
		}
			return 1;
	} else { /* Value isn’t in list */
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
        pthread_rwlock_wrlock(&rwlock);
        Member1(random,&head_p);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < inserts; ++i){
        pthread_rwlock_wrlock(&rwlock);
        Insert(i,&head_p);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < deletes; ++i){
        pthread_rwlock_wrlock(&rwlock);
        Delete(i,&head_p);
        pthread_rwlock_unlock(&rwlock);
    }
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