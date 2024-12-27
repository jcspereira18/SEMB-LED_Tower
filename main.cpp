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

  pthread_t rainAnimationThread;
  if (pthread_create(&rainAnimationThread, NULL, rainAnimation, (void *)&c) !=
      0) {
    printf("[ERROR] - Cannot create rainAnimationThread\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(displayCubeThread, NULL);

  return 0;
}
// TODO: Fazer thread que quando clica em um botão, faz interrupt e liga um led!
