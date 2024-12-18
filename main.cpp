#include <pthread.h>
#include <stdio.h>

#include "include/components/init_comp.hpp"
#include "include/threads.hpp"

void create_thread(pthread_t *thread, void *(*start_routine)(void *), void *arg,
                   const char *error_message) {
  if (pthread_create(thread, NULL, start_routine, arg) != 0) {
    printf("[ERROR] - %s\n", error_message);
    exit(EXIT_FAILURE);
  }
}

int main() {
  CubeSystem c; // System struct

  pthread_t initCubeSystemThread, // Initialize system
      globalResetThread,          // Set global variables to
      updateLedStatusThread,      // Update LED status
      animationCube1Thread;       // Set values of cube to create animation

  // Initialize Cube System
  create_thread(&initCubeSystemThread, createCubeSystem, (void *)&c,
                "Cannot create cube system thread");
  pthread_join(initCubeSystemThread, NULL);

  // Update LED Status
  create_thread(&updateLedStatusThread, updateLedStatus, (void *)&c,
                "Cannot create update LED status thread");
  create_thread(&animationCube1Thread, animationCube1, (void *)&c,
                "Cannot create update LED status thread");

  pthread_join(updateLedStatusThread, NULL);
  pthread_join(animationCube1Thread, NULL);

  // Reset Cube before exiting program
  create_thread(&globalResetThread, globalReset, (void *)&c,
                "Cannot create global reset thread");
  pthread_join(updateLedStatusThread, NULL);

  return 0;
}
// TODO: Fazer thread que quando clica em um botão, faz interrupt e liga um led!
