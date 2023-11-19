#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a structure to hold the thread-specific data
struct ThreadData {
    double *a;
    double sum;
    int N;
    int size;
    long tid;
};

// Thread function to calculate the sum for a specific range
void *calculateSum(void *threadArg) {
    struct ThreadData *myData = (struct ThreadData *)threadArg;

    int start = myData->tid * myData->size;
    int end = start + myData->size;

    for (int i = start; i < end; ++i) {
        myData->sum += myData->a[i];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <array_size> <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    // Allocate memory for the array
    double *a = (double *)malloc(N * sizeof(double));
    if (a == NULL) {
        perror("Error in malloc");
        exit(EXIT_FAILURE);
    }

    // Initialize the array with some values
    for (int i = 0; i < N; ++i) {
        a[i] = i + 1;
    }

    // Create an array of thread structures
    struct ThreadData *threadDataArray = (struct ThreadData *)malloc(num_threads * sizeof(struct ThreadData));
    if (threadDataArray == NULL) {
        perror("Error in malloc");
        exit(EXIT_FAILURE);
    }

    // Create pthread_t array to store thread IDs
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Error in malloc");
        exit(EXIT_FAILURE);
    }

    // Calculate the size of each thread's workload
    int size = N / num_threads;

    // Create threads
    for (long i = 0; i < num_threads; ++i) {
        threadDataArray[i].a = a;
        threadDataArray[i].sum = 0.0;
        threadDataArray[i].N = N;
        threadDataArray[i].size = size;
        threadDataArray[i].tid = i;

        pthread_create(&threads[i], NULL, calculateSum, (void *)&threadDataArray[i]);
    }

    // Wait for threads to finish
    for (long i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the total sum
    double totalSum = 0.0;
    for (long i = 0; i < num_threads; ++i) {
        totalSum += threadDataArray[i].sum;
    }

    printf("Total sum: %f\n", totalSum);

    // Free allocated memory
    free(a);
    free(threadDataArray);
    free(threads);

    return 0;
}
