#include <stdio.h>

#include "../include/components/init_comp.hpp"
#include "../include/components/tools.hpp"
#include "../include/threads.hpp"

void *createCubeSystem(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  initCubeSystem(c);
  return nullptr;
}

void *globalReset(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  setExpanderVal(&c->Expander1, 0x0000);
  setExpanderVal(&c->Expander2, 0x0000);
  setExpanderVal(&c->Expander3, 0x0000);

  setShifterVal(&c->Shifter1, 0x0000);

  return nullptr;
}

void *customPosition(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  setExpanderVal(&c->Expander1, 0x0000); // U1
  setExpanderVal(&c->Expander2, 0x0000); // U2
  setExpanderVal(&c->Expander3, 0x0000); // U3

  setShifterVal(&c->Shifter1, 0x0000); // U4_U5

  return nullptr;
}

void *readButtonsFunc(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  int i = 0;
  printf("\n");
  while (1) {
    i++;
    readButtons(&c->Expander1);
    readButtons(&c->Expander2);
    readButtons(&c->Expander3);
    usleep(100000);
  }
}
