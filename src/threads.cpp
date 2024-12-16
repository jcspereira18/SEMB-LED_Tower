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

/* void *updateLedValues(void *args) { */
/*   CubeSystem *c = (CubeSystem *)args; */
/**/
/*   for (uint16_t shi1 = 1;; shi1 = shi1 << 2, shi1++) { */
/*     setShifterVal(&c->Shifter1, shi1); */
/*     printf("Value of the shifter: "); */
/*     printBinary(shi1); */
/**/
/*     for (uint16_t exp1 = 0b0011'1111'1111'1100, exp2 =
 * 0b0011'1111'1111'1100;; */
/*          exp1 = (exp1 >> 1) & 0b0011'1111'1111'1100, exp2 = (exp2 >> 1) &
 * 0b0011'1111'1111'1100) { */
/*       setExpanderVal(&c->Expander1, exp1); */
/*       setExpanderVal(&c->Expander2, exp2); */
/*       usleep(1000000 / 10); */
/*       if (!exp1) */
/*         break; */
/*     } */
/*   } */
/*   return nullptr; */
/* } */
/**/

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

            // andar 0 - 0000'0000'0000'0001
            // andar 1 - 0000'0000'0000'0100
            // andar 2 - 0000'0000'0001'0000
            // andar 3 - 0000'0000'0100'0000
            // andar 4 - 0000'0001'0000'0000
            // andar 5 - 0000'0100'0000'0000
            //
            // shifter logic
            shifterValues1 = shifterValues1 | createMaskWithOne(andar);

            // coluna 0 - 0000'0000'0000'0100
            // coluna 1 - 0000'0000'0000'1000
            // coluna 2 - 0000'0000'0001'0000
            // coluna 3 - 0000'0000'0010'0000
            // coluna 4 - 0000'0000'0100'0000
            // coluna 5 - 0000'0000'1000'0000
            //
            // expander logic
            /* printf("DEBUG\n"); */
            /* printf("Initial expanderValues1 value: "); */
            /* printBinary(expanderValues1); */
            /* printf("Logical calculation: "); */
            expanderValues1 = expanderValues1 & createMaskWithZero(coluna + 2);

            // coluna 6 - 0000'0001'0000'0000
            // coluna 7 - 0000'0010'0000'0000
            // coluna 8 - 0000'0100'0000'0000
            // coluna 9 - 0000'1000'0000'0000
            // coluna 10 - 0001'0000'0000'0000
            // coluna 11 - 0010'0000'0000'0000
            /* expanderValues1 = expanderValues1 || !(8 << (1 << coluna)); */

            /* printf("Writing value into shifter: "); */
            /* printBinary(shifterValues1); */
            /* printf("Writing value into expander: "); */
            /* printBinary(expanderValues1); */

            setExpanderVal(&c->Expander1, expanderValues1);
            setShifterVal(&c->Shifter1, shifterValues1);

            usleep(1'000 * 5);

            setShifterVal(&c->Shifter1, 0);
            setExpanderVal(&c->Expander1, 0xFFFF);
            shifterValues1 = 0;
            expanderValues1 = 0xFFFF;
          }
        }
      }
    }
    i++;
    /* usleep(100); */
  }

  usleep(10 * 1000000);
  return nullptr;
}

//////////////// Funçõoes auxiliares

int realFloorToIndex(int floor) {
  // WARN: Do not use decimal / float values ...
  // 0 -> 0
  // 1 -> 2
  // 2 -> 4
  // 3 -> 6
  // 4 -> 8
  // 5 -> 10
  if (floor < 0 || floor > 5) {
    printf("Invalid Floor\n");
    return -1;
  }
  return floor * 2;
}

int indexToRealFloor(int index) {
  if (index < 0 || index > 10) {
    printf("Invalid Floor\n");
    return -1;
  }
  // if the index does not correspond to a floor, return the next floor
  return index % 2 == 0 ? index / 2 : index / 2 * 2 + 2;
}
