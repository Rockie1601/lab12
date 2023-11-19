#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Define a structure to hold thread-specific data
struct ThreadData {
    double *a;
    double sum;
    int N;
    int size;
    long tid;
};

void *compute(void *arg) {
    struct ThreadData *myData = (struct ThreadData *)arg;

    int myStart, myEnd, myN, i;

    // Determine start and end of computation for the current thread
    myN = myData->N / myData->size;
    myStart = myData->tid * myN;
    myEnd = myStart + myN;
    if (myData->tid == (myData->size - 1))
        myEnd = myData->N;

    // Compute partial sum
    double mySum = 0.0;
    for (i = myStart; i < myEnd; i++)
        mySum += myData->a[i];

    // Grab the lock, update global sum, and release lock
    pthread_mutex_lock(&mutex);
    myData->sum += mySum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;

    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    int N = atoi(argv[1]);   // no. of elements
    int size = atoi(argv[2]); // no. of threads

    // Allocate vector and initialize
    tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    struct ThreadData *threadDataArray = (struct ThreadData *)malloc(sizeof(struct ThreadData) * size);

    double *a = (double *)malloc(sizeof(double) * N);
    for (i = 0; i < N; i++)
        a[i] = (double)(i + 1);

    // Create threads
    for (i = 0; i < size; i++) {
        threadDataArray[i].a = a;
        threadDataArray[i].sum = 0.0;
        threadDataArray[i].N = N;
        threadDataArray[i].size = size;
        threadDataArray[i].tid = i;

        pthread_create(&tid[i], NULL, compute, (void *)&threadDataArray[i]);
    }

    // Wait for them to complete
    for (i = 0; i < size; i++)
        pthread_join(tid[i], NULL);

    // Calculate and print the total sum
    double totalSum = 0.0;
    for (i = 0; i < size; i++)
        totalSum += threadDataArray[i].sum;

    printf("The total is %g, it should be equal to %g\n", totalSum, ((double)N * (N + 1)) / 2);

    // Clean up allocated memory
    free(tid);
    free(threadDataArray);
    free(a);

    return 0;
}
