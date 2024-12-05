#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define ANDARES 11 // 6 reais + 5 imaginários
#define COLUNAS 6
#define LINHAS 6

// -------------------------------- Expander defines --------------------------------
// I2C Configuration
#define MCP23017_I2C_ADDRESS_1 0x24
#define MCP23017_I2C_ADDRESS_2 0x26
#define MCP23017_I2C_ADDRESS_3 0x27 

// MCP23017 Registers
#define IODIRA 0x00  // Registro de direção do Port A
#define IODIRB 0x01  // Registro de direção do Port B
#define GPIOA 0x12   // Registro GPIO Port A
#define GPIOB 0x13   // Registro GPIO Port B
#define OLATA 0x14   // Registro de latch de saída Port A
#define OLATB 0x15   // Registro de latch de saída Port B

#define OUTPUTTYPE 0
//#define INPUTTYPE 1

// -------------------------------- Shifter defines --------------------------------
#define DATA_PIN 17
#define CLOCK_PIN 4

// Cube initialization
typedef struct {
    unsigned char ledValues[ANDARES][COLUNAS][LINHAS];
    unsigned char toUpdateVal[ANDARES][COLUNAS][LINHAS];
} LedCube;


// MCP23017 I/O expander
typedef struct{
    int fd;
    bool type; // 0 for output, 1 for input
    uint16_t IOdirA;
    uint16_t IOdirB;
    uint16_t I2CAddress;
    uint16_t portGPIOA;
    uint16_t portGPIOB;
    uint16_t portOutLatchA;
    uint16_t portOutLatchB;
    uint8_t valueGPA;
    uint8_t valueGPB;
} Expander;

// 74HC164 shifter
typedef struct{
    int dataPin;
    int clockPin;
} Shifter;

typedef struct{
    LedCube Cube;
    Expander Expander1; // U1
    Expander Expander2; // U2
    Expander Expander3; // U3
    Shifter Shifter1;   // U4
    Shifter Shifter2;   // U5
} CubeSystem;


void initCubeSystem(CubeSystem* system);
void setExpanderVal(Expander* expander, uint16_t data);

#endif // SHIFTREGS_HPP
