#include <stdio.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"

void setExpanderVal(Expander *e, uint16_t data) {
  if (e == NULL) {
    printf("[ERROR] - Invalid parameter while setting values to expander\n");
    exit(EXIT_FAILURE);
  }

  e->valueGPA = data & 0xFF;
  e->valueGPB = (data >> 8) & 0xFF;

  if (wiringPiI2CWriteReg16(e->fd, e->portGPIOA, data) == -1) {
    printf("Error \n");
    exit(EXIT_FAILURE);
  }
  return;
}

void setShifterVal(Shifter *s, uint16_t data) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter in setShifterVal\n");
    exit(EXIT_FAILURE);
  }

  /* bool resultBool = 0; */
  for (int i = 0; i < 16; i++) {
    /* resultBool = (data & (1 << (15 - i))) ? HIGH : LOW; */
    /* digitalWrite(s->dataPin, resultBool); */
    digitalWrite(s->dataPin, (data & (1 << (15 - i))) ? HIGH : LOW);
    clockPulse(s, 1);
  }
  s->data = data;

  return;
}

void clockPulse(Shifter *s, useconds_t utime) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter in clockPulse\n");
    exit(EXIT_FAILURE);
  }
  digitalWrite(s->clockPin, HIGH);
  usleep(utime);
  digitalWrite(s->clockPin, LOW);
  return;
}

uint16_t readExpander(Expander *e) {
  if (e == NULL) {
    printf("[ERROR] - Invalid parameter in readRegisters\n");
    exit(EXIT_FAILURE);
  }

  return ((uint16_t)wiringPiI2CReadReg8(e->fd, e->portGPIOB) << 8) |
         wiringPiI2CReadReg8(e->fd, e->portGPIOA);
}

void printBinary(uint16_t value) {
  for (int i = 15; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
    if (i % 4 == 0 && i != 0) {
      printf(" ");
    }
  }
  printf("\n\n");
}

void goToNextcycle(Shifter *s) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter in cycleShifter\n");
    exit(EXIT_FAILURE);
  }

  if (s->data == 0b0000'0100'0000'0000) {
    s->data = 0b0000'0000'0000'0001;
    // remove 1 from MSB
    digitalWrite(s->dataPin, LOW);
    clockPulse(s, 10);

    // add 1 in LSB
    digitalWrite(s->dataPin, HIGH);
    clockPulse(s, 10);
    digitalWrite(s->dataPin, LOW);
    return;
  }
  s->data = s->data << 2;
  clockPulse(s, 10);
  clockPulse(s, 10);
  return;
}

bool debounceButton(Button *b, uint16_t reading) {
  if (b == NULL) {
    printf("[ERROR] - Invalid parameter in debounceButton\n");
    exit(EXIT_FAILURE);
  }
  if ((bool)reading != b->previousState) {
    b->previousState = (bool)reading;

  } else if ((bool)reading != b->state) {
    b->state = (bool)reading;
    return true;
  }
  return false;
}

void clearLedValuesArray(LedValues *l) {
  if (l == NULL) {
    printf("[ERROR] - Invalid parameter in clearLedValuesArray\n");
    exit(EXIT_FAILURE);
  }

  for (int x = 0; x < 6; x++) {
    for (int y = 0; y < 6; y++) {
      for (int z = 0; z < 6; z++) {
        l->ledValue[x][y][z] = 0;
      }
    }
  }

  return;
}
