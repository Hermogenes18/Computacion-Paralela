#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h> 


#define thread_count  4


int n=50000;
int main()
{
    int i,phase,j,tmp;
    int a[n];

    srand(time(0));
    for(int i=0;i<n;i++)
    {	
    	int random=rand();
    	a[i]=random;
    	//printf("%d\n",random);
    }


    double start_time;
    start_time = omp_get_wtime();

	for (phase = 0; phase < n; phase++) {
		if (phase % 2 == 0)
	# pragma omp parallel for num_threads(thread_count)  \
			default(none) shared(a, n) private(i, tmp)
		for (i = 1; i < n; i += 2) {
			if (a[i-1] > a[i]) {
				tmp = a[i-1];
				a[i-1] = a[i];
				a[i] = tmp;
			}
		}
	else
	# pragma omp parallel for num_threads(thread_count) \
		default(none) shared(a, n) private(i, tmp)
	for (i = 1; i < n-1; i += 2) {
		if (a[i] > a[i+1]) {
			tmp = a[i+1];
			a[i+1] = a[i];
			a[i] = tmp;
		}
		}
	}

	printf("\n Numero de hilos: %d \n",thread_count);
	printf("Execution time = %lf seconds\n", omp_get_wtime() - start_time);

}