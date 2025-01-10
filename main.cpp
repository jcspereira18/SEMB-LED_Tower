#include <pthread.h>
#include <sched.h>
#include <stdio.h>

#include "include/components/init_comp.hpp"
#include "include/threads.hpp"

int getDirectionFromInput(CubeSystem *c) {
  if (c->Expander1.Button1.state)
    return 5; // -z
  else if (c->Expander1.Button2.state)
    return 2; // +y
  else if (c->Expander1.Button3.state)
    return 1; // -x
  else if (c->Expander1.Button4.state)
    return 3; // -y
  else if (c->Expander2.Button1.state)
    return 4; // +z
  else if (c->Expander2.Button3.state)
    return 0; // +x
  else
    return -1; // No button pressed
}

void *updateSnakeDirection(void *arg) {
  CubeSystem *c = (CubeSystem *)arg;

  while (true) {
    int newDirection = getDirectionFromInput(c);

    if (newDirection != -1) { // Update only if a button is pressed
      pthread_mutex_lock(&c->directionMutex);
      c->SnakeDirection = newDirection;
      pthread_mutex_unlock(&c->directionMutex);
    }
  }
  return nullptr;
}

int main() {
  CubeSystem c; // System struct

  // Scheduliing attributes
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

  // Initialize Cube system
  pthread_t initCubeSystemThread;
  if (pthread_create(&initCubeSystemThread, &initAttributes, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create initCubeSystemThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);

  // Update led display
  pthread_t displayCubeThread;
  if (pthread_create(&displayCubeThread, &displayAttr, displayCube, (void *)&c) !=
      0) {
    printf("[ERROR] - Cannot create displayCube\n");
    exit(EXIT_FAILURE);
  }

  // read Buttons
  pthread_t readButtonsThread;
  if (pthread_create(&readButtonsThread, &RRthreads, readButtons, (void *)&c) !=
      0) {
    printf("[ERROR] - Cannot create readButtonsThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_t updateSnakeDirectionsThreads;
  if (pthread_create(&updateSnakeDirectionsThreads, &RRthreads,
                     updateSnakeDirection, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create updateSnakeDirectionsThreads\n");
    exit(EXIT_FAILURE);
  }

  pthread_t systemTransitionsThread;
  if (pthread_create(&systemTransitionsThread, &RRthreads,
                     systemStateTransitions, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create readButtonsThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_t systemActionsThread;
  if (pthread_create(&systemTransitionsThread, &RRthreads, systemStateActions,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create systemActionsThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_join(displayCubeThread, NULL);

  // reset expanders and shifters to exit program
  pthread_t resetThread;
  if (pthread_create(&resetThread, NULL, globalReset, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create resetThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(resetThread, NULL);

  pthread_attr_destroy(&initAttributes);
  pthread_attr_destroy(&RRthreads);

  return EXIT_SUCCESS;
}
