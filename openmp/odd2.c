#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


//#define thread_count  32
#define n 100000

int i,phase,j,tmp;

void odd(int a[n]);

int main(int argc, char* argv[])
{

    int a[n];
    int thread_count = strtol(argv[1], NULL, 10);
  
    srand(time(0));
    for(int i=0;i<n;i++)
    {	
    	int random=rand();
    	a[i]=random;
    }

}

void odd(int a[n]){
	int thread_count = omp_get_num_threads();
    double start_time;
    start_time = omp_get_wtime();

	# pragma omp parallel num threads(thread count) \
			default(none) shared(a, n) private(i, tmp, phase)
		for (phase = 0; phase < n; phase++) {
			if (phase % 2 == 0)
	# pragma omp for
			for (i = 1; i < n; i += 2) {
				if (a[i−1] > a[i]) {
					tmp = a[i−1];
					a[i−1] = a[i];
					a[i] = tmp;
				}
			}
		else
	# pragma omp for
		for (i = 1; i < n−1; i += 2) {
			if (a[i] > a[i+1]) {
				tmp = a[i+1];
				a[i+1] = a[i];
				a[i] = tmp;
			}
		}
	}

	printf("\nNumero de hilos: %d \n",thread_count);
	printf("Execution time = %lf seconds\n", omp_get_wtime() - start_time);
}
