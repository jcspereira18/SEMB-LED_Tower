#include <pthread.h>
#include <sched.h>
#include <stdio.h>

#include "include/components/init_comp.hpp"
#include "include/threads.hpp"

void sync(timespec startTime);

int main() {
  CubeSystem c;

  pthread_attr_t initAttributes;
  pthread_attr_t displayAttr;
  pthread_attr_t RRthreads;

  pthread_attr_init(&initAttributes);
  pthread_attr_init(&displayAttr);
  pthread_attr_init(&RRthreads);

  pthread_attr_setschedpolicy(&initAttributes, SCHED_FIFO);
  pthread_attr_setschedpolicy(&displayAttr, SCHED_RR);
  pthread_attr_setschedpolicy(&RRthreads, SCHED_RR);

  struct sched_param initParam;
  struct sched_param displayParam;
  struct sched_param RRthreadsParam;

  initParam.sched_priority = 3; // Higher means higher prio.
  displayParam.sched_priority = 2;
  RRthreadsParam.sched_priority = 1;

  pthread_attr_setschedparam(&initAttributes, &initParam);
  pthread_attr_setschedparam(&displayAttr, &displayParam);
  pthread_attr_setschedparam(&RRthreads, &RRthreadsParam);

  pthread_attr_setinheritsched(&initAttributes, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setinheritsched(&displayAttr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setinheritsched(&RRthreads, PTHREAD_EXPLICIT_SCHED);

  pthread_t initCubeSystemThread;
  pthread_t micro1thread;
  pthread_t micro2thread;
  pthread_t micro3thread;
  // Hardware initialization
  // Runs once
  // WARN: one-shot thread!

  if (pthread_create(&initCubeSystemThread, &initAttributes, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create initCubeSystemThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);

  // sync
  struct timespec startTime, currentTime;
  // initialize Timer
  long ellapsedTimeMs = 0;
  long iterationTimeMs = 0;

  clock_gettime(CLOCK_MONOTONIC, &startTime);

  printf("Entering loop.\n");

  while (1) {
    printf("Starting micro1thread\n");
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    pthread_create(&micro1thread, &displayAttr, micro1, (void *)&c);
    pthread_join(micro1thread, NULL);
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    ellapsedTimeMs = (currentTime.tv_sec - startTime.tv_sec) * 1000;
    ellapsedTimeMs += (currentTime.tv_nsec - startTime.tv_nsec) / 1'000'000;
    printf("ellapsedTimeMs: %ld ms.\n", ellapsedTimeMs);

    if (ellapsedTimeMs < 100) {
      usleep(100 - ellapsedTimeMs);
      printf("ellapsedTime is < 100, sleeping for %ld ms.\n",
             100 - ellapsedTimeMs);
      clock_gettime(CLOCK_MONOTONIC, &startTime);
      /* continue; */
    }

    printf("Starting micro2thread\n");
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    pthread_create(&micro2thread, &displayAttr, micro2, (void *)&c);
    pthread_join(micro2thread, NULL);
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    ellapsedTimeMs = (currentTime.tv_sec - startTime.tv_sec) * 1000;
    ellapsedTimeMs += (currentTime.tv_nsec - startTime.tv_nsec) / 1'000'000;
    printf("ellapsedTimeMs: %ld ms.\n", ellapsedTimeMs);

    if (ellapsedTimeMs < 100) {
      usleep(100 - ellapsedTimeMs);
      printf("ellapsedTime is < 100, sleeping for %ld ms.\n",
             100 - ellapsedTimeMs);
      clock_gettime(CLOCK_MONOTONIC, &startTime);
      /* continue; */
    }

    printf("Starting micro3thread\n");
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    pthread_create(&micro3thread, &displayAttr, micro3, (void *)&c);
    pthread_join(micro3thread, NULL);
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    ellapsedTimeMs = (currentTime.tv_sec - startTime.tv_sec) * 1000;
    ellapsedTimeMs += (currentTime.tv_nsec - startTime.tv_nsec) / 1'000'000;
    printf("ellapsedTimeMs: %ld ms.\n", ellapsedTimeMs);

    if (ellapsedTimeMs < 100) {
      usleep(100 - ellapsedTimeMs);
      printf("ellapsedTime is < 100, sleeping for %ld ms.\n",
             100 - ellapsedTimeMs);
      clock_gettime(CLOCK_MONOTONIC, &startTime);
      continue;
    }
    if (ellapsedTimeMs >= 100) {
      printf("Breaking loop, ellapsedTimeMs is %ld ms.\n", ellapsedTimeMs);
      break;
    }
  }

  // Cleanup before ending process
  pthread_attr_destroy(&initAttributes);
  pthread_attr_destroy(&RRthreads);

  return EXIT_SUCCESS;
}
