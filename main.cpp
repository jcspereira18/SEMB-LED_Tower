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
  pthread_t micro4thread;
  // Hardware initialization
  // Runs once
  // WARN: one-shot thread!

  if (pthread_create(&initCubeSystemThread, &initAttributes, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create initCubeSystemThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);

  struct timespec loopStart, loopEnd, lastExec3, currentTime;
  double loopTime = 0;
  double exec3Time = 0;

  clock_gettime(CLOCK_MONOTONIC, &lastExec3);

  while (c.SystemState != STOP) {

    clock_gettime(CLOCK_MONOTONIC, &loopStart);
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    pthread_create(&micro1thread, &displayAttr, micro1, (void *)&c);
    pthread_create(&micro2thread, &RRthreads, micro2, (void *)&c);

    exec3Time = (currentTime.tv_sec - lastExec3.tv_sec) +
                (currentTime.tv_nsec - lastExec3.tv_nsec) / 1.0e9;

    if (exec3Time >= 0.15) {
      pthread_create(&micro3thread, &RRthreads, micro3, (void *)&c);
      pthread_join(micro3thread, NULL);
      clock_gettime(CLOCK_MONOTONIC, &lastExec3);
    }

    pthread_join(micro1thread, NULL);
    pthread_join(micro2thread, NULL);

    clock_gettime(CLOCK_MONOTONIC, &loopEnd);

    loopTime = (loopEnd.tv_sec - loopStart.tv_sec) +
               (loopEnd.tv_nsec - loopStart.tv_nsec) / 1.0e9;

    if (loopTime < 0.025) {
      printf("loopTime: %.6f\n", loopTime);
      printf("Sleeping for: %.6f seconds\n", (0.025 - loopTime));
      usleep((0.025 - loopTime) * 1e6);
    }
  }

  // Cleanup before ending process
  pthread_attr_destroy(&initAttributes);
  pthread_attr_destroy(&RRthreads);

  return EXIT_SUCCESS;
}
