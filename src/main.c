#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_TASKS 5

#define PRIORITY_TASK1 5
#define PRIORITY_TASK2 4
#define PRIORITY_TASK3 3
#define PRIORITY_TASK4 2 
#define PRIORITY_TASK5 1

void* task1(void* arg) {
    printf("Hello!\n");
    return NULL;
}

void* task2(void* arg) {
    while (1) {
        printf("Dahyun!\n");
        sleep(2);
    }
    return NULL;
}

void* task3(void* arg) {
    while (1) {
        printf("Chaves é malucoooo\n");
        sleep(1);
    }
    return NULL;
}

void* task4(void* arg) {
    while (1) {
        printf("Chaeyoung!\n");
        sleep(60);
    }
    return NULL;
}

void* task5(void* arg) {
    while (1) {
        printf("Nayeon!\n");
        sleep(10);
    }
    return NULL;
}

int main() {
    printf("Entrei!!\n");
    pthread_t threads[NUM_TASKS];
    struct sched_param sched_params;

    pthread_create(&threads[0], NULL, task1, NULL); // Thread 1
    pthread_create(&threads[1], NULL, task2, NULL); // Thread 2
    pthread_create(&threads[2], NULL, task3, NULL); // Thread 3
    pthread_create(&threads[3], NULL, task4, NULL); // Thread 4
    pthread_create(&threads[4], NULL, task5, NULL); // Thread 5

    // Thread 1 has the lowest priority
    sched_params.sched_priority = PRIORITY_TASK1;
    pthread_setschedparam(threads[0], SCHED_RR, &sched_params);

    // Thread 2 has medium-low priority
    sched_params.sched_priority = PRIORITY_TASK2;
    pthread_setschedparam(threads[1], SCHED_RR, &sched_params);

    // Thread 3 has medium priority
    sched_params.sched_priority = PRIORITY_TASK3;
    pthread_setschedparam(threads[2], SCHED_RR, &sched_params);

    // Thread 4 has higher priority
    sched_params.sched_priority = PRIORITY_TASK4;
    pthread_setschedparam(threads[3], SCHED_RR, &sched_params);

    // Thread 5 has the highest priority
    sched_params.sched_priority = PRIORITY_TASK5;
    pthread_setschedparam(threads[4], SCHED_RR, &sched_params);

    // Wait for all threads to finish (though Thread 2, 3, 4, and 5 run indefinitely)
    for (int i = 0; i < NUM_TASKS; i++) {
        pthread_join(threads[i], NULL);
    }
}
