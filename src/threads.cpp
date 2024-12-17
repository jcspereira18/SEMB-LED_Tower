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

  setExpanderVal(&c->Expander1, 0xFFFF);
  setExpanderVal(&c->Expander2, 0xFFFF);
  setExpanderVal(&c->Expander3, 0xFFFF);

  setShifterVal(&c->Shifter1, 0x0000);

  return nullptr;
}

void *customPosition(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  setExpanderVal(&c->Expander1, 0xFFFF); // U1
  setExpanderVal(&c->Expander2, 0xFFFF); // U2
  setExpanderVal(&c->Expander3, 0xFFFF); // U3

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

uint16_t createMaskWithZero(int pos) {
  // Ensure the position is within the valid range for 16 bits
  if (pos < 0 || pos > 15) {
    return 0xFFFF; // Return all ones if position is invalid
  }

  // Create a mask with a single 0 at the given position
  return (uint16_t)(~(1 << pos));
}

uint16_t createMaskWithOne(int pos) {
  // Ensure the position is within the valid range for 16 bits
  if (pos < 0 || pos > 15) {
    return 0x0000; // Return all zeros if position is invalid
  }

  // Create a mask with a single 1 at the given position
  return (uint16_t)(1 << pos);
}

void *updateLedStatus(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  c->Cube.ledValues[0][0][0] = true;
  c->Cube.ledValues[0][1][0] = true;
  c->Cube.ledValues[0][2][0] = true;
  c->Cube.ledValues[0][3][0] = true;
  c->Cube.ledValues[0][4][0] = true;
  c->Cube.ledValues[0][5][0] = true;
  c->Cube.ledValues[1][0][0] = true;
  c->Cube.ledValues[2][0][0] = true;
  c->Cube.ledValues[3][0][0] = true;
  c->Cube.ledValues[4][0][0] = true;
  c->Cube.ledValues[5][0][0] = true;
  c->Cube.ledValues[5][0][0] = true;
  c->Cube.ledValues[0][5][0] = true;
  c->Cube.ledValues[1][5][0] = true;
  c->Cube.ledValues[2][5][0] = true;
  c->Cube.ledValues[3][5][0] = true;
  c->Cube.ledValues[4][5][0] = true;
  c->Cube.ledValues[5][0][0] = true;
  c->Cube.ledValues[5][1][0] = true;
  c->Cube.ledValues[5][2][0] = true;
  c->Cube.ledValues[5][3][0] = true;
  c->Cube.ledValues[5][4][0] = true;
  c->Cube.ledValues[5][5][0] = true;

  uint16_t shifterValues1 = 0;
  uint16_t expanderValues1 = 0xFFFF;
  int i = 0;
  while (i < 100000) {
    for (int coluna = 0; coluna < 6; coluna++) {
      for (int linha = 0; linha < 2; linha++) {
        for (int andar = 0; andar <= 10; andar = andar + 2) {
          if (c->Cube.ledValues[andar / 2][coluna][linha] == 1) {
            printf("\nUpdating Cube[%d][%d][%d]\n", andar / 2, coluna, linha);

            shifterValues1 = shifterValues1 | createMaskWithOne(andar);
            expanderValues1 = expanderValues1 & createMaskWithZero(coluna + 2);
          }
        }
        // Andar
        // finished calculating values to shifter, showing all floor
        setShifterVal(&c->Shifter1, shifterValues1);
        setExpanderVal(&c->Expander1, expanderValues1);

        /* sleep(1); */
        usleep(100'000);
        setShifterVal(&c->Shifter1, shifterValues1);
        setExpanderVal(&c->Expander1, expanderValues1);
        shifterValues1 = 0;
        expanderValues1 = 0xFFFF;
      }
      // Linha
    }
    // Coluna

    // Reset values

    i++;
    /* usleep(1000000); */
  }

  usleep(10000000);
  return nullptr;
}
