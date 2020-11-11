#include <stdio.h>

#define MAXWORK 40

int work[MAXWORK],  // work to be done
    nwork=0,  // Numero de items queue
    nextput=0,  // Numero de trabajo en nextput
    nextget=-1,  // Conumidores de work[nextget]
    breaksum,  // Detiene la suma
    done = 0, 
    psum,csum,  // suma de producto, consumidor
    pwork,  // trabajo realizado por el productor
    *cwork,  // trabajo realizado por el consumidor
    nth,  // numero de hilos
    debugflag;

void next(int *m)
{  (*m)++; 
   if (*m >= MAXWORK) *m = 0;
}

void putwork(int k)
{  int put = 0;
   while (!put)  {  
      if (nwork < MAXWORK)  {
         #pragma omp critical
         {  work[nextput] = k;
            if (nwork == 0) nextget = nextput;
            next(&nextput);
            nwork++;
            put = 1;
         }
      }
      else sched_yield();
   }
}

int getwork()
{  int k,get=0;
   while (!get)  {
      if (done && nwork == 0) return -1;
      if (nwork > 0)  {
         #pragma omp critical
         {  
            if (nwork > 0)  {
               k = work[nextget];
               next(&nextget);
               nwork--;
               if (nwork == 0) nextget = -1;
               get = 1;
            }
         }
      }
      else sched_yield();
   }
   return k;
}

void dowork()
{  
   #pragma omp parallel  
   {  int me = omp_get_thread_num(),  // Numero de hilos
          num;
      #pragma omp single
      {  int i;
         nth = omp_get_num_threads();  
         printf("there are %d threads\n",nth);
         cwork = (int *) malloc(nth*sizeof(int));
         for (i = 1; i < nth; i++) cwork[i]=0;
      }
      if (me == 0 && debugflag) {int wait=0; while (!wait) ; }
      #pragma omp barrier  
      if (me == 0) 
      {
         pwork = 0;
         while (1)
         {
            num = rand() % 100;
            putwork(num);
            psum += num;
            pwork++;
            if (psum > breaksum)
            {
               done = 1;
               return;
            }
         }
      }
      else  {  // I'm a consumer
         while (1)  {
            num = getwork();
            if (num == -1) return;
            cwork[me]++;
            #pragma omp atomic
            csum += num;
         }
      }
   }
}

int main(int argc, char **argv)
{  int i;
   breaksum = atoi(argv[1]);
   debugflag = atoi(argv[2]);
   dowork();
   printf("sum reported by producer:  %d\n",psum);
   printf("sum reported by consumers:  %d\n",csum);
   printf("work done by producer:  %d\n",pwork);
   printf("work done by consumers:\n");
   for (i = 1; i < nth; i++) 
      printf("%d\n",cwork[i]);
}