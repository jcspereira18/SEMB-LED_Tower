#include <stdio.h>
#include <time.h>

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

void *animationCube1(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  time_t startTime = time(NULL), currentTime = time(NULL);

  // Clear all LEDs
  for (int z = 0; z < 6; z++) {
    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        c->Cube.ledValues[x][y][z] = false;
      }
    }
  }
  // Loop through all layers
  for (int t = 0; difftime(currentTime, startTime) <= 30; t++) {
    // Randomly generate raindrops on the top layer (z = 5)
    for (int i = 0; i < 4;
         i++) { // `dropRate` controls how many drops are added per frame
      int x = rand() % 6;
      int y = rand() % 6;
      c->Cube.ledValues[x][y][5] = true; // Add a drop at the top layer
    }

    // Move all drops down one layer
    for (int z = 0; z < 5; z++) { // Iterate from bottom to second-to-last layer
      for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 6; y++) {
          if (c->Cube.ledValues[x][y][z + 1]) {
            c->Cube.ledValues[x][y][z] = true;      // Move the drop down
            c->Cube.ledValues[x][y][z + 1] = false; // Turn off the old position
          }
        }
      }
    }

    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        if (c->Cube.ledValues[x][y][0]) {
          // Randomly decide whether to remove the drop in the bottom layer
          if ((rand() % 100) < (50)) {
            c->Cube.ledValues[x][y][0] = false; // Remove the drop
          }
        }
      }
    }
    usleep(100000);
    currentTime = time(NULL);
  }

  return nullptr;
}

void *updateLedStatus(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  // c->Cube.ledValues[linha][coluna][andar]

  uint16_t shifterValues1 = 0;
  uint16_t expanderVal1 = 0xFFFF;
  uint16_t expanderVal2 = 0xFFFF;
  uint16_t expanderVal3 = 0xFFFF;

  time_t startTime = time(NULL);
  time_t currentTime = time(NULL);

  while (difftime(currentTime, startTime) <= 30) {
    currentTime = time(NULL);

    // Interate for each floor not considering imaginary floors
    for (int andar = 0; andar < 12; andar = andar + 2) {
      // Calculate GPA_ and GPB_ for each floor
      for (int coluna = 0; coluna < 6; coluna++) {
        for (int linha = 0; linha < 6; linha++) {

          // Update values for each coluna and linha
          if (c->Cube.ledValues[coluna][linha][andar / 2] == true) {
            /* printf("\nUpdating Cube[%d][%d][%d]\n", linha, coluna, andar /
             * 2); */
            if (coluna < 2) {
              expanderVal1 =
                  expanderVal1 &
                  createMaskWithZero(coluna % 2 == 0 ? linha + 8 : linha + 2);
            } else if (coluna < 4) {
              expanderVal2 =
                  expanderVal2 &
                  createMaskWithZero(coluna % 2 == 0 ? linha + 8 : linha + 2);
            } else {
              expanderVal3 =
                  expanderVal3 &
                  createMaskWithZero(coluna % 2 == 0 ? linha + 8 : linha + 2);
            }
          }
        }
      }
      // delay dos cálculos + manter leds ativos
      /* printf("\n\nAndar: %d\n", andar / 2); */
      setExpanderVal(&c->Expander1, expanderVal1);
      setExpanderVal(&c->Expander2, expanderVal2);
      setExpanderVal(&c->Expander3, expanderVal3);
      /* usleep(1000 * 1000); */
      usleep(10000 / 6);
      // baixo: 2
      // medio baixo: 4
      // menos bom: 7
      // bom 8

      // reset
      expanderVal1 = 0xFFFF;
      expanderVal2 = 0xFFFF;
      expanderVal3 = 0xFFFF;

      setExpanderVal(&c->Expander1, expanderVal1);
      setExpanderVal(&c->Expander2, expanderVal2);
      setExpanderVal(&c->Expander3, expanderVal3);
      goToNextcycle(&c->Shifter1);
    }
  }
  return nullptr;
}
