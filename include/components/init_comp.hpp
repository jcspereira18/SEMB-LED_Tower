#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <wiringPi.h>
#include <wiringPiI2C.h>

// Cube dimensions
#define ANDARES 11 // 6 reais + 5 imaginários
#define COLUNAS 6
#define LINHAS 6

// Expander defines
#define MCP23017_I2C_ADDRESS_1 0x24
#define MCP23017_I2C_ADDRESS_2 0x26
#define MCP23017_I2C_ADDRESS_3 0x27

#define IODIRA 0x00 // Registro de direção do Port A
#define IODIRB 0x01 // Registro de direção do Port B
#define GPIOA 0x12  // Registro GPIO Port A
#define GPIOB 0x13  // Registro GPIO Port B
#define OLATA 0x14  // Registro de latch de saída Port A
#define OLATB 0x15  // Registro de latch de saída Port B
#define GPPUA 0x0C  // Pull-up Resistor Configuration for Port A
#define GPPUB 0x0D  // Pull-up Resistor Configuration for Port B

// Shifter defines
#define DATA_PIN 17
#define CLOCK_PIN 4

typedef struct {
  unsigned char ledValues[ANDARES][COLUNAS][LINHAS];
  unsigned char toUpdateVal[ANDARES][COLUNAS][LINHAS];
} LedCube;

// TODO: Debouce buttons
typedef struct {
  bool state;
  uint16_t pin;
} Button;

// MCP23017
typedef struct {
  int fd;             // File descriptor for I2C
  uint8_t I2CAddress; // I2C address
  uint8_t portGPIOA;  // GPIO register for Port A
  uint8_t portGPIOB;  // GPIO register for Port B
  uint8_t IOdirA;     // I/O direction register for Port A
  uint8_t IOdirB;     // I/O direction register for Port B
  uint8_t inputsDirA; // Input pins for Port A
  uint8_t inputsDirB; // Input pins for Port B
  uint8_t portGPPUA;  // Pull-up resistor configuration for Port A
  uint8_t portGPPUB;  // Pull-up resistor configuration for Port B
  uint8_t valueGPA;   // Pin values for Port A
  uint8_t valueGPB;   // Pin values for Port B
  Button *Button1;
  Button *Button2; // [1] [2]
  Button *Button3; // [3] [4]
  Button *Button4;
} Expander;

// 74HC164
typedef struct {
  int dataPin;
  int clockPin;
  uint16_t data;
} Shifter;

typedef struct {
  LedCube Cube;
  Expander Expander1; // U1
  Expander Expander2; // U2
  Expander Expander3; // U3
  Shifter Shifter1;   // U4 & U5
  Button Button11;
  Button Button12;
  Button Button13;
  Button Button14;
  Button Button15;
  Button Button16;
  Button Button21;
  Button Button22;
  Button Button23;
  Button Button24;
  Button Button25;
  Button Button26;
} CubeSystem;

void initExpander(Expander *expander, uint8_t I2CAddress, uint8_t IOdirA,
                  uint8_t IOdirB, uint8_t portGPIOA, uint8_t portGPIOB,
                  uint8_t portGPPUA, uint8_t portGPPUB, uint8_t inputsDirA,
                  uint8_t inputsDirB, uint8_t initialValueGPA,
                  uint8_t initialValueGPB);
void initShifter(Shifter *shifter, uint16_t dataPin, uint16_t clockPin,
                 uint16_t initialData);
void initCubeSystem(CubeSystem *system);
void initButton(Button *b, uint16_t pin);
#endif // COMPONENTS_HPP
