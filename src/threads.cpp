#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "../include/components/init_comp.hpp"
#include "../include/components/tools.hpp"
#include "../include/modes/animations.hpp"
#include "../include/modes/snake.hpp"
#include "../include/threads.hpp"

// TODO: check function inputs

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

void *systemStateTransitions(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  int previousState = -1;
  int currentState = -1;

  while (1) {
    pthread_mutex_lock(&c->StateMutex);
    currentState = c->SystemState;
    pthread_mutex_unlock(&c->StateMutex);

    if (c->Expander2.Button2.state) {
      c->SystemState = IDLE;
      printf("condition 0 is true\n");
    } else if (c->Expander3.Button1.state) {
      c->SystemState = RAIN;
      printf("condition 1 is true\n");
    } else if (c->Expander2.Button4.state) {
      c->SystemState = SNAKE;
      printf("condition 2 is true\n");
    } else if (c->Expander3.Button3.state) {
      c->SystemState = STOP;
      printf("condition 3 is true\n");
    }

    printf("currentState: %d\n", c->SystemState);
    pthread_mutex_unlock(&c->StateMutex);

    sleep(1);
  }
  return nullptr;
}

void *systemStateActions(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  while (1) {
    switch (c->SystemState) {
    case IDLE:
      fireworksAnimation(&c->LedArray, &c->SystemState);
      break;
    case RAIN:
      rainAnimation(&c->LedArray, &c->SystemState);
      break;
    case SNAKE:
      snakeGame(&c->LedArray, &c->SystemState, c);
      break;

    default:
      break;
    }
    usleep(100000);
  }
  return nullptr;
}

uint16_t createMaskWithZero(int pos);
uint16_t createMaskWithOne(int pos);

void *displayCube(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  // c->Cube.ledValues[linha][coluna][andar]

  uint16_t expanderVal1 = 0xFFFF;
  uint16_t expanderVal2 = 0xFFFF;
  uint16_t expanderVal3 = 0xFFFF;

  time_t startTime = time(NULL);
  time_t currentTime = time(NULL);

  if (c->Shifter1.data == 0)
    setShifterVal(&c->Shifter1, 1);

  while (c->SystemState != STOP) {
    // Interate for each floor not considering imaginary floors
    for (int andar = 0; andar < 12; andar = andar + 2) {
      // Calculate GPA_ and GPB_ for each floor
      for (int coluna = 0; coluna < 6; coluna++) {
        for (int linha = 0; linha < 6; linha++) {

          // Update values for each coluna and linha
          if (c->LedArray.ledValue[coluna][linha][andar / 2] == true) {
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
      setExpanderVal(&c->Expander1, expanderVal1);
      setExpanderVal(&c->Expander2, expanderVal2);
      setExpanderVal(&c->Expander3, expanderVal3);

      usleep(10000 / 10);

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

void *readButtons(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  uint16_t readingExp1 = 0;
  uint16_t readingExp2 = 0;
  uint16_t readingExp3 = 0;

  while (1) {
    readingExp1 = readExpander(&c->Expander1);
    readingExp2 = readExpander(&c->Expander2);
    readingExp3 = readExpander(&c->Expander3);

    // TODO: update all buttons to use the expanders instead of system
    // buttons...
    debounceButton(&c->Expander1.Button1, ~readingExp1 & 0b0100'0000'0000'0000);
    debounceButton(&c->Expander1.Button2, ~readingExp1 & 0b0000'0000'0000'0010);
    debounceButton(&c->Expander1.Button3, ~readingExp1 & 0b1000'0000'0000'0000);
    debounceButton(&c->Expander1.Button4, ~readingExp1 & 0b0000'0000'0000'0001);
    debounceButton(&c->Expander2.Button1, ~readingExp2 & 0b0100'0000'0000'0000);
    debounceButton(&c->Expander2.Button2, ~readingExp2 & 0b0000'0000'0000'0010);
    debounceButton(&c->Expander2.Button3, ~readingExp2 & 0b1000'0000'0000'0000);
    debounceButton(&c->Expander2.Button4, ~readingExp2 & 0b0000'0000'0000'0001);
    debounceButton(&c->Expander3.Button1, ~readingExp3 & 0b0100'0000'0000'0000);
    debounceButton(&c->Expander3.Button2, ~readingExp3 & 0b0000'0000'0000'0010);
    debounceButton(&c->Expander3.Button3, ~readingExp3 & 0b1000'0000'0000'0000);
    debounceButton(&c->Expander3.Button4, ~readingExp3 & 0b0000'0000'0000'0001);

    // enable and disable prints
    if (0) {
      if (c->Expander1.Button1.state) {
        printf("Button 11 is pressed\n");
      }
      if (c->Expander1.Button2.state) {
        printf("Button 12 is pressed\n");
      }
      if (c->Expander1.Button3.state) {
        printf("Button 13 is pressed\n");
      }
      if (c->Expander1.Button4.state) {
        printf("Button 14 is pressed\n");
      }
      if (c->Expander2.Button1.state) {
        printf("Button 21 is pressed\n");
      }
      if (c->Expander2.Button2.state) {
        printf("Button 22 is pressed\n");
      }
      if (c->Expander2.Button3.state) {
        printf("Button 23 is pressed\n");
      }
      if (c->Expander2.Button4.state) {
        printf("Button 24 is pressed\n");
      }
      if (c->Expander3.Button1.state) {
        printf("Button 31 is pressed\n");
      }
      if (c->Expander3.Button2.state) {
        printf("Button 32 is pressed\n");
      }
      if (c->Expander3.Button3.state) {
        printf("Button 33 is pressed\n");
      }
      if (c->Expander3.Button4.state) {
        printf("Button 34 is pressed\n");
      }
    }
    usleep(100'000 / 6);
  }

  return nullptr;
}
// Auxiliar functions

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
