/* 
  Simple Pthread Program to find the sum of a vector - Version 2
  Each thread computes the partial sum, writes the partial sum to 
  a separate global variable and the main thread computes the global sum. 
  Author: Purushotham Bangalore
  Date: Jan 25, 2009

  To Compile: gcc -O -Wall pthread_psum2.c -lpthread
  To Run: ./a.out 1000 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

double *a=NULL, *partialsum;
int    N, nthreads;

void *compute(void *arg) {
    int myStart, myEnd, myN, i;
    long tid = (long)arg;

    // determine start and end of computation for the current thread
    myN = N/nthreads;
    myStart = tid*myN;
    myEnd = myStart + myN;
    if (tid == (nthreads-1)) myEnd = N;

    // compute partial sum
    double mysum = 0.0;
    for (i=myStart; i<myEnd; i++)
      mysum += a[i];

    partialsum[tid] = mysum;
    return (NULL);
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;
    double sum = 0.0;

    if (argc != 3) {
       printf("Usage: %s <# of elements> <# of threads>\n",argv[0]);
       exit(-1);
    }

    N = atoi(argv[1]); // no. of elements
    nthreads = atoi(argv[2]); // no. of threads

    // allocate vector and initialize
    tid = (pthread_t *)malloc(sizeof(pthread_t)*nthreads);
    a = (double *)malloc(sizeof(double)*N); 
    partialsum = (double *)malloc(sizeof(double)*nthreads); 
    for (i=0; i<N; i++)
      a[i] = (double)(i + 1);

    // create threads
    for ( i = 0; i < nthreads; i++)
      pthread_create(&tid[i], NULL, compute, (void *)i);
    
    // wait for them to complete
    for ( i = 0; i < nthreads; i++) 
      pthread_join(tid[i], NULL);

    for ( i = 0; i < nthreads; i++) 
      sum += partialsum[i];
    
    printf("The total is %g, it should be equal to %g\n", 
           sum, ((double)N*(N+1))/2);
    
    free(tid);
    free(a);
    free(partialsum);

    return 0;
}

