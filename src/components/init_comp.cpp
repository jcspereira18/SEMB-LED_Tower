#include <pthread.h>
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
  memset(s->LedArray.ledValue, 0, sizeof(s->LedArray.ledValue));

  // BUG: Buttons are not correctly associated
  initButton(&s->Expander1.Button1, 0);
  initButton(&s->Expander1.Button2, 0);
  initButton(&s->Expander1.Button3, 0);
  initButton(&s->Expander1.Button4, 0);

  initButton(&s->Expander2.Button1, 0);
  initButton(&s->Expander2.Button2, 0);
  initButton(&s->Expander2.Button3, 0);
  initButton(&s->Expander2.Button4, 0);

  initButton(&s->Expander3.Button1, 0);
  initButton(&s->Expander3.Button2, 0);
  initButton(&s->Expander3.Button3, 0);
  initButton(&s->Expander3.Button4, 0);

  // initialize expanders
  initExpander(&s->Expander1, MCP23017_I2C_ADDRESS_1, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);
  initExpander(&s->Expander2, MCP23017_I2C_ADDRESS_2, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);
  initExpander(&s->Expander3, MCP23017_I2C_ADDRESS_3, IODIRA, IODIRB, GPIOA,
               GPIOB, GPPUA, GPPUB, 0b0000'0011, 0b1100'0000, 0x00, 0x00);

  // initialize shifters
  initShifter(&s->Shifter1, DATA_PIN, CLOCK_PIN, 0x0001);

  s->SystemState = IDLE;
  s->StateMutex = PTHREAD_MUTEX_INITIALIZER;

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

  setShifterVal(s, initialData);

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
