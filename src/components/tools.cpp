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

  bool resultBool = 0;
  for (int i = 0; i < 16; i++) {
    resultBool = (data & (1 << (15 - i))) ? HIGH : LOW;
    digitalWrite(s->dataPin, resultBool);
    // WARN: Calibrate delay!
    /* clockPulse(s, 100 * 0.0001); */
    clockPulse(s, 100 / 2);
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

uint16_t readRegisters(Expander *e) {
  if (e == NULL) {
    printf("[ERROR] - Invalid parameter in readRegisters\n");
    exit(EXIT_FAILURE);
  }

  return ((uint16_t)wiringPiI2CReadReg8(e->fd, e->portGPIOB) << 8) |
         wiringPiI2CReadReg8(e->fd, e->portGPIOA);
}

void readButtons(Expander *e) {
  if (e == NULL) {
    printf("[ERROR] - Invalid parameter in updateButton\n");
    exit(EXIT_FAILURE);
  }

  uint16_t data = readRegisters(e);
 // printBinary(data);

  //printBinary(e->Button1->pin);
  //printBinary(e->Button1->pin && wiringPiI2CReadReg8(e->fd, e->portGPIOA));

  return;
}

void printBinary(uint16_t value) {
  for (int i = 15; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
    if (i % 4 == 0 && i != 0) {
      printf(" ");
    }
  }
  printf("\n");
}
