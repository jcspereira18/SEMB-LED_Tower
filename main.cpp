#include <pthread.h>
#include <sched.h>
#include <stdio.h>

#include "include/components/init_comp.hpp"
#include "include/threads.hpp"

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
  pthread_t displayCubeThread;
  pthread_t readButtonsThread;
  pthread_t updateSnakeDirectionsThreads;
  pthread_t systemTransitionsThread;
  pthread_t systemActionsThread;
  pthread_t resetThread;

  // Hardware initialization
  if (pthread_create(&initCubeSystemThread, &initAttributes, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create initCubeSystemThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);

  while (1) {

    pthread_create(&displayCubeThread, &displayAttr, displayCube, (void *)&c);
  }
  pthread_join(displayCubeThread, NULL);

  pthread_attr_destroy(&initAttributes);
  pthread_attr_destroy(&RRthreads);

  return EXIT_SUCCESS;
}
