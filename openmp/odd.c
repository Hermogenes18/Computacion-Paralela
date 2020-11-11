/*
       To compile:-
        gcc -fopenmp file_name.c
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int n=10;
int main()
{
    int i,j,temp;
    int a[]= {9,8,7,6,5,4,3,2,1,0};

    /*printf("\nEnter 10 Array Elements\n");
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }*/
   
    printf("\nArray Elements Before Sorting\n");
   
    for(i=0;i<n;i++)
    printf("\na[%d]=%d",i,a[i]);

    double start_time;
    start_time = omp_get_wtime();

   
    for(i=0;i<n;i++)
    {   
        if(i%2==0)
        {
            #pragma omp parallel for private(temp,j) shared(a)
            for(j=0;j<n-1;j+=2)
            {
                if(a[j]> a[j+1])
                {
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
            }
        }
        else
        {
            #pragma omp parallel for private(temp,j) shared(a)
            for(j=1;j<n-1;j+=2)
            {
                if(a[j]> a[j+1])
                {
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
            }
        }
       
    }

    printf("\n Execution time = %lf seconds\n", omp_get_wtime() - start_time);

    printf("\n\nSorted Array");
    for(i=0;i<n;i++)
    printf("\na[%d]=%d",i,a[i]);
    printf("\n\n");
}