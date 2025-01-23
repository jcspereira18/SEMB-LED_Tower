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

void newFireworksAnimation(CubeSystem *c);
void newRainAnimation(CubeSystem *c);

void systemStateTransitions(CubeSystem *c) {

  int previousState = -1;
  int currentState = -1;

  pthread_mutex_lock(&c->StateMutex);
  currentState = c->SystemState;
  pthread_mutex_unlock(&c->StateMutex);

  if (c->Expander2.Button2.state) {
    c->SystemState = IDLE;
    clearLedValuesArray(&c->LedArray);
    /* printf("condition 0 is true\n"); */
  } else if (c->Expander3.Button1.state) {
    clearLedValuesArray(&c->LedArray);
    c->SystemState = RAIN;
    /* printf("condition 1 is true\n"); */
  } else if (c->Expander2.Button4.state) {
    clearLedValuesArray(&c->LedArray);
    c->SystemState = SNAKE;
    /* printf("condition 2 is true\n"); */
  } else if (c->Expander3.Button3.state) {
    clearLedValuesArray(&c->LedArray);
    c->SystemState = STOP;
    /* printf("condition 3 is true\n"); */
  }

  /* printf("currentState: %d\n", c->SystemState); */
  pthread_mutex_unlock(&c->StateMutex);

  return;
}

void systemStateActions(CubeSystem *c) {
  static time_t lastSnakeExecutionTime =
      0; // Tracks last execution time for SNAKE
  time_t currentTime;

  // Get the current time (UNIX timestamp, seconds)
  time(&currentTime);

  switch (c->SystemState) {
  case IDLE:
    newFireworksAnimation(c); // Always runs immediately
    break;

  case RAIN:
    newRainAnimation(c); // Always runs immediately
    break;

  case SNAKE:
    if (currentTime - lastSnakeExecutionTime >= 1) {
      lastSnakeExecutionTime = currentTime; // Update the last execution time
      snakeGame(&c->LedArray, &c->SystemState, c);
    }
    break;

  default:
    break;
  }
}

uint16_t createMaskWithZero(int pos);
uint16_t createMaskWithOne(int pos);

void displayCube(CubeSystem *c) {
  uint16_t expanderVal1 = 0xFFFF;
  uint16_t expanderVal2 = 0xFFFF;
  uint16_t expanderVal3 = 0xFFFF;

  if (c->Shifter1.data != 1)
    setShifterVal(&c->Shifter1, 1);

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

    // mantain layer with 1ms
    usleep(1000);

    // reset
    expanderVal1 = 0xFFFF;
    expanderVal2 = 0xFFFF;
    expanderVal3 = 0xFFFF;

    setExpanderVal(&c->Expander1, expanderVal1);
    setExpanderVal(&c->Expander2, expanderVal2);
    setExpanderVal(&c->Expander3, expanderVal3);

    goToNextcycle(&c->Shifter1);
  }
  return;
}

void readButtons(CubeSystem *c) {

  uint16_t readingExp1 = 0;
  uint16_t readingExp2 = 0;
  uint16_t readingExp3 = 0;

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
  if (FALSE) {
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

  return;
}

int getDirectionFromInput(CubeSystem *c) {
  if (c->Expander1.Button1.state)
    return 5; // -z
  else if (c->Expander1.Button2.state)
    return 2; // +y
  else if (c->Expander1.Button3.state)
    return 1; // -x
  else if (c->Expander1.Button4.state)
    return 3; // -y
  else if (c->Expander2.Button1.state)
    return 4; // +z
  else if (c->Expander2.Button3.state)
    return 0; // +x
  else
    return -1; // No button pressed
}

void updateSnakeDirection(CubeSystem *c) {
  int newDirection = getDirectionFromInput(c);

  if (newDirection != -1) { // Update only if a button is pressed
    pthread_mutex_lock(&c->directionMutex);
    c->SnakeDirection = newDirection;
    pthread_mutex_unlock(&c->directionMutex);
  }
  return;
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

void newRainAnimation(CubeSystem *c) {
  for (int i = 0; i < 4; i++) {
    int x = rand() % 6;
    int y = rand() % 6;
    c->LedArray.ledValue[x][y][5] = true;
  }

  for (int z = 0; z < 5; z++) {
    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        if (c->LedArray.ledValue[x][y][z + 1]) {
          c->LedArray.ledValue[x][y][z] = true;
          c->LedArray.ledValue[x][y][z + 1] = false;
        }
      }
    }
  }

  for (int x = 0; x < 6; x++) {
    for (int y = 0; y < 6; y++) {
      if (c->LedArray.ledValue[x][y][0]) {
        if ((rand() % 100) < (50)) {
          c->LedArray.ledValue[x][y][0] = false;
        }
      }
    }
  }

  return;
}

void newFireworksAnimation(CubeSystem *c) {
  static int phase = 0; // Tracks the current phase of the animation
  static int start_x, start_y,
      start_z;                 // Random starting point for firework burst
  static int radius = 0;       // Current radius of the burst
  static int sparkleFrame = 0; // Tracks the number of sparkle frames

  // Clear the LED array at the beginning of each phase
  if (phase == 0) {
    clearLedValuesArray(&c->LedArray);

    // Generate a random starting point for the firework burst
    start_x = rand() % 6;
    start_y = rand() % 6;
    start_z = rand() % 6;

    // Light up the initial vertical column
    for (int z = 0; z <= start_z; z++) {
      c->LedArray.ledValue[start_x][start_y][z] = true;
    }

    phase = 1; // Move to the radial burst phase
    return;
  }

  // Handle the radial burst
  if (phase == 1) {
    clearLedValuesArray(
        &c->LedArray); // Clear the LED array for the current frame

    // Draw the expanding burst
    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        for (int z = 0; z < 6; z++) {
          int distance = abs(x - start_x) + abs(y - start_y) + abs(z - start_z);
          if (distance == radius) {
            c->LedArray.ledValue[x][y][z] = true;
          }
        }
      }
    }

    radius++; // Increase the radius for the next frame

    if (radius >= 6) {
      phase = 2;        // Move to the sparkle phase
      sparkleFrame = 0; // Reset sparkle frame counter
    }
    return;
  }

  // Handle the sparkle effect
  if (phase == 2) {
    clearLedValuesArray(
        &c->LedArray); // Clear the LED array for the current frame

    // Create random sparkles around the starting point
    for (int j = 0; j < 10; j++) { // Number of sparkles per frame
      int sparkle_x = start_x + (rand() % 3 - 1); // Random offset -1, 0, 1
      int sparkle_y = start_y + (rand() % 3 - 1);
      int sparkle_z = start_z + (rand() % 3 - 1);

      // Ensure the sparkles are within the cube bounds
      if (sparkle_x >= 0 && sparkle_x < 6 && sparkle_y >= 0 && sparkle_y < 6 &&
          sparkle_z >= 0 && sparkle_z < 6) {
        c->LedArray.ledValue[sparkle_x][sparkle_y][sparkle_z] = true;
      }
    }

    sparkleFrame++;
    if (sparkleFrame >= 5) { // After 5 sparkle frames, reset the animation
      phase = 0;
      radius = 0;
    }
    return;
  }
}

void *micro1(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  displayCube(c);

  return nullptr;
}

void *micro2(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  readButtons(c);
  updateSnakeDirection(c);
  systemStateTransitions(c);

  return nullptr;
}

void *micro3(void *args) {
  CubeSystem *c = (CubeSystem *)args;

  systemStateActions(c);
  return nullptr;
}
