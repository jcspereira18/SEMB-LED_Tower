#include <stdio.h>
#include <string.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"

void initCubeSystem(CubeSystem *s) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter while initializing cube system\n");
    exit(EXIT_FAILURE);
  }
  // initialize Cube with 0's
  memset(s->Cube.ledValues, 0, sizeof(s->Cube.ledValues));
  memset(s->Cube.toUpdateVal, 0, sizeof(s->Cube.toUpdateVal));

  // initialize buttons
  // 1 -> 4
  // 2 -> 2
  // 3 -> 1
  // 4 -> 3
    
  // TODO: Change this initializations
  // BUG: Buttons are not correctly associated
  initButton(&s->Button11, 0b1000'0000'0000'0000);
  initButton(&s->Button12, 0b0000'0010);
  initButton(&s->Button21, 0b0000'0001);
  initButton(&s->Button22, 0b0100'0000'0000'0000);

  initButton(&s->Button13, 0b0000'0001);
  initButton(&s->Button14, 0b0000'0010);
  initButton(&s->Button23, 0b0100'0000);
  initButton(&s->Button24, 0b1000'0000);

  initButton(&s->Button15, 0b0000'0001);
  initButton(&s->Button16, 0b0000'0010);
  initButton(&s->Button25, 0b0100'0000);
  initButton(&s->Button26, 0b1000'0000);

  // link expander to button
  s->Expander1.Button1 = &s->Button21;
  s->Expander1.Button2 = &s->Button12;
  s->Expander1.Button3 = &s->Button22;
  s->Expander1.Button4 = &s->Button11;

  s->Expander2.Button1 = &s->Button13;
  s->Expander2.Button2 = &s->Button14;
  s->Expander2.Button3 = &s->Button23;
  s->Expander2.Button4 = &s->Button24;

  s->Expander3.Button1 = &s->Button15;
  s->Expander3.Button2 = &s->Button16;
  s->Expander3.Button3 = &s->Button25;
  s->Expander3.Button4 = &s->Button26;

  // initialize expanders
  initExpander(&s->Expander1, MCP23017_I2C_ADDRESS_1, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);
  initExpander(&s->Expander2, MCP23017_I2C_ADDRESS_2, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);
  initExpander(&s->Expander3, MCP23017_I2C_ADDRESS_3, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);

  // initialize shifters
  initShifter(&s->Shifter1, DATA_PIN, CLOCK_PIN, 0x0000);

  return;
}

void initExpander(Expander *e, uint8_t I2CAddress, uint8_t IOdirA,
                  uint8_t IOdirB, uint8_t portGPIOA, uint8_t portGPIOB,
                  uint8_t portGPPUA, uint8_t portGPPUB, uint8_t inputsDirA,
                  uint8_t inputsDirB, uint8_t initialValueGPA,
                  uint8_t initialValueGPB) {

  if (e == NULL) {
    printf("[ERROR] - Invalid parameter while initializing expander\n");
    exit(EXIT_FAILURE);
  }

  // Initialize MCP23017 on I2C address
  e->I2CAddress = I2CAddress;
  e->fd = wiringPiI2CSetup(e->I2CAddress);
  if (e->fd == -1) {
    printf("[ERROR] - Cannot initialize expander with %d Address\n",
           e->I2CAddress);
    exit(EXIT_FAILURE);
  }

  // Port A and B Setup
  // Set Port A direction Inputs and Outputs
  e->IOdirA = IOdirA;
  e->IOdirB = IOdirB;
  e->inputsDirA = inputsDirA;
  e->inputsDirB = inputsDirB;
  if (wiringPiI2CWriteReg8(e->fd, e->IOdirA, e->inputsDirA) == -1) {
    printf("[ERROR] - Cannot set IO for Port A direction\n");
    exit(EXIT_FAILURE);
  }
  if (wiringPiI2CWriteReg8(e->fd, e->IOdirB, e->inputsDirB) == -1) {
    printf("[ERROR] - Cannot set IO for Port B direction\n");
    exit(EXIT_FAILURE);
  }
  e->portGPPUA = portGPPUA;
  e->portGPPUB = portGPPUB;
  e->inputsDirA = inputsDirA;
  e->inputsDirB = inputsDirB;
  if (wiringPiI2CWriteReg8(e->fd, e->portGPPUA, e->inputsDirA) == -1) {
    printf("[ERROR] - Cannot set IO for Port A direction\n");
    exit(EXIT_FAILURE);
  }
  if (wiringPiI2CWriteReg8(e->fd, e->portGPPUB, e->inputsDirB) == -1) {
    printf("[ERROR] - Cannot set IO for Port B direction\n");
    exit(EXIT_FAILURE);
  }

  e->portGPIOA = portGPIOA;
  e->portGPIOB = portGPIOB;

  // Set initial values
  setExpanderVal(e, (uint16_t(initialValueGPA) << 8) || initialValueGPB);
  printf("[INFO] - Sucessfully initialized expander\n");

  return;
}

void initShifter(Shifter *s, uint16_t dataPin, uint16_t clockPin,
                 uint16_t initialData) {

  if (s == NULL) {
    printf("[ERROR] - Invalid parameter while initializing shifter\n");
    exit(EXIT_FAILURE);
  }

  s->dataPin = dataPin;
  s->clockPin = clockPin;

  if (wiringPiSetupGpio() == -1) {
    printf("[ERROR] - Cannot initialize shifter\n");
    exit(EXIT_FAILURE);
  }
  pinMode(s->dataPin, OUTPUT);
  pinMode(s->clockPin, OUTPUT);
  digitalWrite(s->dataPin, LOW);
  digitalWrite(s->clockPin, LOW);

  return;
}

void initButton(Button *b, uint16_t pin) {
  if (b == NULL) {
    printf("[ERROR] - Invalid parameter while initializing button \n");
    exit(EXIT_FAILURE);
  }
  // Initial values
  b->state = LOW;
  b->pin = pin;
  return;
}

