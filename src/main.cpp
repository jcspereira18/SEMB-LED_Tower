#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// #include "../include/main.hpp"
#include "../include/system.hpp"
#include "../include/threads.hpp"

int main() {
  CubeSystem cubeSystem;

  pthread_t initCubeSystemThread; // Initializing Cube System
  if (pthread_create(&initCubeSystemThread, NULL, createCubeSystem,
                     (void *)&cubeSystem) != 0) {
    printf("[ERROR] - Cannot create cube system\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(initCubeSystemThread, NULL); // wait thread to finish

  pthread_t globalResetThread; // Reseting global State of the Cube
  if (pthread_create(&globalResetThread, NULL, globalReset,
                     (void *)&cubeSystem) != 0) {
    printf("[ERROR] - Cannot create globalResetThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(globalResetThread, NULL);

  // Following 2 threads are runnning cuncurently

  pthread_t debugPrintsThread; // Print Cube status
  if (pthread_create(&debugPrintsThread, NULL, debugPrints,
                     (void *)&cubeSystem) != 0) {
    printf("[ERROR] - Cannot create debugPrintsThread\n");
    exit(EXIT_FAILURE);
  } // Dont want to wait function to end ...

  pthread_t customPositionThread; // Set custom position
  if (pthread_create(&customPositionThread, NULL, customPosition,
                     (void *)&cubeSystem) != 0) {
    printf("[ERROR] - Cannot create customPositionThread\n");
    exit(EXIT_FAILURE);
  }

  pthread_t readButtonsThread;
  if (pthread_create(&readButtonsThread, NULL, readButtons,
                     (void *)&cubeSystem) != 0) {
    printf("[ERROR] - Cannot create readButtons\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(customPositionThread, NULL);
  pthread_join(debugPrintsThread, NULL);

  // WARN: Fazer thread que quando clica em um botão, faz interrupt e liga um
  // led!
  printf("Program runned all tasks. Exiting now.\n");
  return 0;
}
