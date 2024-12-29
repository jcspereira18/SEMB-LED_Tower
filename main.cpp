#include <pthread.h>
#include <stdio.h>

#include "include/components/init_comp.hpp"
#include "include/threads.hpp"

int main() {
  CubeSystem c; // System struct

  // Initialize Cube system
  pthread_t initCubeSystemThread;
  if (pthread_create(&initCubeSystemThread, NULL, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create initCubeSystemThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);


  // Update led display
  pthread_t displayCubeThread;
  if (pthread_create(&displayCubeThread, NULL, displayCube, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create displayCube\n");
    exit(EXIT_FAILURE);
  }

  // read Buttons
  pthread_t readButtonsThread;
  if (pthread_create(&readButtonsThread, NULL, readButtons, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create readButtonsThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_t systemTransitionsThread;
  if (pthread_create(&systemTransitionsThread, NULL, systemStateTransitions,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create readButtonsThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_t systemActionsThread;
  if (pthread_create(&systemTransitionsThread, NULL, systemStateActions,
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
  return 0;
}
// TODO: Fazer thread que quando clica em um botão, faz interrupt e liga um led!
