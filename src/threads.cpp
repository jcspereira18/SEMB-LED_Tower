#include <pthread.h>
#include <stdio.h>

#include "../include/system.hpp"
#include "../include/threads.hpp"

void *createCubeSystem(void *args) {
  CubeSystem *cubeSystem = (CubeSystem *)args;

  initCubeSystem(cubeSystem);
  printf("[INFO] - Successfully created cube system\n");
  return nullptr;
}

void *globalReset(void *args) {
  CubeSystem *cubeSystem = (CubeSystem *)args;

  setExpanderVal(&cubeSystem->Expander1, 0xFFFF);
  setExpanderVal(&cubeSystem->Expander2, 0xFFFF);
  setExpanderVal(&cubeSystem->Expander3, 0xFFFF);

  setShifterVal(&cubeSystem->Shifter1, 0b0000'0000'0000'0000);
  printf("[INFO] - Successfully reset the system globally\n");

  return nullptr;
}

void *customPosition(void *args) {
  CubeSystem *cubeSystem = (CubeSystem *)args;

  setExpanderVal(&cubeSystem->Expander1, 0b0011'1111'0000'0000); // U1
  setExpanderVal(&cubeSystem->Expander2, 0b0000'0000'0000'0000); // U2
  setExpanderVal(&cubeSystem->Expander3, 0b0000'0000'0000'0000); // U3

  setShifterVal(&cubeSystem->Shifter1,
                0b0000'0000'0000'0000); // U4_QA HIGH // Handling U4 and U5
  printf("[INFO] - Successfully set the custom position\n");
  return nullptr;
}

void *readButtons(void *args) {
  CubeSystem *cubeSystem = (CubeSystem *)args;

  int i = 0;
  printf("\n");
  while (1) {
    printf("[index] - %d\n", i);
    printf("[INFO] - Values read from expander1: ");
    printBinary(readRegisters(&cubeSystem->Expander1));
    printf("[INFO] - Values read from expander2: ");
    printBinary(readRegisters(&cubeSystem->Expander2));
    printf("[INFO] - Values read from expander3: ");
    printBinary(readRegisters(&cubeSystem->Expander3));
    printf("\n");
    i++;
    usleep(1000000);
  }
}
