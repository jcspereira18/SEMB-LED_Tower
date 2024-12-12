#include <pthread.h>
#include <stdio.h>

#include "../include/components/init_comp.hpp"
#include "../include/threads.hpp"

int main() {
  CubeSystem c; // System struct

  pthread_t initCubeSystemThread, // Initialize system
      globalResetThread,          // Set global variables to 0
      customPositionThread,       // Set custom variables to test
      readButtonsThread;          // Loop to read buttons

  if (pthread_create(&initCubeSystemThread, NULL, createCubeSystem,
                     (void *)&c) != 0) {
    printf("[ERROR] - Cannot create cube system\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL);

  if (pthread_create(&globalResetThread, NULL, globalReset, (void *)&c) != 0) {
    printf("[ERROR] - Cannot create globalResetThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(globalResetThread, NULL);

  if (pthread_create(&customPositionThread, NULL, customPosition, (void *)&c) !=
      0) {
    printf("[ERROR] - Cannot create customPositionThread\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&readButtonsThread, NULL, readButtonsFunc, (void *)&c) !=
      0) {
    printf("[ERROR] - Cannot create readButtons\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(customPositionThread, NULL);
  pthread_join(readButtonsThread, NULL);

  return 0;
}
// TODO: Fazer thread que quando clica em um botão, faz interrupt e liga um led!
