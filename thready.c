#include "thready.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void random_sleep() {
    int sleep_time = rand() % 8 + 1;
    sleep(sleep_time);
}

void* thread_func1(void* arg) {
    int* ix = (int*)arg;
    fprintf(stderr, "Thread %d (%ld) beginning\n", *ix, pthread_self());
    random_sleep();
    fprintf(stderr, "Thread %d (%ld) exiting\n", *ix, pthread_self());
    free(arg);
    return NULL;
}

void pattern_1(int num_of_things) {
    pthread_t threads[num_of_things];  // array

    for (int ix = 0; ix < num_of_things; ix++) {
        int* thread_arg = malloc(sizeof(int));  // allocate memory
        *thread_arg = ix + 1;                   // fill the memory
        pthread_create(&threads[ix], NULL, thread_func1, thread_arg);
    }

    for (int ix = 0; ix < num_of_things; ix++) {
        pthread_join(threads[ix], NULL);
    }
    fprintf(stderr, "\n");
}

void* thread_func2(void* arg) {
    int* args = (int*)arg;
    int ix = args[0];
    int total_things = args[1];

    fprintf(stderr, "Thread %d (%ld) beginning\n", ix, pthread_self());
    random_sleep();

    if (ix < total_things) {
        pthread_t child_thread;
        int* next_args = malloc(2 * sizeof(int));  // allocate memory
        next_args[0] = ix + 1;                     // fill the memory
        next_args[1] = total_things;

        fprintf(stderr, "Thread %d (%ld) creating thread %d\n", ix, pthread_self(), ix + 1);
        pthread_create(&child_thread, NULL, thread_func2, next_args);
        pthread_join(child_thread, NULL);
        free(next_args);
    }

    fprintf(stderr, "Thread %d (%ld) exiting\n", ix, pthread_self());
    return NULL;
}

void pattern_2(int num_of_things) {
    int* args = malloc(2 * sizeof(int));  // allocate memory
    args[0] = 1;                          // fill the memory
    args[1] = num_of_things;
    thread_func2(args);
    free(args);
    fprintf(stderr, "\n");
}
